//  Copyright by Oganyan Robert 2020

#include <cmath>
#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>
#include "../../../modules/task_3/oganyan_r_global_search/global_search.h"

std::pair<std::function<double(dpair)>, std::function<dpair(dpair)>> Cur_fun (int num_fun) {
    switch (num_fun) {
        case 1:
            return {std::function<double(dpair)>(fun_first),std::function<dpair(dpair)>(grad_first)};
        case 2:
            return {std::function<double(dpair)>(fun_second),std::function<dpair(dpair)>(grad_second)};
        case 3:
            return {std::function<double(dpair)>(fun_third),std::function<dpair(dpair)>(grad_third)};
        case 4:
            return {std::function<double(dpair)>(fun_forth),std::function<dpair(dpair)>(grad_forth)};
        case 5:
            break;
        default:
            return {std::function<double(dpair)>(fun_first),std::function<dpair(dpair)>(grad_first)};
    }

}

inline dpair GetStart(double x_left, double x_right,
                      double y_left, double y_right) {
    dpair pos;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(x_left, x_right);
    pos.x = dis(gen);
    dis = std::uniform_real_distribution<>(y_left, y_right);
    pos.y = dis(gen);
//    std::cout<<"random start: "<<std::endl;
//    std::cout<<pos.x<< " "<<pos.y<<std::endl;
    return pos;
}

inline bool IsExtremum(const dpair& last, const dpair& cur, const std::function<double(dpair)>& func, const double& eps) {
    return std::abs(func(cur) - func(last)) < eps;
}

inline bool IsInside(dpair cur, double x_left, double x_right,
                     double y_left, double y_right) {
    return !(cur.x < x_left || cur.x < x_right || cur.y < y_left || cur.y >y_right);
}

inline dpair Calculate(dpair cur, dpair grad, double step) {
    dpair new_cur;
    new_cur.x = cur.x - step * grad.x;
    new_cur.y = cur.y - step * grad.y;
    return new_cur;
}

inline double MakeSimplefx(double x, dpair grad, dpair cur, const std::function<double(dpair)>& func)
{
    dpair buffer;

    buffer.x = cur.x-x*grad.x;
    buffer.y = cur.y-x*grad.y;

    return func(buffer);
}

inline double GoldenSelection(double a, double b, double eps, dpair gradient, dpair cur, const std::function<double(dpair)>& func) {

    const double fi=1.6180339887;
    double x1,x2;
    double y1,y2;

    x1=b-((b-a)/fi);
    x2=a+((b-a)/fi);
    y1=MakeSimplefx(x1,gradient, cur, func);
    y2=MakeSimplefx(x2,gradient, cur, func);
    while (std::abs(b-a)>eps)
    {
        if (y1<=y2)
        {
            b=x2;
            x2=x1;
            x1=b-((b-a)/fi);
            y2=y1;
            y1=MakeSimplefx(x1,gradient, cur, func);
        }
        else
        {
            a=x1;
            x1=x2;
            x2=a+((b-a)/fi);
            y1=y2;
            y2=MakeSimplefx(x2,gradient, cur, func);
        }
    }

    return (a+b)/2;
}

double SequentialGlobalSearch(int fun_num,
                               double x_left, double x_right, double y_left, double y_right,
                               double eps) {
    auto cur_pair = Cur_fun(fun_num);
    auto func = cur_pair.x;
    auto grad = cur_pair.y;
    dpair cur_pos { GetStart(x_left, x_right, y_left, y_right)};
    dpair last_pos { cur_pos };
    bool out_of_borders { false};
    bool extremum_done { false };

    do {
        last_pos = cur_pos;
        auto gradient_vec { grad(cur_pos) };
        double new_step { GoldenSelection(0,0.1,eps,gradient_vec,cur_pos, func)};
        cur_pos = Calculate(cur_pos, gradient_vec, new_step);

        out_of_borders = IsInside(cur_pos, x_left, x_right, y_left, y_right);
        if (out_of_borders) break;

        extremum_done = IsExtremum(last_pos, cur_pos, func, eps);
        if (extremum_done) break;

    } while (true);

    if (out_of_borders || !extremum_done) {
        return 1e9;
    }

    return func(cur_pos);

}

double ParallelGlobalSearch(int fun_num,
    double x_left, double x_right, double y_left, double y_right) {

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double global_min = 1e9;


    //  Заполнение вектора для отправки данных
    std::vector<double> to_send;
    if (rank == 0) {
        double distx = x_right - x_left;
        double disty = y_right - y_left;
        double deltax = distx / 4;
        double deltay = disty / size;
        size_t size_vec = 4 * size * 4;
        to_send.resize(size_vec);

        double cur_x = x_left;
        double cur_y = x_right;
        for (size_t i = 0; i < size_vec / 4; ++i) {
            to_send[4 * i] = cur_x;
            to_send[4 * i + 1] = cur_y;
            cur_x += deltax;
            to_send[4 * i + 2] = cur_x;
            to_send[4 * i + 3] = cur_y + deltay;
            if (i % 16 == 0) {
                cur_x = x_left;
                cur_y += deltay;
            }
        }
    }

    if (rank == 0) {
        // Отпрвка данных
        int cur_step = 0;
        for (int i = 0; i < rank - 1; ++i) {
            MPI_Send(&to_send[0] + cur_step, 16, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            cur_step += 16;
        }
    }

    std::vector<double> proc_data(16);
    if (rank == 0) {
        size_t j = 0;
        size_t start = 4 * 4 * size - 16;
        for (size_t i = start; i < start + 16; ++i) {
            proc_data[j++] = to_send[i];
        }
    }
    else {
        MPI_Recv(&proc_data, 16, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, nullptr);
    }

    double local_min = 1e9;
    int cnt = 0;
    while (cnt++ != 4) {
        local_min = std::min(local_min, SequentialGlobalSearch(fun_num,
            proc_data[cnt], proc_data[cnt + 2], proc_data[cnt+1], proc_data[cnt+3]));
    }
    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    return global_min;

}



//inline StepChange()