// Copyright 2020 Igor Rukhovich
#include "../../modules/task_3/rukhovich_i_radix_sort_batcher/radix_sort_batcher.h"
#include <mpi.h>

double RandomDouble::Next() {
    static RandomDouble rand = RandomDouble();
    return rand.dist_(rand.gen_);
}

RandomDouble::RandomDouble() : gen_(std::random_device()()), dist_(-1e3, 1e3) {}

std::vector<double> random_double_array(size_t size) {
    std::vector<double> arr(size);

    for (size_t i = 0; i < size; ++i) {
        arr[i] = RandomDouble::Next();
    }

    return arr;
}

void bitwise_sort_mod(BitsetIt first, BitsetIt last, size_t bit, bool positive = true) {
    auto le = first, ri = last;
    if (le >= ri || le + 1 == ri) return;

    while (le < last && ((*le)[bit] != positive)) ++le;
    if (le == last) {
        if (bit == 0u) return;
        bitwise_sort_mod(first, last, bit - 1u, positive);
        return;
    }

    --ri;
    while (le < ri) {
        while (le < ri && ((*ri)[bit] == positive)) --ri;
        if (le == ri) break;

        std::swap(*le, *ri);
        while (le < ri && ((*le)[bit] != positive)) ++le;
    }

    if (bit == 0u) return;
    bitwise_sort_mod(first, le, bit - 1u, positive);
    bitwise_sort_mod(le, last, bit - 1u, positive);
}

void bitwise_sort(BitsetIt first, BitsetIt last) {
    auto le = first, ri = last;
    if (le >= ri || le + 1 == ri) return;

    while (le < last && (*le)[63u]) ++le;
    if (le == last) {
        bitwise_sort_mod(first, last, 62u, false);
        return;
    }

    --ri;
    while (le < ri) {
        while (le < ri && !(*ri)[63u]) --ri;
        if (le == ri) break;

        std::swap(*le, *ri);
        while (le < ri && (*le)[63u]) ++le;
    }

    bitwise_sort_mod(first, le, 62u, false);
    bitwise_sort_mod(le, last, 62u, true);
}

template<> void radix_sort(DoubleIt first, DoubleIt last) {
    if (first >= last) return;

    std::vector<std::bitset<64>> bits(last - first);
    auto it = first;
    for (size_t i = 0; i < bits.size(); ++it, ++i) {
        bits[i] = reinterpret_cast<uint64_t &>(*it);
    }

    bitwise_sort(bits.begin(), bits.end());

    it = first;
    for (size_t i = 0; i < bits.size(); ++it, ++i) {
        uint64_t num = bits[i].to_ullong();
        *it = reinterpret_cast<double &>(num);
    }
}

template<> void par_radix_sort_batcher(DoubleIt first, DoubleIt last) {

}