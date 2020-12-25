// Copyright 2020 Igor Rukhovich
#include "../../modules/task_2/rukhovich_i_linear_topology/linear_topology.h"
#include <mpi.h>

MPI_Comm make_linear_topology() {
    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    MPI_Comm linear_comm;
    const int dims[1] = { num_proc };
    const int periods[1] = { false };
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, false, &linear_comm);
    return linear_comm;
}