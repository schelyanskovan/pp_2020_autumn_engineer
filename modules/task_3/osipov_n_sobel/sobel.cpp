// Copyright 2020 Osipov Nikolay
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <cmath>
#include "../../../modules/task_3/osipov_n_sobel/sobel.h"


int testM(int sq, int max, int min) {
    if (sq < min) {
        return min;
    } else if (sq > max) {
        return max;
    }
    return sq;
}

int testSobel(std::vector<int> image, int cols, int posrows, int poscols) {
    std::vector<int> vectorx = {
         -1, 0, 1,
         -2, 0, 2,
         -1, 0, 1
    };
    std::vector<int> vectory = {
       -1, -2, -1,
        0,  0,  0,
        1,  2,  1
    };
    int sobel_x = 0, sobel_y = 0, result = 0, count = 0;
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++) {
            sobel_x += image[(j + poscols) + (i + posrows) * cols] * vectorx[count];
            sobel_y += image[(j + poscols) + (i + posrows) * cols] * vectory[count];
            count++;
        }
    result = testM(sqrt(pow(sobel_x, 2) + pow(sobel_y, 2)), 255, 0);
    return result;
}

std::vector<int> sequentialSobel(std::vector<int> image, int rows, int cols) {
    std::vector<int> result_image(cols * rows);
    for (int i = 0; i < cols; i++) {
        result_image[i] = 0;
    }
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 0; j < cols; j++) {
            if (j == cols - 1 || j == 0) {
                result_image[cols * i + j] = 0;
            } else {
                result_image[cols * i + j] = testSobel(image, cols, i, j);
            }
        }
    }
    for (int i = 0; i < cols; i++) {
            result_image[cols * (rows - 1) + i] = 0;
        }
    return result_image;
}

std::vector<int> parallelSobel(std::vector<int> image, int rows, int cols) {
    int size, rank;
    std::vector<int> result_image(rows * cols);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rem = (rows - 1) % size;
    int count = (rows - 1) / size;
    for (int i = 0; i < cols; i++) {
        result_image[i] = 0;
    }
    if (rank == 0) {
        for (int i = 1; i < rem + count; i++) {
            for (int j = 0; j < cols; j++) {
                if (j == cols - 1 || j == 0) {
                    result_image[cols * i + j] = 0;
                } else {
                    result_image[cols * i + j] = testSobel(image, cols, i, j);
                }
            }
        }
        for (int i = 1; i < size; i++) {
            MPI_Send(image.data() + cols * (rem + count * i - 1),
                cols * (count + 2), MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        std::vector<int> tmpimg(cols * (count + 2));
        std::vector<int> tmpsobel(cols * count);
        MPI_Status status;
        MPI_Recv(tmpimg.data(), cols * (count + 2), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < cols; j++) {
                if (j == cols - 1 || j == 0) {
                    tmpsobel[i * cols + j] = 0;
                } else {
                    tmpsobel[i * cols + j] = testSobel(tmpimg, cols, i + 1, j);
                }
            }
        }
        MPI_Send(tmpsobel.data(), cols * count, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Status status;
            MPI_Recv(result_image.data() + cols * (rem + count * i),
                cols * count, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        for (int i = 0; i < cols; i++) {
            result_image[cols * (rows - 1) + i] = 0;
        }
    }
    return result_image;
}
