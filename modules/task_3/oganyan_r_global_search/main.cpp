//  Copyright by Oganyan Robert 2020
#include <iostream>
#include "../../../modules/task_3/oganyan_r_global_search/functions.h"
#include "../../../modules/task_3/oganyan_r_global_search/global_search.h"

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>

using std::cout;
using std::endl;

void CreateTest(const std::function<double(dpair)>& func, const std::function<dpair(dpair)>& grad,
    double x_left, double x_right, double y_left, double y_right,
    double ans_minimum, dpair ans_pos) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double time_start { MPI_Wtime()};
   // const d_dpair res_paral { ParallelGlobalSearch(func, grad, x_left, x_right, y_left, y_right) };
    /*
    if (rank == 0) {
        const double time_end { MPI_Wtime()};
        cout << "Parallel Time :" << time_end - time_start << endl;
        ASSERT_EQ(1, abs(ans_minimum - res_paral.first) <= 0.1);
    }
*/
    if (rank == 0) {
        time_start = MPI_Wtime();
        const d_dpair res_seq { SequentialGlobalSearch(func, grad, x_left, x_right, y_left, y_right) };
        const double time_end_s { MPI_Wtime() };
        cout << "Sequential Time :" << time_end_s - time_start << endl;
        cout<<std::fixed<<std::setprecision(10);
        cout<< res_seq.x << endl;
        cout<<res_seq.second.first<< " " << res_seq.second.second<<endl;
        ASSERT_EQ(1, abs(ans_minimum - res_seq.first) <= 0.15);
    }
}

TEST(Parallel_Operations_MPI, Test_first_fun) {
    const std::function<double(dpair)> func { fun_first };
    const std::function<dpair(dpair)> grad { grad_first };
   CreateTest(func, grad, -3.0, 3.0, -2.0, 6.0, 0.0, {0.0, 0.0});
}

TEST(Parallel_Operations_MPI, Test_second_fun) {
    const std::function<double(dpair)> func {fun_second};
    const std::function<dpair(dpair)> grad { grad_second };
    CreateTest(func, grad, -3.0, 3.0, -2.0, 6.0, -2.0, {0.0, 1.0});
}
/*
TEST(Parallel_Operations_MPI, Test_third_fun) {
    const std::function<double(dpair)> func {fun_third};
    const std::function<dpair(dpair)> grad { grad_third };
    double anss = -8.0 / (2.71828 * 2.71828);
    CreateTest(func, grad, -10.0, 10.0, -10.0, 10.0,
        anss, {-4.0, 2.0});
}


TEST(Parallel_Operations_MPI, Test_forth_fun) {
    const std::function<double(dpair)> func {fun_forth};
    const std::function<dpair(dpair)> grad { grad_forth };
    CreateTest(func, grad, -5.0, 5.0, -5.0, 5.0, 4.0, {1.0, 0.5});

}
/*
TEST(Parallel_Operations_MPI, Test_large) {

}
*/
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