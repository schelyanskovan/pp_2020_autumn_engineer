// Copyright 2020 Khismatulina Karina

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <stack>
#include <iostream>
#include "../../../modules/task_3/khismatulina_k_graham/graham.h"

TEST(khismatulina_task_3, test_1_error) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(GenPoints(-1));
    }
}

TEST(khismatulina_task_3, test_2_par_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> Points{ {0, 0}, {1, 5}, {3, 6}, {-1, -4}, {2, 2}, {1, 2.5}, {1, -1}, {0, 1}, {0, 3}, {4, -1} };
    std::vector<Point> P = Sort(Points);
    std::vector<Point> res = GrahamPar(P);
    std::vector<Point> check{ {-1, 0}, {4, 5}, {3, 6}, {1, -4}, {0, 2} };
    bool a = 1;
    if (rank == 0) {
        for (int i = 0; i < res.size(); ++i)
            if (res[i] != check[i])
                a = 0;
        EXPECT_EQ(a, 1);
    }
}

TEST(khismatulina_task_3, test_3_par_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> P = GenPoints(100);
    std::vector<Point> res = GrahamPar(P);
    std::vector<Point> check = GrahamSeq(P);
    bool a = 1;
    if (rank == 0) {
        for (int i = 0; i < res.size(); ++i)
            if (res[i] != check[i])
                a = 0;
        EXPECT_EQ(a, 1);
    }
}

TEST(khismatulina_task_3, test_4_par_200) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> P = GenPoints(200);
    std::vector<Point> res = GrahamPar(P);
    std::vector<Point> check = GrahamSeq(P);
    bool a = 1;
    if (rank == 0) {
        for (int i = 0; i < res.size(); ++i)
            if (res[i] != check[i])
                a = 0;
        EXPECT_EQ(a, 1);
    }
}

TEST(khismatulina_task_3, test_5_par_500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> P = GenPoints(500);
    std::vector<Point> res = GrahamPar(P);
    std::vector<Point> check = GrahamSeq(P);
    bool a = 1;
    if (rank == 0) {
        for (int i = 0; i < res.size(); ++i)
            if (res[i] != check[i])
                a = 0;
        EXPECT_EQ(a, 1);
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