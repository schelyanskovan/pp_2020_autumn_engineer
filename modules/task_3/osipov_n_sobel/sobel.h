// Copyright 2020 Osipov Nikolay
#ifndef MODULES_TASK_3_OSIPOV_N_SOBEL_SOBEL_H_
#define MODULES_TASK_3_OSIPOV_N_SOBEL_SOBEL_H_

#include <vector>

int testM(int sq, int max, int min);
int testSobel(std::vector<int> image, int cols, int posrows, int poscols);
std::vector<int> sequentialSobel(std::vector<int> image, int  rows, int cols);
std::vector<int> parallelSobel(std::vector<int> image, int rows, int cols);

#endif  // MODULES_TASK_3_OSIPOV_N_SOBEL_SOBEL_H_
