// Copyright 2020 Schelyanskova Anastasiya

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./radix_sort_double_simple_merge.h"

using namespace std;

TEST(Parallel_Operations_MPI, Test_Negative_Size) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = -666;
  if (rank == 0) {
  ASSERT_ANY_THROW(getRandomVector(size));
  }
}

TEST(Parallel_Operations_MPI, Test_Get_Random_Array)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 100;
  if (rank == 0) {
  ASSERT_NO_THROW(getRandomVector(size));
  }
}

TEST(Parallel_Operations_MPI, TEST_RadixSort_simple) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<double> vec{7.2, 9.56, 8.87, 5.35, 4.27, 7.9, 7.87};
  vector<double> res{4.1, 5.3, 7.2, 7.15, 7.18, 8.9, 9.23};
  if (rank == 0) 
  {
     vec = radixSort(vec, 1);
     for (int i = 0; i < 7; ++i)
     {
         EXPECT_EQ(vec[i], res[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_Check_First_Radix) 
{
int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<double> vec{71.1, 92.67, 86.56, 54.23, 43.12, 72.19, 77.87};
  vector<double> res{71.98, 92.65, 72.21, 43.23, 54.78, 86.98, 77.12};
  if (rank == 0) 
  {
     vec = radixSort(vec, 1);
     for (int i = 0; i < 7; ++i)
     {
         EXPECT_EQ(vec[i], res[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_Check_Two_Radix) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<double> vec{71.12, 92.24, 86.87, 54.45, 43.56, 72.34, 77.98};
  vector<double> res{43.123, 54.56, 71.45, 72.2, 77.23, 86.98, 92.76};
  if (rank == 0) {
     vec = radixSort(vec, 1);
     vec = radixSort(vec, 2);
     for (int i = 0; i < 7; ++i)
     {
         EXPECT_EQ(vec[i], res[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_Check_Three_Radix)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<double> vec{15.12, 192.54, 82.13, 387.87, 5.98, 634.23, 34.12};
  vector<double> res{5.12, 15.11, 34.23, 82.45, 192.56, 387.24, 634.48};
  if (rank == 0) 
  {
     vec = radixSort(vec, 1);
     vec = radixSort(vec, 2);
     vec = radixSort(vec, 3);
     for (int i = 0; i < 7; ++i)
     {
         EXPECT_EQ(vec[i], res[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_Check_Merge) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
     vector<double> vec1{1.23, 1.38, 3.23, 5.56};
     vector<double> vec2{0.23, 2.2, 3.6, 6.23};
     vector<double> res{0.8, 1.23, 1.776, 2.54, 3.65, 3.23, 5.123, 6.567};
     vector<double> vec3 = merge(vec1, vec2);
     for (int i = 0; i < 8; ++i)
     {
         EXPECT_EQ(vec3[i], res[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_ParallelRadixSort) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<double> vec{7.56, 9.21, 8.98, 5.24, 4.11, 7.23, 7.98, 2.1, 5.23, 8.1, 0.56, 6.23, 7.1, 1.23, 9.78, 2.56, 5.23, 6.56, 7.123, 1.123};
  vector<double> res = ParallelRadixSort(vec, 20);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) 
  {
     vector<double> check = checkVector(vec);
     for (int i = 0; i < 20; ++i)
     {
         EXPECT_EQ(res[i], check[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_ParallelRadixSort2) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<double> vec{7.13, 9.56, 8.24, 5.12, 4.67, 7.2, 7.3, 2.1, 5.4, 8.67, 0.123, 6.78, 7.45};
  vector<double> res = ParallelRadixSort(vec, 13);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) 
  {
     vector<double> check = checkVector(vec);
     for (int i = 0; i < 13; ++i)
     {
         EXPECT_EQ(res[i], check[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_ParallelRadixSort3) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<double> vec{73.24, 91.13, 817.89, 5.56, 42.12, 74.67, 701.56, 23.13, 59.11, 8.2, 0, 66.89, 773.45, 1024.1};
  vector<double> res = ParallelRadixSort(vec, 14);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0)
  {
     vector<double> check = checkVector(vec);
     for (int i = 0; i < 14; ++i)
     {
         EXPECT_EQ(res[i], check[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_ParallelRadixSort_Random) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> vec;
  if (rank == 0)
  {
      vec = getRandomVector(14);
  }
  vector<double> res = ParallelRadixSort(vec, 14);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) 
  {
     std::vector<double> check = checkVector(vec);
     for (int i = 0; i < 14; ++i)
     {
         EXPECT_EQ(res[i], check[i]) << "Arrays check_array and res_array differ at index " << i;
     }
  }
}

TEST(Parallel_Operations_MPI, TEST_ParallelRadixSortRandom) 
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<double> vec;
  if (rank == 0)
  {
      vec = getRandomVector(23);
  }
  vector<double> res = ParallelRadixSort(vec, 23);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) 
  {
     vector<double> check = checkVector(vec);
     for (int i = 0; i < 23; ++i)
     {
         EXPECT_EQ(res[i], check[i]) << "Arrays check_array and res_array differ at index " << i;
     }
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
