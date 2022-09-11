#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
  
int main(int argc, char* argv[])
{
    #pragma omp parallel
    {
        std::cout << "[0] ID " << omp_get_thread_num() 
        << ", num threads " << omp_get_num_threads() << std::endl;

        #pragma omp parallel
        {
            std::cout << "[1] ID " << omp_get_thread_num() 
            << ", num threads " << omp_get_num_threads() << std::endl;

            #pragma omp parallel
            {
                std::cout << "[2] ID " << omp_get_thread_num() 
                << ", num threads " << omp_get_num_threads() << std::endl;
            }
        }
    }
}