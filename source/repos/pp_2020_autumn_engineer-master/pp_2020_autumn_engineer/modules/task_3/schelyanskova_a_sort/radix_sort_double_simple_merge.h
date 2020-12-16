// Copyright 2020 Schelyanskova Anastasiya

#ifndef MODULES_TASK_3_SHCHELYANSKOVA_N_RADIX_SORT_DOUBLE_SIMPLE_MERGE_H_
#define MODULES_TASK_3_SHCHELYANSKOVA_N_RADIX_SORT_DOUBLE_SIMPLE_MERGE_H_

std::vector<double> getRandomVector(int size);
std::vector<double> checkVector(std::vector<double> *vec);
int countRadix(int num);
double maxVal(vector<double> *vec);
std::vector<double> merge(std::vector<double> *vec1, std::vector<double> *vec2);
std::vector<double> radixSort(std::vector<double> *vec, int radix);
std::vector<double> ParallelRadixSort(std::vector<double> *vec, int size1);
void difOfTime(int size1);
void timeOfRadixSort(int size1);

#endif  // MODULES_TASK_3_SHCHELYANSKOVA_N_RADIX_SORT_DOUBLE_SIMPLE_MERGE_H_
