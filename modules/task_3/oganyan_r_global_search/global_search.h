//  Copyright by Oganyan Robert 2020

#ifndef PP_2020_AUTUMN_ENGINEER_MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
#define PP_2020_AUTUMN_ENGINEER_MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_

#include "../../../modules/task_3/oganyan_r_global_search/functions.h"

std::pair<std::function<double(dpair)>, std::function<dpair(dpair)>> Cur_fun (int num_fun);

//  Старт градиентного спуска
inline dpair GetStart(double x_left, double x_right,
                      double y_left, double y_right);

//  Проверка является ли точка экстремумом
inline bool IsExtremum(const dpair& last, const dpair& cur,
                const std::function<double(dpair)>& func, const double& eps);

//  Вышли ли за пределы
inline bool IsInside(dpair cur, double x_left, double x_right,
                     double y_left, double y_right);

//  Вычисление новой точки
inline dpair Calculate(dpair cur, dpair grad, double step);

//  Вычисление одномерной функции для нахождения шага методом золотого сечения
inline double MakeSimplefx(double x, dpair grad, dpair cur,
                const std::function<double(dpair)>& func);


//  Вычисление нового шага методом золотого сечения
inline double GoldenSelection(double a, double b, double eps, dpair gradient,
                            dpair cur, const std::function<double(dpair)>& func);

double SequentialGlobalSearch(int num_fun,
                double x_left, double x_right, double y_left, double y_right,
                double eps = 1e-5);

double ParallelGlobalSearch(int num_fun,
                double x_left, double x_right, double y_left, double y_right);



#endif //PP_2020_AUTUMN_ENGINEER_MODULES_TASK_3_OGANYAN_R_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
