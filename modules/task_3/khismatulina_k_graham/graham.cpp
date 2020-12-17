// Copyright 2020 Khismatulina Karina
#include <mpi.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <random>
#include "../../../modules/task_3/khismatulina_k_graham/graham.h"

double modul(const Vector& Vec) {
    return (sqrt(Vec.X * Vec.X + Vec.Y * Vec.Y));
}

double skalyar(const Vector& Vec1, const Vector& Vec2) {
    return (Vec1.X * Vec2.X + Vec1.Y * Vec2.Y);
}


double cos(const Vector& A, const Vector& B) {
    return (skalyar(A, B) / (modul(A) * modul(B)));
}

Point SearchMinPoint(const std::vector<Point>& Points) {
    Point min_point = Points[0];
    int i = 0;
    while (i < Points.size())
    {
        min_point = min_point.min(min_point, Points[i]);
        i += 1;
    }
    return min_point;
}

std::vector<Point> GenPoints(int n) {
    if (n < 1)
        throw "slishkom malo!!!";
    std::vector<Point> p(n);
    std::mt19937 gen;
    for (int i = 0; i < n; i++) {
        p[i].x = gen();
        p[i].y = gen();
    }
    return p;
}

std::vector<Point> Sort(const std::vector<Point>& Points) {
    std::vector<Point> P = Points;
    Point min_point = SearchMinPoint(P);
    std::vector<double> Cos(P.size());
    Vector A(1, 0);
    for (int i = 0; i < P.size(); i++) {
        Vector B(min_point, Points[i]);
        if (min_point == Points[i]) {
            Cos[i] = 2;
        } else {
            Cos[i] = cos(A, B);
        }
    }
    for (int i = 0; i < P.size() - 1; ++i) {
        for (int j = 0; j < P.size() - i - 1; ++j) {
            if (Cos[j + 1] > Cos[j]) {
                Point tmp = P[j + 1];
                P[j + 1] = P[j];
                P[j] = tmp;
                double tmp_cos = Cos[j + 1];
                Cos[j + 1] = Cos[j];
                Cos[j] = tmp_cos;
            }
        }
    }
    for (size_t i = 0; i < P.size() - 1; ++i) {
        if (fabs(Cos[i + 1] - Cos[i]) < 0.000001) {
            Vector Vec1(min_point, P[i]);
            Vector Vec2(min_point, P[i + 1]);
            if (modul(Vec1) > modul(Vec2)) {
                Point temp = P[i + 1];
                P[i + 1] = P[i];
                P[i] = temp;
            }
        }
    }
    return P;
}

bool cw(double x1, double y1, double x2, double y2, double x3, double y3) {
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1) < 0;
}

std::vector<Point> GrahamSeq(const std::vector<Point>& P) {
    std::vector<Point> Points = Sort(P);
    if (Points.size() < 2) {
        return Points;
    }
    std::vector<double> Res;
    Res.push_back(Points[0].x);
    Res.push_back(Points[0].y);
    Res.push_back(Points[1].x);
    Res.push_back(Points[1].y);
    if (Points.size() > 2) {
        for (int i = 2; i < Points.size(); ++i) {
            while (cw(Res[Res.size() - 4], Res[Res.size() - 3], Res[Res.size() - 2],
                Res[Res.size() - 1], Points[i].x, Points[i].y)) {
                Res.pop_back();
                Res.pop_back();
            }
            Res.push_back(Points[i].x);
            Res.push_back(Points[i].y);
        }
    }
    std::vector<Point> resPoint(Res.size() / 2);
    for (int i = 0; i < Res.size(); i += 2) {
        resPoint[i / 2].x = Res[i];
        resPoint[i / 2].y = Res[i + 1];
    }
    return resPoint;
}

std::vector<Point> GrahamPar(const std::vector<Point>& Points) {
    std::vector<double> Res;
    std::vector<Point> P = Sort(Points);
    int points_amount = P.size();
    MPI_Status status;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = points_amount / size;
    if (points_amount >= size) {
        std::vector<double> Vec;
        for (int i = 0; i < P.size(); ++i) {
            Vec.push_back(P[i].x);
            Vec.push_back(P[i].y);
        }
        std::vector<double> local_vec(2 * delta);
        MPI_Scatter(&Vec[0], 2 * delta, MPI_DOUBLE, &local_vec[0], 2 * delta, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if (rank == (size - 1)) {
            int i = size * delta;
            while (i < P.size())
            {
                local_vec.push_back(P[i].x);
                local_vec.push_back(P[i].y);
                i += 1;
            }
        }
        std::vector<Point> l_P(local_vec.size() / 2);
        for (int i = 0; i < local_vec.size(); i += 2) {
            l_P[i / 2].x = local_vec[i];
            l_P[i / 2].y = local_vec[i + 1];
        }
        std::vector<Point> local_res = GrahamSeq(l_P);
        local_vec.clear();
        for (size_t i = 0; i < local_res.size(); i++) {
            local_vec.push_back(local_res[i].x);
            local_vec.push_back(local_res[i].y);
        }
        int vec_n = local_vec.size();
        std::vector<int> vec_size(size);
        int N = 0;
        MPI_Gather(&vec_n, 1, MPI_INT, &vec_size[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Reduce(&vec_n, &N, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        std::vector<double> global_vec(N);
        if (rank == 0) {
            std::vector<int> address(size);
            address[0] = 0;
            for (int i = 1; i < size; i++) {
                address[i] = vec_size[i - 1];
            }
            for (int i = 2; i < size; i++) {
                address[i] += address[i - 1];
            }
            for (int i = 0; i < vec_n; i++) {
                global_vec[i] = local_vec[i];
            }
            for (int i = 1; i < size; i++) {
                MPI_Recv(&global_vec[address[i]], vec_size[i], MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            }
        } else {
            MPI_Send(&local_vec[0], vec_n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        }
        if (rank == 0) {
            std::vector<Point> global_res(global_vec.size() / 2);
            int i = 0;
            while (i < global_vec.size())
            {
                global_res[i / 2].x = global_vec[i];
                global_res[i / 2].y = global_vec[i + 1];
                i += 2;
            }
            std::vector<Point> res_point = GrahamSeq(global_res);
            return res_point;
        }
    } else {
        std::vector<Point> res_point = GrahamSeq(P);
        return res_point;
    }
    return P;
}