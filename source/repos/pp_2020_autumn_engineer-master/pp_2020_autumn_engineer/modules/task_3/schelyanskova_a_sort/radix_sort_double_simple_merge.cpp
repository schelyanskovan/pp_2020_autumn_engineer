// Copyright 2020 Schelyanskova Anastasiya

#include <mpi.h>
#include <math.h>
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>
#include "./radix_sort_double_simple_merge.h"

using namespace std;

vector<double> getRandomVector(int size) {
  vector<double> vec(size);
  for (int i = 0; i < size; i++)
  {
    vec[i] = (double)(rand()) / 10000 * (1000 - 10) + 10;
  }
  return vec;
}

vector<double> checkVector(std::vector<double> *vec) {
    double tmp;
    for (vector<double>::iterator i = vec->begin(); i < vec->end(); ++i)
    {
        tmp = *i;
        for (vector<double>::iterator j = i; j < vec->end(); ++j)
        {
            if (*i > *j)
            {
                *i = *j;
                *j = tmp;
            }
        };
    };
    return *vec;
}

int countRadix(int num) {
  int res = 0;
  while (num > 0) {
        num = num / 10;
        res++;
  }
  return res;
}

double maxVal(std::vector<double> * vec) {
    double res;
    for (vector<double>::iterator i = vec->begin(); i < vec->end() - 1; i++) {
        double nextValue = *(i + 1);
        if (*i < nextValue) {
            res = nextValue;
        }
    }
    
    return res;
}

vector<double> merge(vector<double> * vec1, vector<double> * vec2) {
    vector<double> res(vec1->size() + vec2->size());
    vector<double>::iterator i1 = vec1->begin();
    vector<double>::iterator i2 = vec2->begin();
    for (; i1 < vec1->end(), i2 < vec2->end(); ++i1, ++i2) {
        res.push_back(*i1 < *i2 ? *i1 : *i2);
    }
    
    for (; i1 < vec1->end(); i1++) {
        res.push_back(*i1);
    }
    for (; i2 < vec2->end(); i2++) {
        res.push_back(*i2);
    }
    
    return res;
}

vector<double> radixSort(vector<double> * vec, int radix) {
  vector<double> counts1(10, 0);
  vector<double> counts2(10, 0);
  vector<double> res(vec->size());
  int size = vec->size();
  for (vector<double>::iterator i = vec->begin(); i < vec->end(); i++)
  { //86 88-91 105-107
      counts1[((*i % static_cast<double>(pow(10, (radix)))) / pow(10, (radix - 1)))]++;
  }
  for (int i = 0; i < size; i++)
  {
      counts1[((vec[i] % static_cast<double>(pow(10, (radix)))) / pow(10, (radix - 1)))]++;
  }
      
  for (int i = 1; i < 10; i++)
  {
      for (int j = i - 1; j >= 0; j--)
      {
          counts1[i] += counts1[j];
      }
  }

  double tmp = 0;
  for (vector<double>::iterator i = vec->begin(); i < vec->end(); i++)
  {
      tmp = *i;
      //int index = (tmp % static_cast<double>(pow(10, (radix)))) / pow(10, (radix - 1));
      //res[counts2[index]] = tmp;
      //counts2[index]++;
  }
      
  return res;
}

vector<double> ParallelRadixSort(vector<double> * vec, int size1) {
  int radix = countRadix(maxVal(vec));
  int size, rank;
  int size_vec = size1;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int delta = size_vec / size;

  MPI_Barrier(MPI_COMM_WORLD);

  vector<double> local_vec(delta);
  if (rank == 0) 
  {
     for (int i = 1; i < size; i++) 
     {
         if ((i == (size - 1))&&((size_vec % size) != 0)) 
         {
            int delta_for_last = size_vec - (size - 1) * delta;
            MPI_Send(&vec[0] + i * delta, delta_for_last, MPI_INT, i, 0, MPI_COMM_WORLD);
         } else 
         {
            MPI_Send(&vec[0] + i * delta, delta, MPI_INT, i, 0, MPI_COMM_WORLD);
         }
     }
  }
  if (rank == 0)
  {
     local_vec = vector<double>(vec->begin(), vec->begin() + delta);
  } else 
  {
      MPI_Status status;
      if ((rank == (size - 1))&&((size_vec % size) != 0))
      {
         delta = size_vec - (size - 1)*delta;
         local_vec.resize(delta);
      }
      MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
  {
     for (int i = 1; i < size; i++)
     {
        MPI_Send(&radix, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
     }
  } else 
  {
    MPI_Status status;
    MPI_Recv(&radix, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  for (int i = 1; i <= radix; i++) 
  {
      local_vec = radixSort(&local_vec, i);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  int index = (rank + size) % size + 1;

  MPI_Barrier(MPI_COMM_WORLD);

  vector<double> res(0);
  MPI_Status status;
  if (rank == 0) 
  {
     int size_recive_vec1 = 0;
     int size_recive_vec2 = 0;
     vector<double> recive_vec1;
     vector<double> recive_vec2;
     if (2 * index <= size) 
     {
        MPI_Recv(&size_recive_vec1, 1, MPI_INT, ((index * 2 - 1) % size), 0, MPI_COMM_WORLD, &status);
        recive_vec1.resize(size_recive_vec1);
        MPI_Recv(&recive_vec1[0], size_recive_vec1, MPI_INT, ((index * 2 - 1) % size), 0, MPI_COMM_WORLD, &status);
     }
     if (2 * index + 1  <= size) 
     {
        MPI_Recv(&size_recive_vec2, 1, MPI_INT, ((index * 2) % size), 0, MPI_COMM_WORLD, &status);
        recive_vec2.resize(size_recive_vec2);
        MPI_Recv(&recive_vec2[0], size_recive_vec2, MPI_INT, ((index * 2) % size), 0, MPI_COMM_WORLD, &status);
     }
     if (!recive_vec1.empty() && !recive_vec2.empty()) 
     {
        res = merge(&recive_vec1, &recive_vec2);
        res = merge(&res, &local_vec);
     } else if (!recive_vec1.empty()) 
     {
         res = merge(&recive_vec1, &local_vec);
     } else if (!recive_vec2.empty()) 
     {
         res = merge(&recive_vec2, &local_vec);
     } else 
     {
         res = local_vec;
     }
  } 
  else 
  {
    vector<double> recive_vec1;
    vector<double> recive_vec2;
    int size_recive_vec1 = 0;
    int size_recive_vec2 = 0;
    if (2 * index <= size) 
    {
       MPI_Recv(&size_recive_vec1, 1, MPI_INT, ((index * 2 - 1) % size), 0, MPI_COMM_WORLD, &status);
       recive_vec1.resize(size_recive_vec1);
       MPI_Recv(&recive_vec1[0], size_recive_vec1, MPI_INT, ((index * 2 - 1) % size), 0, MPI_COMM_WORLD, &status);
    }
    if (2 * index + 1 <= size) 
    {
       MPI_Recv(&size_recive_vec2, 1, MPI_INT, ((index * 2) % size), 0, MPI_COMM_WORLD, &status);
       recive_vec2.resize(size_recive_vec2);
       MPI_Recv(&recive_vec2[0], size_recive_vec2, MPI_INT, ((index * 2) % size), 0, MPI_COMM_WORLD, &status);
    }
    vector<double> send_vec;
    if (!recive_vec1.empty() && !recive_vec2.empty()) 
    {
     send_vec = merge(&recive_vec1, &recive_vec2);
     send_vec = merge(&send_vec, &local_vec);
    } else if (!recive_vec1.empty()) 
    {
        send_vec = merge(&recive_vec1, &local_vec);
    } else if (!recive_vec2.empty()) 
    {
        send_vec = merge(&recive_vec2, &local_vec);
    } else 
    {
        send_vec = local_vec;
    }
      int size_cur_vec = send_vec.size();
      MPI_Send(&size_cur_vec, 1, MPI_INT, ((index / 2 - 1) % size), 0, MPI_COMM_WORLD);
      MPI_Send(&send_vec[0], send_vec.size(), MPI_INT, ((index / 2 - 1) % size), 0, MPI_COMM_WORLD);
  }
   
  MPI_Barrier(MPI_COMM_WORLD);

  return res;
}

void difOfTime(int size1) {

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double timeRadixSort1, timeRadixSort2, timeSort1, timeSort2;

  vector<double> vec;
  if (rank == 0) 
  {
      vec = getRandomVector(size1);
  }

  if (rank == 0)
  {
      timeRadixSort1 = MPI_Wtime();
  }
     
  ParallelRadixSort(&vec, size1);
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
  {
      timeRadixSort2 = MPI_Wtime();
  }
     
  if (rank == 0) 
  {
     timeSort1 = MPI_Wtime();
     int radix = countRadix(maxVal(&vec));
     for (int i = 1; i <= radix; i++)
     {
         vec = radixSort(&vec, radix);
     }

     timeSort2 = MPI_Wtime();
     double resRadixSort = timeRadixSort2 - timeRadixSort1;
     double resSort = timeSort2 - timeSort1;
     if (resSort < resRadixSort) 
     {
        cout << "SimpleSort faster than ParallelRadixSort on " << resRadixSort - resSort << endl;
     } else 
     {
        cout << "ParallelRadixSort faster than SimpleSort on " << resSort - resRadixSort << endl;
     }
  }
}

void timeOfRadixSort(int size1) {

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double timeRadixSort1, timeRadixSort2;

  vector<double> vec;
  if (rank == 0)
  {
      vec = getRandomVector(size1);
      timeRadixSort1 = MPI_Wtime();
  }

  MPI_Barrier(MPI_COMM_WORLD);
  ParallelRadixSort(&vec, size1);
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) 
  {
      timeRadixSort2 = MPI_Wtime();
  }
     
  if (rank == 0) 
  {
     double resRadixSort = timeRadixSort2 - timeRadixSort1;
     cout << "time is  " << resRadixSort << endl;
  }
}
