// Copyright 2020 Igor Rukhovich
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./linear_topology.h"

TEST(MPILinearTopology, what_topology) {
    MPI_Comm topology = make_linear_topology();

    int stat;
    MPI_Topo_test(topology, &stat);
    ASSERT_EQ(stat, MPI_CART);
    ASSERT_FALSE(stat == MPI_GRAPH);
    ASSERT_FALSE(stat == MPI_UNDEFINED);
}

TEST(MPILinearTopology, send_to_neightbor) {
    MPI_Comm topology = make_linear_topology();
    int rank, num_proc;
    MPI_Comm_rank(topology, &rank);
    MPI_Comm_size(topology, &num_proc);
    int num_elements = 4;

    if (num_proc < 2) {
        return;
    }

    if (rank == 0) {
        int *buff = new int[num_elements];
        for (size_t i = 0; i < static_cast<size_t>(num_elements); ++i) {
            buff[i] = i;
        }
        MPI_Send(buff, 4, MPI_INT, 1, 0, topology);
        delete[] buff;
    }

    if (rank == 1) {
        int *buff = new int[num_elements];
        MPI_Status stat;
        MPI_Recv(buff, 4, MPI_INT, 0, 0, topology, &stat);
        for (size_t i = 0; i < static_cast<size_t>(num_elements); ++i) {
            ASSERT_EQ(i, static_cast<size_t>(buff[i]));
        }
        delete[] buff;
    }
}

TEST(MPILinearTopology, send_to_neightbors) {
    MPI_Comm topology = make_linear_topology();
    int rank, num_proc;
    MPI_Comm_rank(topology, &rank);
    MPI_Comm_size(topology, &num_proc);
    int num_elements = 4;

    if (num_proc < 3) {
        return;
    }
    
    if (rank == 1) {
        int *buff = new int[num_elements];
        for (size_t i = 0; i < static_cast<size_t>(num_elements); ++i) {
            buff[i] = i;
        }
        MPI_Send(buff, 4, MPI_INT, 2, 0, topology);
        MPI_Send(buff, 4, MPI_INT, 0, 0, topology);
        delete[] buff;
    }

    if (rank == 0 || rank == 2) {
        int *buff = new int[num_elements];
        MPI_Status stat;
        MPI_Recv(buff, 4, MPI_INT, 1, 0, topology, &stat);
        for (size_t i = 0; i < static_cast<size_t>(num_elements); ++i) {
            ASSERT_EQ(i, static_cast<size_t>(buff[i]));
        }
        delete[] buff;
    }
}

TEST(MPILinearTopology, send_to_next) {
    MPI_Comm topology = make_linear_topology();
    int rank, num_proc;
    MPI_Comm_rank(topology, &rank);
    MPI_Comm_size(topology, &num_proc);
    int num_elements = 4;

    if (num_proc < 2) {
        return;
    }
    
    if (rank != num_proc - 1) {
        int *buff = new int[num_elements];
        for (size_t i = 0; i < static_cast<size_t>(num_elements); ++i) {
            buff[i] = i + rank;
        }
        MPI_Send(buff, 4, MPI_INT, rank + 1, 0, topology);
        delete[] buff;
    }

    if (rank != 0) {
        int *recv_buff = new int[num_elements];
        MPI_Status stat;
        MPI_Recv(recv_buff, 4, MPI_INT, rank - 1, 0, topology, &stat);
        for (size_t i = 0; i < static_cast<size_t>(num_elements); ++i) {
            ASSERT_EQ(i + rank - 1, static_cast<size_t>(recv_buff[i]));
        }
        delete[] recv_buff;
    }
}

TEST(MPILinearTopology, send_to_prev) {
    MPI_Comm topology = make_linear_topology();
    int rank, num_proc;
    MPI_Comm_rank(topology, &rank);
    MPI_Comm_size(topology, &num_proc);
    int num_elements = 4;

    if (num_proc < 2) {
        return;
    }
    
    if (rank != num_proc - 1) {
        int *buff = new int[num_elements];
        for (size_t i = 0; i < static_cast<size_t>(num_elements); ++i) {
            buff[i] = i + rank;
        }
        MPI_Send(buff, 4, MPI_INT, rank + 1, 0, topology);
        delete[] buff;
    }

    if (rank != 0) {
        int *recv_buff = new int[num_elements];
        MPI_Status stat;
        MPI_Recv(recv_buff, 4, MPI_INT, rank - 1, 0, topology, &stat);
        for (size_t i = 0; i < static_cast<size_t>(num_elements); ++i) {
            ASSERT_EQ(i + rank - 1, static_cast<size_t>(recv_buff[i]));
        }
        delete[] recv_buff;
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
