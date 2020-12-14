// Copyright 2020 Igor Rukhovich
#ifndef MODULES_TASK_3_RUKHOVICH_I_RADIX_SORT_BATCHER_RADIX_SORT_BATCHER_H_
#define MODULES_TASK_3_RUKHOVICH_I_RADIX_SORT_BATCHER_RADIX_SORT_BATCHER_H_
#include <bitset>
#include <random>
#include <vector>

class RandomDouble {
public:
    static double Next();

private:
    RandomDouble();
    
    std::mt19937_64 gen_;
    std::uniform_real_distribution<double> dist_;
};

std::vector<double> random_double_array(size_t size);


typedef std::vector<double>::iterator DoubleIt;
typedef std::vector<std::bitset<64>>::iterator BitsetIt;

void bitwise_sort_mod(BitsetIt first, BitsetIt last, size_t bit, bool positive);
void bitwise_sort(BitsetIt first, BitsetIt last);

template <class RandomIt>
void radix_sort(RandomIt first, RandomIt last);

template <class RandomIt>
void par_radix_sort_batcher(RandomIt first, RandomIt last);

#endif  // MODULES_TASK_3_RUKHOVICH_I_RADIX_SORT_BATCHER_RADIX_SORT_BATCHER_H_
