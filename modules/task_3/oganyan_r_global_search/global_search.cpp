//  Copyright by Oganyan Robert 2020
#include <vector>
#include <random>
#include <iostream>
#include "../../../modules/task_3/oganyan_r_global_search/global_search.h"

inline std::vector<int> GradientDescent(const std::function<double(double)>& func);

/*
inline dpair GetStart(double x_left, double x_right,
                      double y_left, double y_right) {
    return {(x_left + x_right) / 2.0, (y_left + y_right) / 2.0};
}
*/

inline dpair GetStart(double x_left, double x_right,
                      double y_left, double y_right) {
    dpair pos;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(x_left, x_right);
    pos.x = dis(gen);
    dis = std::uniform_real_distribution<>(y_left, y_right);
    pos.y = dis(gen);
    std::cout<<"random start: "<<std::endl;
    std::cout<<pos.x<< " "<<pos.y<<std::endl;
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

d_dpair SequentialGlobalSearch(const std::function<double(dpair)>& func, const std::function<dpair(dpair)>& grad,
                               double x_left, double x_right, double y_left, double y_right,
                               double step, double eps) {

    dpair cur_pos { GetStart(x_left, x_right, y_left, y_right)};
    dpair last_pos { cur_pos };
    bool out_of_borders { false};
    bool extremum_done { false };

    do {
        last_pos = cur_pos;
        auto gradient_vec { grad(cur_pos) };
        double new_step { GoldenSelection(0,0.05,eps,gradient_vec,cur_pos, func)};
        cur_pos = Calculate(cur_pos, gradient_vec, new_step);

        out_of_borders = IsInside(cur_pos, x_left, x_right, y_left, y_right);
        if (out_of_borders) break;

        extremum_done = IsExtremum(last_pos, cur_pos, func, eps);
        if (extremum_done) break;

    } while (true);

    if (out_of_borders || !extremum_done) {
        return NOT_FOUND;
    }

    return {func(cur_pos),cur_pos};

}

d_dpair ParallelGlobalSearch(const std::function<double(dpair)>& func, const std::function<dpair(dpair)>& grad,
    double x_left, double x_right, double y_left, double y_right,
    double step, double eps) {
    return {0, {0,0}};

}



//inline StepChange()