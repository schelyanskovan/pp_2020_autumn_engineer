// Copyright 2020 Kucheryavykh Yan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "./kucheryavykh_y_rectangles_method.h"

TEST(Parallel_MPI, Test_x3) {
    int p_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
    std::vector <double> begin_point = { 0, 0, 0 };
    std::vector <double> end_point = { 2, 2, 2 };
    double res_p, res_s;
    int fineness = 100;

    auto function = [](std::vector <double> vec) {
        return vec[0]*vec[0]*3;
    };
    double t = MPI_Wtime();
    res_p = rectangles_parallel(function, begin_point, end_point, fineness);
    if (p_rank == 0) {
        std::cout << "Parallel integral: " << res_p << std::endl;
        std::cout << "Parallel time: " << MPI_Wtime() - t << std::endl;
        t = MPI_Wtime();
        res_s = rectangles_sequential(function, begin_point, end_point, fineness);
        std::cout << "Sequential integral: " << res_p << std::endl;
        std::cout << "Sequential time: " << MPI_Wtime() - t << std::endl;

        EXPECT_NEAR(res_p, res_s, 0.1);
    }
}

TEST(Parallel_MPI, Test_sin) {
    int p_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
    std::vector <double> begin_point = { 0, 0, 1};
    std::vector <double> end_point = { 2, 2, 4 };
    double res_p, res_s;
    int fineness = 50;

    auto function = [](std::vector <double> vec) {
        return sin(vec[0]);
    };
    double t = MPI_Wtime();
    res_p = rectangles_parallel(function, begin_point, end_point, fineness);
    if (p_rank == 0) {
        std::cout << "Parallel integral: " << res_p << std::endl;
        std::cout << "Parallel time: " << MPI_Wtime() - t << std::endl;
        t = MPI_Wtime();
        res_s = rectangles_sequential(function, begin_point, end_point, fineness);
        std::cout << "Sequential integral: " << res_p << std::endl;
        std::cout << "Sequential time: " << MPI_Wtime() - t << std::endl;

        EXPECT_NEAR(res_p, res_s, 0.1);
    }
}

TEST(Parallel_MPI, Test_2x) {
    int p_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
    std::vector <double> begin_point = { 0, 0, 0 };
    std::vector <double> end_point = { 5, 5, 5 };
    double res_p, res_s;
    int fineness = 200;

    auto function = [](std::vector <double> vec) {
        return 2 * vec[0];
    };
    double t = MPI_Wtime();
    res_p = rectangles_parallel(function, begin_point, end_point, fineness);
    if (p_rank == 0) {
        std::cout << "Parallel integral: " << res_p << std::endl;
        std::cout << "Parallel time: " << MPI_Wtime() - t << std::endl;
        t = MPI_Wtime();
        res_s = rectangles_sequential(function, begin_point, end_point, fineness);
        std::cout << "Sequential integral: " << res_p << std::endl;
        std::cout << "Sequential time: " << MPI_Wtime() - t << std::endl;

        EXPECT_NEAR(res_p, res_s, 0.1);
    }
}

TEST(Parallel_MPI, Test_x_y_z) {
    int p_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
    std::vector <double> begin_point = { 0, 0, 0 };
    std::vector <double> end_point = { 3, 3, 3 };
    double res_p, res_s;
    int fineness = 100;

    auto function = [](std::vector <double> vec) {
        return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
    };
    double t = MPI_Wtime();
    res_p = rectangles_parallel(function, begin_point, end_point, fineness);
    if (p_rank == 0) {
        std::cout << "Parallel integral: " << res_p << std::endl;
        std::cout << "Parallel time: " << MPI_Wtime() - t << std::endl;
        t = MPI_Wtime();
        res_s = rectangles_sequential(function, begin_point, end_point, fineness);
        std::cout << "Sequential integral: " << res_p << std::endl;
        std::cout << "Sequential time: " << MPI_Wtime() - t << std::endl;

        EXPECT_NEAR(res_p, res_s, 0.1);
    }
}

TEST(Parallel_MPI, Test_sin_x_sin_y_z) {
    int p_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
    std::vector <double> begin_point = { 0, 0, 0, 1 };
    std::vector <double> end_point = { 2, 2, 2, 4 };
    double res_p, res_s;
    int fineness = 50;

    auto function = [](std::vector <double> vec) {
        return sin(vec[0]) + sin(vec[1]) + vec[2];
    };
    double t = MPI_Wtime();
    res_p = rectangles_parallel(function, begin_point, end_point, fineness);
    if (p_rank == 0) {
        std::cout << "Parallel integral: " << res_p << std::endl;
        std::cout << "Parallel time: " << MPI_Wtime() - t << std::endl;
        t = MPI_Wtime();
        res_s = rectangles_sequential(function, begin_point, end_point, fineness);
        std::cout << "Sequential integral: " << res_p << std::endl;
        std::cout << "Sequential time: " << MPI_Wtime() - t << std::endl;

        EXPECT_NEAR(res_p, res_s, 0.1);
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
