//  Copyright by Oganyan Robert 2020

#include <cmath>
#include "../../../modules/task_3/oganyan_r_global_search/functions.h"

inline double fun_first(double x, double y) {
    return x * x + y * y;
}

inline double fun_second(double x, double y) {
    return 3*x*x + x*y + 2*y*y -x -4*y;
}

inline double fun_third(double x, double y) {
    return  -exp(x+y) * (x*x - 2*y*y);
}

inline double fun_forth(double x, double y) {
    return x * x * x - 6*x*y + 8*y * y * y + 5;
}

