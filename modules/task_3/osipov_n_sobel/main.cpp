// Copyright 2020 Osipov Nikolay
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_3/osipov_n_sobel/sobel.h"

TEST(test_sobel, 3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 3;
    int cols = 3;
    std::vector<int> mat(rows * cols);
    mat[0] = 14;
    mat[1] = 35;
    mat[2] = 75;
    mat[3] = 203;
    mat[4] = 76;
    mat[5] = 3;
    mat[6] = 14;
    mat[7] = 78;
    mat[8] = 51;
    std::vector<int> parallel(rows * cols);
    parallel = parallelSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sequential(rows * cols);
        sequential = sequentialSobel(mat, rows, cols);
        ASSERT_EQ(sequential, parallel);
    }
}

TEST(test_sobel, 3x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 3;
    int cols = 4;
    std::vector<int> mat(rows * cols);
    mat[0] = 3;
    mat[1] = 240;
    mat[2] = 48;
    mat[3] = 87;
    mat[4] = 21;
    mat[5] = 6;
    mat[6] = 87;
    mat[7] = 96;
    mat[8] = 64;
    mat[9] = 245;
    mat[10] = 165;
    mat[11] = 5;
    std::vector<int> parallel(rows * cols);
    parallel = parallelSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sequential(rows * cols);
        sequential = sequentialSobel(mat, rows, cols);
        ASSERT_EQ(sequential, parallel);
    }
}

TEST(test_sobel, 5x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 5;
    int cols = 4;
    std::vector<int> mat(rows * cols);
    mat[0] = 96;
    mat[1] = 43;
    mat[2] = 249;
    mat[3] = 6;
    mat[4] = 18;
    mat[5] = 85;
    mat[6] = 53;
    mat[7] = 22;
    mat[8] = 77;
    mat[9] = 66;
    mat[10] = 185;
    mat[11] = 54;
    mat[12] = 1;
    mat[13] = 114;
    mat[14] = 75;
    mat[15] = 81;
    mat[16] = 164;
    mat[17] = 8;
    mat[18] = 93;
    mat[19] = 5;
    std::vector<int> parallel(rows * cols);
    parallel = parallelSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sequential(rows * cols);
        sequential = sequentialSobel(mat, rows, cols);
        ASSERT_EQ(sequential, parallel);
    }
}

TEST(test_sobel, 4x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 4;
    int cols = 4;
    std::vector<int> mat(rows * cols);
    mat[0] = 185;
    mat[1] = 5;
    mat[2] = 145;
    mat[3] = 146;
    mat[4] = 142;
    mat[5] = 150;
    mat[6] = 6;
    mat[7] = 9;
    mat[8] = 53;
    mat[9] = 7;
    mat[10] = 175;
    mat[11] = 1;
    mat[12] = 2;
    mat[13] = 13;
    mat[14] = 4;
    mat[15] = 5;
    std::vector<int> parallel(rows * cols);
    parallel = parallelSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sequential(rows * cols);
        sequential = sequentialSobel(mat, rows, cols);
        ASSERT_EQ(sequential, parallel);
    }
}

TEST(test_sobel, 5x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 5;
    int cols = 5;
    std::vector<int> mat(rows * cols);
    mat[0] = 1;
    mat[1] = 2;
    mat[2] = 3;
    mat[3] = 4;
    mat[4] = 25;
    mat[5] = 6;
    mat[6] = 7;
    mat[7] = 8;
    mat[8] = 9;
    mat[9] = 10;
    mat[10] = 11;
    mat[11] = 64;
    mat[12] = 235;
    mat[13] = 250;
    mat[14] = 12;
    mat[15] = 13;
    mat[16] = 14;
    mat[17] = 15;
    mat[18] = 16;
    mat[19] = 17;
    mat[20] = 18;
    mat[21] = 19;
    mat[22] = 20;
    mat[23] = 98;
    mat[24] = 89;
    std::vector<int> parallel(rows * cols);
    parallel = parallelSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sequential(rows * cols);
        sequential = sequentialSobel(mat, rows, cols);
        ASSERT_EQ(sequential, parallel);
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
