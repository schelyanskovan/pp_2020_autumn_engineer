// Copyright 2020 Stoicheva Darya
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "sort_batcher_mpi.hpp"

using namespace std;

template<typename result, typename source>
result pointer_cast(source *v) {
    return static_cast<result>(static_cast<void*>(v));
}


template<typename result, typename source>
result pointer_cast(const source *v) {
    return static_cast<result>(const_cast<void *>(static_cast<const void*>(v)));
}


void print_vector(const std::vector<int> vector, const size_t size, const std::string prefix,
    const std::string label, std::ostream &out) {
    print_vector<int>(vector, size, prefix, label, out);
}


void print_vector(const std::vector<float> vector, const size_t size, const std::string prefix,
    const std::string label, std::ostream &out) {
    print_vector<float>(vector, size, prefix, label, out);
}


void print_vector(const std::vector<double> vector, const size_t size, const std::string prefix,
    const std::string label, std::ostream &out) {
    print_vector<double>(vector, size, prefix, label, out);
}


template<typename T>
void print_vector(const std::vector<T> vector, const size_t size, const std::string prefix,
    const std::string label, std::ostream &out) {
#ifdef DEBUG_PRINT
    std::stringstream buffer;
    buffer << prefix + (label.empty() ? "Vector" : label) + ": {" << std::endl;
    buffer << prefix << "  ";
    for (size_t i = 0; i < size; i++) {
        buffer << vector[i] << ", ";
    }
    buffer << std::endl << prefix << "}" << std::endl;
    out << buffer.str();
#endif
}


template<typename T>
std::vector<T> getSequentialOperations(const std::vector<T> source) {
    std::vector<T> sorted(source);
    std::sort(sorted.begin(), sorted.end());

    return sorted;
}
template std::vector<int> getSequentialOperations<int>(const std::vector<int> source);
template std::vector<float>  getSequentialOperations<float>(const std::vector<float> source);
template std::vector<double>  getSequentialOperations<double>(const std::vector<double> source);


template <typename T>
int compare(const void * a, const void * b)
{
    T *aa = reinterpret_cast<T *>(const_cast<void *>(a));
    T *bb = reinterpret_cast<T *>(const_cast<void *>(b));

    if (*aa > *bb) {
        return 1;
    } else if (*aa < *bb) {
        return -1;
    }
    return 0;
}
//template int compare(const int *i, const int *j);
//template float compare(const float *i, const float *j);
//template double compare(const double *i, const double *j);

template<typename T>
std::vector<T> getParallelOperations(const std::vector<T> source) {
    std::vector<T> result(source);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm comm = MPI_COMM_WORLD;
    
    size_t count_items_per_proc = size_t(std::max(1, int(source.size() / world_size)));
    size_t count_extra_items = source.size() < world_size ? 0 : source.size() % world_size;
    
    if (count_items_per_proc < 3) {
        if (rank == 0) {
            // dbg printf("[%d] rank = %d  world_size = %d,  size = %d,  count per process = %d\n", rank, rank, world_size, source.size(), count_items_per_proc);
            printf("[%d] Quick sort.\n", rank);
            qsort(result.data(), result.size(), sizeof(T), compare<T>);
            return result;
        }
        return result;
    }

    // 1. Scatter source data 
    if (rank == 0) {

    }

    std::vector<T> proc_source(count_items_per_proc);
    MPI_Scatter(source.data(), source.size(), MPI_INT, 

    // 2. Each process make qsort of local data

    // 3. 

    //std::sort(result.begin(), result.end());
    return result;
}
template std::vector<int> getParallelOperations<int>(const std::vector<int> source);
template std::vector<float>  getParallelOperations<float>(const std::vector<float> source);
template std::vector<double>  getParallelOperations<double>(const std::vector<double> source);



