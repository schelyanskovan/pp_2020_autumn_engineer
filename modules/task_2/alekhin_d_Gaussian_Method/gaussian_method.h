// Copyright 2020 Alekhin Denis
#ifndef MODULES_TASK_2_ALEKHIN_D_GAUSSIAN_METHOD_GAUSSIAN_METHOD_H_
#define MODULES_TASK_2_ALEKHIN_D_GAUSSIAN_METHOD_GAUSSIAN_METHOD_H_

#include <vector>

struct Matrix {
  std::vector<double> matrix;
  int rows;
  int columns;
};


std::vector<double> getSequantialGauss(Matrix matrix);
void initMatrix(Matrix* matrix);
void randomMatrix(Matrix* matrix);
int chooseLeadingLine(Matrix matrix, int column);
void swapLines(Matrix* matrix, int line1, int line2);

std::vector<double> getParallelGauss(Matrix matrix);
void distributeData(Matrix matrix, Matrix* local_matrix);
std::vector<int> numberOfLinesToStore(int global_matrix_rows);
void printMatrix(Matrix local_matrix);
void shareCoeffs(int global_matrix_rows, const Matrix* local_matrix, int column);
int chooseLeadingLineParallel(int global_matrix_rows,
  int global_matrix_culumns, const Matrix* local_matrix, int column);

void getGaussForward(Matrix global_matrix, Matrix* local_matrix);
void swapLinesParallel(Matrix* local_matrix, int line1, int line2);
void gatherMatrix(Matrix* global_matrix, Matrix* local_matrix);
std::vector<double> getGaussBackward(Matrix global_matrix, Matrix* local_matrix);

#endif  // MODULES_TASK_2_ALEKHIN_D_GAUSSIAN_METHOD_GAUSSIAN_METHOD_H_
