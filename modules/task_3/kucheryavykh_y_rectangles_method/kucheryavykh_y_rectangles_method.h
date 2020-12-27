// Copyright 2020 Kucheryavykh Yan
#ifndef MODULES_TASK_3_KUCHERYAVYKH_Y_RECTANGLES_METHOD_KUCHERYAVYKH_Y_RECTANGLES_METHOD_H_
#define MODULES_TASK_3_KUCHERYAVYKH_Y_RECTANGLES_METHOD_KUCHERYAVYKH_Y_RECTANGLES_METHOD_H_

#include <mpi.h>
#include <cstring>
#include <cmath>
#include <vector>
#include <iostream>


template <typename Function>
double rectangles_sequential(Function function, std::vector <double> begin_point,
    std::vector <double> end_point, const int fineness) {

    int dimension = begin_point.size();
    std::vector<double> h(dimension);

    for (int i = 0; i < dimension; ++i) {
        h[i] = (end_point[i] - begin_point[i]) / static_cast<double>(fineness);
    }
    std::vector <double> half_point(dimension);
    double result = 0.0;
    int k;
    int sectors = std::pow(fineness, dimension);
    for (int i = 0; i < sectors; ++i) {
        for (k = 0; k < dimension; k++) {
            int a = std::pow(fineness, k+1);
            int b = (fineness * k == 0 ? 1 : std::pow(fineness, k));
            int p = (i % a) / b;
            half_point[k] = begin_point[k] + h[k] * p + h[k] * 0.5;
        }
        result += function(half_point);
    }
    for (int i = 0; i < dimension; ++i) {
        result *= h[i];
    }
    return result;
}

template <typename Function>
double rectangles_parallel(Function function , std::vector <double> begin_point,
    std::vector <double> end_point, const int fineness) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int dimension = begin_point.size();
    int l_start, l_end;
    int sectors = std::pow(fineness, dimension);
    int delta = sectors / size;
    int rem = sectors % size;
    double l_res = 0.0;
    double res = 0.0;

    if (rank < rem) {
        l_start = rank * (delta + 1);
        l_end = rank * (delta + 1) + delta + 1;
    } else {
        l_start = rank * delta + rem;
        l_end = rank * delta + delta + rem;
    }

    std::vector<double> h(dimension);

    for (int i = 0; i < dimension; ++i) {
        h[i] = (end_point[i] - begin_point[i]) / static_cast<double>(fineness);
    }

    std::vector <double> half_point(dimension);
    int k;
    for (int i = l_start; i < l_end; ++i) {
        for (k = 0; k < dimension; k++) {
            int a = std::pow(fineness, k + 1);
            int b = (fineness * k == 0 ? 1 : std::pow(fineness, k));
            int p = (i % a) / b;
            half_point[k] = begin_point[k] + h[k] * p + h[k] * 0.5;
        }
        l_res += function(half_point);
    }
    MPI_Reduce(&l_res, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    for (int i = 0; i < dimension; ++i) {
        res *= h[i];
    }
    return res;
}

#endif  // MODULES_TASK_3_KUCHERYAVYKH_Y_RECTANGLES_METHOD_KUCHERYAVYKH_Y_RECTANGLES_METHOD_H_
