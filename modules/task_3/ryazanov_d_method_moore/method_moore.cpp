// Copyright 2020 Dmitriy Ryazanov
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include "../../modules/task_3/ryazanov_d_method_moore/method_moore.h"



std::vector<int> CreateGraph(int size) {
  std::mt19937 rand_r;
  rand_r.seed(static_cast<unsigned int>(time(0)));
  int edges = size;
  std::vector<int> graph(size * edges);
  const int max_point = 99999999;

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < edges; j++) {
      graph[i + (j * edges)] = (rand_r() % 100);
      if (graph[i + (j * edges)] == 0) {
        graph[i + (j * edges)] = max_point;
      }
      if (i == j) {
        graph[i + (j * edges)] = 0;
      }
    }
  }
  return graph;
}

std::vector<int> MethodMoore(std::vector<int>* graph) {
  const int max_point = 99999999;
  int size = static_cast<int>(sqrt(static_cast<int>(graph->size())));
  std::vector<int> result(size, max_point);
  if (size < 1) {
    throw "Error";
  }
  result[0] = 0;
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size; j++) {
      for (int k = 0; k < size; k++) {
        if (result[j] < max_point && ((*graph)[k + (j * size)] < max_point)) {
          if (result[k] > result[j] + (*graph)[k + (j * size)]) {
            result[k] = result[j] + (*graph)[k + (j * size)];
          }
        }
      }
    }
  }
  return result;
}

std::vector<int> MethodMooreP(std::vector<int>* graph) {
  const int max_point = 99999999;
  int size = static_cast<int>(sqrt(static_cast<int>(graph->size())));
  if (size < 1) {
    throw "Error";
  }
  std::vector<int> result(size, max_point);
  result[0] = 0;


  int rank = -1;
  int proc = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc);

  int step = size / proc;
  std::vector<int> distanceproc((step + (rank < (size % proc) ? 1 : 0)), max_point);
  std::vector<int> procgaraph((step + (rank < (size % proc) ? 1 : 0)) * size);

  std::vector<int> sendcounts(proc);
  std::vector<int> displs(proc);
  std::vector<int> sendcounts_dist(proc);
  std::vector<int> displs_dist(proc);

  displs[0] = 0;
  displs_dist[0] = 0;
  for (int i = 0; i < proc; i++) {
    sendcounts[i] = (step + (i < (size % proc) ? 1 : 0)) * size;
    sendcounts_dist[i] = step + (i < size % proc ? 1 : 0);
    if (i > 0) {
      displs[i] = (displs[(i - 1)] + sendcounts[(i - 1)]);
      displs_dist[i] = displs_dist[(i - 1)] + sendcounts_dist[(i - 1)];
    }
  }

  int root_proc = -1;
  for (int i = 0; i < proc - 1; ++i)
    if (0 >= displs[i] / size) root_proc++;
  if (rank == root_proc)
    distanceproc[0 - (displs[rank] / size)] = 0;

  std::vector<int> sendbuf(size * size);
  if (rank == 0) {
    for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++)
        sendbuf[(i * size) + j] = (*graph)[(j * size) + i];
  }

  MPI_Scatterv(sendbuf.data(), sendcounts.data(), displs.data(),
      MPI_INT, &procgaraph[0], procgaraph.size(), MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < size - 1; i++) {
    for (int k = 0; k < sendcounts_dist[rank]; k++) {
      for (int j = 0; j < size; ++j)
        if ((procgaraph[(k * size) + j] < max_point) && (result[j] < max_point))
          if (distanceproc[k] > result[j] + procgaraph[(k * size) + j])
            distanceproc[k] = result[j] + procgaraph[(k * size) + j];
    }
    MPI_Allgatherv(distanceproc.data(), (step + (rank < size % proc ? 1 : 0)), MPI_INT,
        result.data(), sendcounts_dist.data(), displs_dist.data(), MPI_INT, MPI_COMM_WORLD);
  }
  return result;
}
