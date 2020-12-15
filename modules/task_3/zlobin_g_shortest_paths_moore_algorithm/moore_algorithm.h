// Copyright 2020 Zlobin George
#ifndef MODULES_TASK_3_ZLOBIN_G_SHORTEST_PATHS_MOORE_ALGORITHM_MOORE_ALGORITHM_H_
#define MODULES_TASK_3_ZLOBIN_G_SHORTEST_PATHS_MOORE_ALGORITHM_MOORE_ALGORITHM_H_

#define MAX_VALUE 20
#define EDGE_PROB 0.5

void fillGraphWithRandomValues(int* graph_to_fill, size_t size = 10);
void getSequentialMooreAlgorithm(int* graph, size_t size, int* shortest_ways, size_t begin_vertex = 0);
void getParallelMooreAlgorithm(int* graph, size_t size, int* shortest_ways, size_t begin_vertex = 0);

#endif  // MODULES_TASK_3_ZLOBIN_G_SHORTEST_PATHS_MOORE_ALGORITHM_MOORE_ALGORITHM_H_
