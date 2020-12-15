// Copyright 2020 Gavrilov Nikita
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#include <stdio.h>
#include <mpi.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "BatcherMergingSort.h"
#include <vector>
#include <iostream>
#include <random>
#include <string>
#include <ctime>

bool IsSorted(const std::vector<int>& data) {
    for (size_t i = 0; i < data.size()-1; i++) {
        if (data[i] > data[i + 1])
            return false;
    }
    return true;
}
std::vector<int> GetRandomVector(size_t size) {
    std::mt19937 rnd;
    rnd.seed(static_cast<unsigned int>(time(0)));

    std::vector<int> result(size);
    for (size_t i = 0; i < size; i++) {
        result[i] = rnd() % 20000 - 10000;
    }

    return result;
}


TEST(Parallel_Operations_MPI, No_Throw) {
    std::vector<int> data = GetRandomVector(10000);
    ASSERT_NO_THROW(Sort(&data));
}

TEST(Parallel_Operations_MPI, Sorting_Right) {
    std::vector<int> data = GetRandomVector(10000);
    Sort(&data);
    ASSERT_TRUE(IsSorted(data));
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
