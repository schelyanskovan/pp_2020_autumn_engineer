// Copyright 2020 Konstantin Sandalov

#include "../../modules/task_3/sandalov_k_histogram_stretching/histogram_stretching.hpp"
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <array>

TEST(Parallel_Operations_MPI, ZeroCheck) {
    int procSize, procRank;
    float parTime;
    float seqTime;
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    std::vector<int> randArray;
    std::vector<int> copy;
    if (procRank == 0) {
        createRandomImage(&randArray, 100, 100);
        copy = randArray;
        // for (const auto& el : randArray) std::cout << el << '\n';
        // std::cout << '\n';
    }
    parTime = MPI_Wtime();
    parallelHistogramStretching(&randArray);
    parTime = MPI_Wtime() - parTime;
    // if (procRank == 0) {
    //     std::cout << '\n';
    //     for (const auto& el : randArray) std::cout << el << '\n';
    // }
    if (procRank == 0) {
        seqTime = MPI_Wtime();
        seqHistogramStretching(&copy);
        seqTime = MPI_Wtime() - seqTime;
        for (size_t i = 0; i < randArray.size(); ++i) {
            ASSERT_EQ(copy[i], randArray[i]);
        }
        std::cout << "SeqTime: " << seqTime << std::endl;
        std::cout << "ParTime: " << parTime << std::endl;
    }
}

TEST(Parallel_Operations_MPI, FirstCheck) {
    int procSize, procRank;
    float parTime;
    float seqTime;
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    std::vector<int> randArray;
    std::vector<int> copy;
    if (procRank == 0) {
        createRandomImage(&randArray, 1000, 1000);
        copy = randArray;
        // for (const auto& el : randArray) std::cout << el << '\n';
        // std::cout << '\n';
    }
    parTime = MPI_Wtime();
    parallelHistogramStretching(&randArray);
    parTime = MPI_Wtime() - parTime;
    // if (procRank == 0) {
    //     std::cout << '\n';
    //     for (const auto& el : randArray) std::cout << el << '\n';
    // }
    if (procRank == 0) {
        seqTime = MPI_Wtime();
        seqHistogramStretching(&copy);
        seqTime = MPI_Wtime() - seqTime;
        for (size_t i = 0; i < randArray.size(); ++i) {
            ASSERT_EQ(copy[i], randArray[i]);
        }
        std::cout << "SeqTime: " << seqTime << std::endl;
        std::cout << "ParTime: " << parTime << std::endl;
    }
}

TEST(Parallel_Operations_MPI, SecondCheck) {
    int procSize, procRank;
    float parTime;
    float seqTime;
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    std::vector<int> randArray;
    std::vector<int> copy;
    if (procRank == 0) {
        createRandomImage(&randArray, 10000, 10000);
        copy = randArray;
        // for (const auto& el : randArray) std::cout << el << '\n';
        // std::cout << '\n';
    }
    parTime = MPI_Wtime();
    parallelHistogramStretching(&randArray);
    parTime = MPI_Wtime() - parTime;
    // if (procRank == 0) {
    //     std::cout << '\n';
    //     for (const auto& el : randArray) std::cout << el << '\n';
    // }
    if (procRank == 0) {
        seqTime = MPI_Wtime();
        seqHistogramStretching(&copy);
        seqTime = MPI_Wtime() - seqTime;
        for (size_t i = 0; i < randArray.size(); ++i) {
            ASSERT_EQ(copy[i], randArray[i]);
        }
        std::cout << "SeqTime: " << seqTime << std::endl;
        std::cout << "ParTime: " << parTime << std::endl;
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