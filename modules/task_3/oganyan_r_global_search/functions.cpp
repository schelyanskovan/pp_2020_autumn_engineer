//  Copyright by Oganyan Robert 2020

#include <cmath>
#include "../../../modules/task_3/oganyan_r_global_search/functions.h"

extern inline double fun_first(dpair coord) {
    return coord.x * coord.x + coord.y * coord.y;
}

extern inline dpair grad_first(dpair coord) {
    return { 2 * coord.x, 2 * coord.y};
}

extern inline double fun_second(dpair coord) {
    return 3*coord.x*coord.x + coord.x*coord.y + 2*coord.y*coord.y -coord.x -4*coord.y;
}

extern inline dpair grad_second(dpair coord) {
    return { 6 * coord.x + coord.y - 1, 2 * coord.y};
}

extern inline double fun_third(dpair coord) {
    return  -exp(coord.x+coord.y) * (coord.x*coord.x - 2*coord.y*coord.y);
}

extern inline double fun_forth(dpair coord) {
    return coord.x * coord.x * coord.x - 6*coord.x*coord.y + 8*coord.y * coord.y * coord.y + 5;
}


