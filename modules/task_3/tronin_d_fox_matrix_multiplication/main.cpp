// Copyright 2020 Tronin Dmitry
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include <ctime>
#include "../../../modules/task_3/tronin_d_fox_matrix_multiplication/fox_matrix_multiplication.h"

#define LOG_INFO std::cout << "[          ] "

void CreateTest(size_t size) {
    int comm_size, comm_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
    if (static_cast<int>(sqrt(comm_size))
        * static_cast<int>(sqrt(comm_size))
        != comm_size) {
        return;
    }
    std::vector<double> left, right, sequential;
    if (comm_rank == 0) {
        left = getRandomVector(size);
        right = getRandomVector(size);
    }
    double seq_start, seq_stop;
    if (comm_rank == 0) {
        seq_start = MPI_Wtime();
        sequential = SequentialMultiplication(left, right, size);
        seq_stop = MPI_Wtime();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    auto par_start = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    auto answer = ParallelFoxMultiplication(left, right, size);
    MPI_Barrier(MPI_COMM_WORLD);
    auto par_stop = MPI_Wtime();
    if (comm_rank == 0) {
        for (size_t row = 0; row < size; ++row) {
            for (size_t column = 0; column < size; ++column) {
                ASSERT_NEAR(answer[row * size + column],
                            sequential[row * size + column],
                            1e-5) << row << ' ' << column << ' '
                                  << answer[row * size + column];
            }
        }
        LOG_INFO << "Size of the matrix " << size << '\n';
        LOG_INFO << "Sequential method "
                 << seq_stop - seq_start << '\n';
        LOG_INFO << "Parallel method "
                 << par_stop - par_start << '\n';
    }
}

TEST(FoxMultiplication, VerySmall) {
    CreateTest(12);
}

TEST(FoxMultiplication, Small) {
    CreateTest(36);
}

TEST(FoxMultiplication, Medium) {
    CreateTest(324);
}

TEST(FoxMultiplication, Big) {
    CreateTest(648);
}
TEST(FoxMultiplication, Large) {
    CreateTest(1296);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
