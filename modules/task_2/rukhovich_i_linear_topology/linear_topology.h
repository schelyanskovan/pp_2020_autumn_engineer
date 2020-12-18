// Copyright 2020 Igor Rukhovich
#ifndef MODULES_TASK_2_RUKHOVICH_I_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
#define MODULES_TASK_2_RUKHOVICH_I_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_

#include <mpi.h>
#include <random>
#include <vector>

class RandomInt {
 public:
    static int Next();

 private:
    RandomInt();

    std::mt19937_64 gen_;
    std::uniform_int_distribution<int> dist_;
};

std::vector<int> random_integral_array(size_t size);

MPI_Comm linear_topology(int* order);

#endif  // MODULES_TASK_2_RUKHOVICH_I_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
