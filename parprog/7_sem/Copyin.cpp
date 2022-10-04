#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>

float tol;

int main(int argc, char* argv[])
{
    #pragma omp threadprivate(tol)

    int nthreads, tid;

    tol = 10.0;
    #pragma omp parallel
    {
        tid = omp_get_thread_num();
        printf("Hello from thread = %d, tol = %f\n",
               tid, tol);
  
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n",
                   nthreads);
        }
    }

    return 0;
}