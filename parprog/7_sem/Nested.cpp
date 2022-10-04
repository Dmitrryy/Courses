#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
  
int main(int argc, char* argv[])
{
    omp_set_nested(true);
    #pragma omp parallel num_threads(4)
    {
        #pragma omp critical
        std::cout << "[0] ID " << omp_get_thread_num() 
        << ", num threads " << omp_get_num_threads() << std::endl;
        #pragma omp parallel num_threads(2)
        {
            #pragma omp critical
            std::cout << "[1] ID " << omp_get_thread_num() 
            << ", num threads " << omp_get_num_threads() << std::endl;

            #pragma omp parallel num_threads(6)
            {
                #pragma omp critical
                std::cout << "[2] ID " << omp_get_thread_num() 
                << ", num threads " << omp_get_num_threads() << std::endl;
            }
        }
    }
}