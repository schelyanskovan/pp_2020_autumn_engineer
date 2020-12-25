// Copyright 2020 Dmitriy Ryazanov
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./method_moore.h"

TEST(Parallel_Operations_MPI, Create_Random_Graph) {
  ASSERT_NO_THROW(CreateGraph(30));
}

TEST(Parallel_Operations_MPI, Method_Moore_Sequential) {
  std::vector<int> Graph;
  Graph = CreateGraph(30);
  ASSERT_NO_THROW(MethodMoore(&Graph));
}

TEST(Parallel_Operations_MPI, Method_Moore_Parallel_To_30) {
  std::vector<int> Graph;
  Graph = CreateGraph(30);
  ASSERT_NO_THROW(MethodMoore(&Graph));
}

TEST(Parallel_Operations_MPI, Method_Moore_Parallel_To_10) {
  std::vector<int> Graph;
  Graph = CreateGraph(10);
  ASSERT_NO_THROW(MethodMoore(&Graph));
}

TEST(Parallel_Operations_MPI, Sequential_And_Parallel_Operations) {
  int size = 10;
  std::vector<int> Graph;
  std::vector<int> result;
  std::vector<int> pResult;
  Graph = CreateGraph(size);
  result = MethodMoore(&Graph);
  pResult = MethodMooreP(&Graph);
  for (int i = 0; i < size; i++) {
    EXPECT_EQ(result[i], pResult[i]);
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
