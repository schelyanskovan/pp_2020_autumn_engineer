//  Copyright by Oganyan Robert 2020

#ifndef MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_FUNCTIONS_H_
#define MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_FUNCTIONS_H_

#include <functional>
#include <utility>

#define x first
#define y second

using dpair = std::pair<double, double>;
using d_dpair = std::pair<double, dpair>;

constexpr d_dpair NOT_FOUND = {100000000.0, {0, 0}};

extern inline double fun_first(dpair coord);
extern inline dpair grad_first(dpair coord);

extern inline double fun_second(dpair coord);
extern inline dpair grad_second(dpair coord);

extern inline double fun_third(dpair coord);
extern inline dpair grad_third(dpair coord);

inline double fun_forth(dpair coord);
inline dpair grad_forth(dpair coord);

inline double fun_fifth(dpair coord);
inline dpair grad_fifth(dpair coord);

#endif //  MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_FUNCTIONS_H_
