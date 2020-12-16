// Copyright 2020 Igor Rukhovich
#include <algorithm>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./radix_sort_batcher.h"

template <class T>
void check_arrays(const std::vector<T>& lhs, const std::vector<T>& rhs) {
    ASSERT_EQ(lhs.size(), rhs.size());
    for (size_t i = 0; i < lhs.size(); ++i) {
        ASSERT_EQ(lhs[i], rhs[i]);
    }
}

void check_par_radix_batcher(size_t num_iterations, size_t num_elements) {
    for (size_t i = 0u; i < num_iterations; ++i) {
        std::vector<double> array = random_double_array(num_elements);
        std::vector<double> array_cpy(array);
        
        par_radix_sort_batcher(array.begin(), array.end());
        std::sort(array_cpy.begin(), array_cpy.end());

        check_arrays(array, array_cpy);
    }
}

TEST(RadixSortBatcherMerge, test_radix_empty) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<double> array(0u);
        std::vector<double> array_cpy(array);
        
        radix_sort(array.begin(), array.end());
        std::sort(array_cpy.begin(), array_cpy.end());

        check_arrays(array, array_cpy);
    }
}

TEST(RadixSortBatcherMerge, test_radix_one_element) {
    std::vector<double> array = random_double_array(1u);
    std::vector<double> array_cpy(array);
    
    radix_sort(array.begin(), array.end());
    std::sort(array_cpy.begin(), array_cpy.end());

    check_arrays(array, array_cpy);
}

TEST(RadixSortBatcherMerge, test_radix_few_elements) {
    std::vector<double> array = random_double_array(10u);
    std::vector<double> array_cpy(array);
    
    radix_sort(array.begin(), array.end());
    std::sort(array_cpy.begin(), array_cpy.end());
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::cout << "rank: " << rank << ", value: " << array[5] << '\n';

    check_arrays(array, array_cpy);
}

TEST(RadixSortBatcherMerge, test_radix_lots_of_elements) {
    std::vector<double> array = random_double_array(10000u);
    std::vector<double> array_cpy(array);
    
    radix_sort(array.begin(), array.end());
    std::sort(array_cpy.begin(), array_cpy.end());

    check_arrays(array, array_cpy);
}

TEST(RadixSortBatcherMerge, test_batcher_empty) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        check_par_radix_batcher(1u, 0u);
    }
}

TEST(RadixSortBatcherMerge, test_batcher_one_element) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        check_par_radix_batcher(10u, 1u);        
    }
}

TEST(RadixSortBatcherMerge, test_batcher_few_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        check_par_radix_batcher(10u, 10u);        
    }
}

TEST(RadixSortBatcherMerge, test_batcher_lots_of_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        check_par_radix_batcher(10u, 10000u);        
    }
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
