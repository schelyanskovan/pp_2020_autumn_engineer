// Copyright 2020 Denis Kadnikov
#include "../../../modules/task_3/kadnikov_d_shell_sort_simple/shell_sort_simple.h"

#include <mpi.h>
#include <time.h>

#include <cstdlib>
#include <iostream>
#include <random>

int* createRandomArray(int arraySize) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  int* array = new int[arraySize];
  for (int i = 0; i < arraySize; i++) {
    array[i] = gen() % 1000;
  }
  return array;
}

void shellSort(int* arr, int n) {
  int gap;
  for (gap = 1; gap < n / 3; gap = gap * 3 + 1) {
  }
  for (; gap > 0; gap = (gap - 1) / 3) {
    for (int i = gap; i < n; i++) {
      int temp = arr[i];
      int j;
      for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
        arr[j] = arr[j - gap];

      arr[j] = temp;
    }
  }
}

void getParallelOperations(int arraySize) {
  if (arraySize < 1) throw(1);

  int size, rank;
  int* array = new int[arraySize];
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  srand(time(NULL));

  if (rank == 0) {
    array = createRandomArray(arraySize);
  }

  int* subArray;
  int n = size > 1 ? arraySize / (size - 1) : arraySize;
  double begin;
  double end;

  if (rank == 0) {
    begin = clock();

    for (int i = 1; i < size; i++) {
      MPI_Send(array + n * (i - 1), n, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    int k = arraySize - n * (size - 1);
    subArray = new int[k];

    for (int i = n * (size - 1); i < arraySize; i++) {
      subArray[i - n * (size - 1)] = array[i];
    }

    shellSort(subArray, k);

    int* rArray = new int[arraySize];

    for (int i = 0; i < k; i++) {
      rArray[i] = subArray[i];
    }

    for (int i = 1; i < size; i++) {
      MPI_Recv(rArray + n * (i - 1) + k, n, MPI_INT, MPI_ANY_SOURCE, 1,
               MPI_COMM_WORLD, &status);
    }

    shellSort(rArray, arraySize);
    end = clock();

    std::cout << "Time elapsed " << (end - begin) / CLOCKS_PER_SEC << "seconds";
  } else {
    subArray = new int[n];
    MPI_Recv(subArray, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    shellSort(subArray, n);
    MPI_Send(subArray, n, MPI_INT, 0, 1, MPI_COMM_WORLD);
  }
}
