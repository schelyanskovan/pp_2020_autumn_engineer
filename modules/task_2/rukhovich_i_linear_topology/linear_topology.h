// Copyright 2020 Igor Rukhovich
#ifndef MODULES_TASK_2_RUKHOVICH_I_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
#define MODULES_TASK_2_RUKHOVICH_I_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_


#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector, std::string ops);
int getSequentialOperations(std::vector<int> vec, std::string ops);

#endif  // MODULES_TASK_2_RUKHOVICH_I_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
