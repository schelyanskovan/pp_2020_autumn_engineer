//  Copyright by Oganyan Robert 2020
#include <vector>
#include "../../../modules/task_3/oganyan_r_global_search/global_search.h"

d_dpair SequentialGlobalSearch(const std::function<double(double)>& func, double x_left, double x_right,
                               double y_left, double y_right, double step, double eps) {

}

d_dpair ParallelGlobalSearch(const std::function<double(double)>& func, double x_left, double x_right,
                             double y_left, double y_right, double step, double eps) {

}

inline std::vector<int> GradientDescent(const std::function<double(double)>& func);

inline dpair RandomStart(double x_left, double x_right,
                         double y_left, double y_right);

inline bool IsExtremum(double last, double cur);

//inline StepChange()