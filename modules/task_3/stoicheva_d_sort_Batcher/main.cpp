// Copyright 2020 Stoicheva Darya
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "sort_batcher_mpi.hpp"

using namespace std;

template<typename T>
std::vector<T> create_random_vector(size_t size) {
    assert(size >= 0);
    std::vector<T> vector(size);
    std::mt19937 gen(uint32_t(time(0)));
    std::uniform_int_distribution<int> uid_int(-50, +50);
    std::uniform_real_distribution<float> uid_real(-50.0, +50.0);

    for (size_t i = 0; i < size; i++) {
        vector[i] = std::is_same<T, int>::value ? uid_int(gen) : uid_real(gen);
    }

    return vector;
}


template<typename T>
void test_sort_with(std::vector<T> source) {

    std::vector<T> expected = getSequentialOperations<T>(source);

    test_sort_with(source, expected);
}


template<typename T>
void test_sort_with(std::vector<T> source, std::vector<T> expected) {

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm comm = MPI_COMM_WORLD;

    if (rank == 0) {
    }

    std::vector<T> result = getParallelOperations<T>(source);

    if (rank == 0) {

        ASSERT_EQ(source.size(), result.size()) << "Vectors source and result are of unequal length";
        ASSERT_EQ(source.size(), expected.size()) << "Vectors source and expected are of unequal length";

        ASSERT_EQ(expected, result);
        
        //const std::string message("expected and result differ at index ");
        //for (size_t i = 0; i < expected.size(); i++) {
        //    if (std::is_same<T, int>::value ) {
        //        ASSERT_EQ(expected[i], result[i]) << "Vector<int> " << message << i;
        //    } else {
        //       ASSERT_NEAR(expected[i], result[i], 0.001) << "Vector<float> " << message << i;
        //    }
        //}
    }

}


TEST(Parallel_Operations_MPI, Test_sort_Batcher_EmptyArray) {
    std::vector<int> source({});
    std::vector<int> expected({});

    test_sort_with<int>(source, expected);
}


TEST(Parallel_Operations_MPI, Test_sort_Batcher_OneElement) {
    std::vector<int> source({777});
    std::vector<int> expected({777});

    test_sort_with<int>(source, expected);
}


TEST(Parallel_Operations_MPI, Test_sort_Batcher_TenElements) {
    std::vector<int> source({ 5, 2, 1, 6, 0, 8, 4, 3, 9, 7 });
    std::vector<int> expected({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    test_sort_with<int>(source, expected);
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
