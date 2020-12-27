// Copyright 2020 Mikhail Baulin
#include <gtest/gtest.h>

#include <numeric>
#include <vector>

#include "../../../3rdparty/gtest-mpi/linux/gtest-mpi-listener.hpp"
#include "./matrix_vectort_mul_mpi.h"

TEST(Parallel_matrix_vectort_mult_MPI, Test_1) {
  int rank;
  int rows = 3;
  int cols = 3;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> mat{2, 4, 0, -2, 1, 3, -1, 0, 1};

  std::vector<int> vec;

  if (rank == 0) {
    vec = std::vector<int>{1, 2, -1};
  }

  std::vector<int> global_max;
  global_max = getParallelOperations(mat, rows, cols, vec);

  if (rank == 0) {
    std::vector<int> reference_result =
        getSequentialOperations(mat, rows, cols, vec);
    ASSERT_EQ(reference_result, global_max);
  }
}

TEST(Parallel_matrix_vectort_mult_MPI, Test_2) {
  int rank;
  int rows = 4;
  int cols = 2;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> mat{3, 3, 2, 2, 0, 0, 1, 1};

  std::vector<int> vec;

  if (rank == 0) {
    vec = std::vector<int>{-1, 1};
  }

  std::vector<int> global_result;
  global_result = getParallelOperations(mat, rows, cols, vec);

  if (rank == 0) {
    std::vector<int> reference_result = std::vector<int>{0, 0, 0, 0};
    ASSERT_EQ(reference_result, global_result);
  }
}

TEST(Parallel_matrix_vectort_mult_MPI, Random_Test_1) {
  int rank;
  int rows = 5;
  int cols = 3;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> mat = getMatrixWithRandomNumbers(rows, cols);

  std::vector<int> vec;

  if (rank == 0) {
    vec = getMatrixWithRandomNumbers(cols, 1);
  }

  std::vector<int> global_result;
  global_result = getParallelOperations(mat, rows, cols, vec);

  if (rank == 0) {
    std::vector<int> reference_result =
        getSequentialOperations(mat, rows, cols, vec);
    ASSERT_EQ(reference_result, global_result);
  }
}

TEST(Parallel_matrix_vectort_mult_MPI, Random_Test_2_many_items) {
  int rank;
  int rows = 100;
  int cols = 100;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> mat = getMatrixWithRandomNumbers(rows, cols);

  std::vector<int> vec;

  if (rank == 0) {
    vec = getMatrixWithRandomNumbers(cols, 1);
  }

  std::vector<int> global_result;
  global_result = getParallelOperations(mat, rows, cols, vec);

  if (rank == 0) {
    std::vector<int> reference_result =
        getSequentialOperations(mat, rows, cols, vec);
    ASSERT_EQ(reference_result, global_result);
  }
}

TEST(Parallel_matrix_vectort_mult_MPI, Random_Test_3_all_zero) {
  int rank;
  int rows = 250;
  int cols = 100;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> mat = getMatrixWithRandomNumbers(rows, cols);

  std::vector<int> vec;

  if (rank == 0) {
    vec = std::vector<int>(cols);
  }

  std::vector<int> global_result;
  global_result = getParallelOperations(mat, rows, cols, vec);

  if (rank == 0) {
    std::vector<int> reference_result(rows);
    ASSERT_EQ(reference_result, global_result);
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
