//  Copyright by Oganyan Robert 2020

#ifndef PP_2020_AUTUMN_ENGINEER_MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
#define PP_2020_AUTUMN_ENGINEER_MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_

#include <functional>
#include <utility>

using dpair = std::pair<double, double>;
using d_dpair = std::pair<double, dpair>;

d_dpair SequentialGlobalSearch(const std::function<double(double)>& func, double x_left, double x_right,
                    double y_left, double y_right, double step = 0.25, double eps = 1e-4);

d_dpair ParallelGlobalSearch(const std::function<double(double)>& func, double x_left, double x_right,
                           double y_left, double y_right, double step = 0.25, double eps = 1e-4);

inline std::vector<int> GradientDescent(const std::function<double(double)>& func);

inline dpair RandomStart(double x_left, double x_right,
                         double y_left, double y_right);

inline bool IsExtremum(double last, double cur);

//inline StepChange()

#endif //PP_2020_AUTUMN_ENGINEER_MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
