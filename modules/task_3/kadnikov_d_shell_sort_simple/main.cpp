// Copyright 2020 Denis Kadnikov
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./shell_sort_simple.h"

TEST(Parallel_shell_sort_simple_MPI, can_create_random_array) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int* array1;
  const int a_size = 100;

  ASSERT_NO_THROW(array1 = createRandomArray(a_size));
}

TEST(Parallel_shell_sort_simple_MPI, can_sort_array_with_1_elem) {
  int arraySize = 1;

  ASSERT_NO_THROW(getParallelOperations(arraySize));
}

TEST(Parallel_shell_sort_simple_MPI, can_sort_array_with_100_elems) {
  int arraySize = 100;

  ASSERT_NO_THROW(getParallelOperations(arraySize));
}

TEST(Parallel_shell_sort_simple_MPI, can_sort_array_with_1000_elems) {
  int arraySize = 1000;

  ASSERT_NO_THROW(getParallelOperations(arraySize));
}

TEST(Parallel_shell_sort_simple_MPI, cannot_create_array_with_negative_size) {
  int arraySize = -10;

  ASSERT_ANY_THROW(getParallelOperations(arraySize));
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
