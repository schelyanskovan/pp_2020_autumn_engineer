// Copyright 2020 Sozinov Alex
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./gauss_horisontal.h"

TEST(Parallel_Operations_MPI, test_find_seq) {
  std::vector<double> coefs{ 1, 3, 2,
                             2, 7, 5,
                             1, 4, 6 };
  std::vector<double> pPart{ 1, 18, 26 };
  ASSERT_NO_THROW(GetSequentialSolution(coefs, pPart, 3));
}

TEST(Parallel_Operations_MPI, test_find_par) {
  std::vector<double> coefs{ 1, 3, 2,
                             2, 7, 5,
                             1, 4, 6 };
  std::vector<double> pPart{ 1, 18, 26 };
  ASSERT_NO_THROW(GetParalSolution(coefs, pPart, 3));
}

TEST(Parallel_Operations_MPI, test_sol_1) {
  int size = 1000;
  std::vector<double> coefs = GetMatrixCoef(size);
  std::vector<double> pPart = GetRightPart(size);
  double start1 = MPI_Wtime();

  std::vector<double> p = GetParalSolution(coefs, pPart, size);

  double end1 = MPI_Wtime();
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    double start2 = MPI_Wtime();

    std::vector<double> s = GetSequentialSolution(coefs, pPart, size);
    double end2 = MPI_Wtime();
    printf("Matrix 1000x1000\nSequential - %f\nParallel - %f", end2 - start2, end1 - start1);
    ASSERT_EQ(s, p);
  }
}

TEST(Parallel_Operations_MPI, test_sol_2) {
  int size = 500;
  std::vector<double> coefs = GetMatrixCoef(size);
  std::vector<double> pPart = GetRightPart(size);
  double start1 = MPI_Wtime();

  std::vector<double> p = GetParalSolution(coefs, pPart, size);

  double end1 = MPI_Wtime();
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    double start2 = MPI_Wtime();

    std::vector<double> s = GetSequentialSolution(coefs, pPart, size);
    double end2 = MPI_Wtime();
    printf("Matrix 500x500\nSequential - %f\nParallel - %f", end2 - start2, end1 - start1);
    ASSERT_EQ(s, p);
  }
}

TEST(Parallel_Operations_MPI, test_sol_3) {
  int size = 10;
  std::vector<double> coefs = GetMatrixCoef(size);
  std::vector<double> pPart = GetRightPart(size);
  double start1 = MPI_Wtime();

  std::vector<double> p = GetParalSolution(coefs, pPart, size);

  double end1 = MPI_Wtime();
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    double start2 = MPI_Wtime();

    std::vector<double> s = GetSequentialSolution(coefs, pPart, size);
    double end2 = MPI_Wtime();
    printf("Matrix 10x10\nSequential - %f\nParallel - %f", end2 - start2, end1 - start1);
    ASSERT_EQ(s, p);
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
