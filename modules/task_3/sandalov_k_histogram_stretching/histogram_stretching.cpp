// Copyright 2020 Konstantin Sandalov

#include "../../modules/task_3/sandalov_k_histogram_stretching/histogram_stretching.hpp"
#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>


void createRandomImage(std::vector<int>* randImg, int width, int height) {
    const int min = 0;
    const int max = 255;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distrib(min, max);
    int size = width * height;
    randImg->resize(size);
    for (auto& pixel : *randImg) {
        pixel = static_cast<int>(distrib(generator));
    }
}

int histogramStretching(std::vector<int>* targetImg, int minVal, int maxVal) {
    if ((minVal == 0 && maxVal == 255) || targetImg == nullptr) return -1;
    //std::cout << '\t' << minVal << '\t' << maxVal << '\n';
    float b = 255. / (maxVal - minVal);
    float a = b * (minVal);
    //std::cout << '\t' << a << '\t' << b << '\n';
    for (auto& pxlVal : *targetImg) {
        pxlVal = pxlVal * b - a;
    }
    return 1;
}

int countMinMax(std::vector<int>* targetImg, int* minVal, int* maxVal) {
    if (minVal == nullptr || maxVal == nullptr || targetImg == nullptr) return -1;
    int min = 256;
    int max = -1;
    for(const auto& pxlVal : *targetImg) {
        if (pxlVal < min) min = pxlVal;
        if (pxlVal > max) max = pxlVal;
    }
    if (min < 0 || min > 255 || max < 0 || max > 255) return -2;
    *minVal = min;
    *maxVal = max;
    return 1;
}

int seqHistogramStretching(std::vector<int>* targetImg) {
    if (targetImg == nullptr) return -1;
    int min = 0, max = 0;
    int countMinMaxRetValue = countMinMax(targetImg, &min, &max);
    if (countMinMaxRetValue != 1) return -1;
    int histStretchingRetValue = histogramStretching(targetImg, min, max);
    if (histStretchingRetValue != 1) return -1;
    return 1;
}

void compareMaxMin(void* in, void* inOut, int* len, MPI_Datatype *dtype) {
    int* minMaxIn = static_cast<int*>(in);
    int* minMaxOut = static_cast<int*>(inOut);

    if (minMaxIn[0] < minMaxOut[0]) minMaxOut[0] = minMaxIn[0];
    if (minMaxIn[1] > minMaxOut[1]) minMaxOut[1] = minMaxIn[1];
}

int parallelHistogramStretching(std::vector<int>* targetImg) {
    int procSize, procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);

    if (procSize == 1) {
        int seqHistStrRes = seqHistogramStretching(targetImg);
        if (seqHistStrRes != 1) return -1;
        return 1;
    }

    size_t imageSize = 0;
    if (procRank == 0) {
        if (targetImg != nullptr) imageSize = targetImg->size();
    }

    MPI_Bcast(&imageSize, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
    if (imageSize == 0) return -1;

    MPI_Group myGroup;
    if (imageSize < static_cast<unsigned long>(procSize)) {
        MPI_Group worldGroup;
        MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
        if (imageSize > procSize - imageSize) {
            int* ranks = new int[procSize - imageSize];
            for (int i = imageSize; i < procSize; ++i) {
                ranks[i - imageSize] = i;
            }
            MPI_Group_excl(worldGroup, procSize - imageSize, ranks, &myGroup);
            delete[] ranks;
        } else {
            int* ranks = new int[imageSize];
            for (size_t i = 0; i < imageSize; ++i) {
                ranks[i] = i;
            }
            MPI_Group_incl(worldGroup, imageSize, ranks, &myGroup);
            delete[] ranks;
        }
        MPI_Group_free(&worldGroup);
    } else {
        MPI_Comm_group(MPI_COMM_WORLD, &myGroup);
    }

    procSize = std::min(imageSize, static_cast<unsigned long>(procSize));

    MPI_Comm MY_COMM;
    MPI_Comm_create(MPI_COMM_WORLD, myGroup, &MY_COMM);
    MPI_Group_free(&myGroup);
    if (procRank < procSize) {
        int size = imageSize / procSize;
        int add = imageSize % procSize;

        int* sendCounts = new int[procSize];
        int* displs = new int[procSize];
        int displ = 0;
        std::vector<int> localImg;
        for (int i = 0; i < procSize; ++i) {
            sendCounts[i] = size;
            if (i < add) ++sendCounts[i];
            displs[i] = displ;
            displ+= sendCounts[i];
        }
        localImg.resize(sendCounts[procRank]);

        int* ptr;
        if (targetImg == nullptr) ptr = nullptr;
        else ptr = targetImg->data();
        MPI_Scatterv(ptr, sendCounts, displs, MPI_INT, localImg.data(), sendCounts[procRank], MPI_INT, 0, MY_COMM);

        int localMinMax[] = {0, 0};
        countMinMax(&localImg, localMinMax, localMinMax + 1);
        int globalMinMax[] = {-1, -1};
        MPI_Op op;
        MPI_Op_create(compareMaxMin, true, &op);
        MPI_Reduce(localMinMax, globalMinMax, 2, MPI_INT, op, 0, MY_COMM);
        MPI_Bcast(globalMinMax, 2, MPI_INT, 0, MY_COMM);

        histogramStretching(&localImg, globalMinMax[0], globalMinMax[1]);
        MPI_Gatherv(localImg.data(), sendCounts[procRank], MPI_INT, ptr, sendCounts, displs, MPI_INT, 0, MY_COMM);
        delete[] sendCounts;
        delete[] displs;
        MPI_Comm_free(&MY_COMM);
    }
    return 1;
}