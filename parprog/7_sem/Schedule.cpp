#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <iomanip>
  
int main(int argc, char* argv[])
{
    char* cur_end = NULL;
    int num = (int) strtol(argv[1], &cur_end, 10);
    if (errno || *cur_end != 0 || num <= 0) {
        printf("incorrect number of threads: %s, (interpret as: %d(int))\n", argv[1], num);
        exit(2);
    }

    double start_time = omp_get_wtime();

    size_t i = 0;
    double result = 0.0;
    std::vector<int> schedule(num, 0);
    std::cout << "Guided 4" << std::endl;
    #pragma omp parallel for reduction(+:result) schedule(guided, 4) 
    for (i = 1; i <= num; ++i) 
    {
        //std::cout << "iter " << i << ", thread " << omp_get_thread_num() << std::endl;
        schedule.at(i-1) = omp_get_thread_num();
        for(size_t j = 1; j < i * i * i; ++j) {
            result += 1.0 / static_cast<double>(j);
        }
    }

    std::cout << std::setw(5) << "Iter";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl << std::setw(5) << "Th";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << schedule.at(i);
    }
    std::cout << std::endl;


    std::cout << "Guided 1" << std::endl;
    #pragma omp parallel for reduction(+:result) schedule(guided, 1) 
    for (i = 1; i <= num; ++i) 
    {
        //std::cout << "iter " << i << ", thread " << omp_get_thread_num() << std::endl;
        schedule.at(i-1) = omp_get_thread_num();
        for(size_t j = 1; j < i * i * i; ++j) {
            result += 1.0 / static_cast<double>(j);
        }
    }

    std::cout << std::setw(5) << "Iter";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl << std::setw(5) << "Th";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << schedule.at(i);
    }
    std::cout << std::endl;


    std::cout << "Dynamic 4" << std::endl;
    #pragma omp parallel for reduction(+:result) schedule(dynamic, 4) 
    for (i = 1; i <= num; ++i) 
    {
        //std::cout << "iter " << i << ", thread " << omp_get_thread_num() << std::endl;
        schedule.at(i-1) = omp_get_thread_num();
        for(size_t j = 1; j < i * i * i; ++j) {
            result += 1.0 / static_cast<double>(j);
        }
    }

    std::cout << std::setw(5) << "Iter";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl << std::setw(5) << "Th";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << schedule.at(i);
    }
    std::cout << std::endl;


    std::cout << "Dynamic 1" << std::endl;
    #pragma omp parallel for reduction(+:result) schedule(dynamic, 1) 
    for (i = 1; i <= num; ++i) 
    {
        //std::cout << "iter " << i << ", thread " << omp_get_thread_num() << std::endl;
        schedule.at(i-1) = omp_get_thread_num();
        for(size_t j = 1; j < i * i * i; ++j) {
            result += 1.0 / static_cast<double>(j);
        }
    }

    std::cout << std::setw(5) << "Iter";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl << std::setw(5) << "Th";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << schedule.at(i);
    }
    std::cout << std::endl;


    std::cout << "Static 4" << std::endl;
    #pragma omp parallel for reduction(+:result) schedule(static, 4) 
    for (i = 1; i <= num; ++i) 
    {
        //std::cout << "iter " << i << ", thread " << omp_get_thread_num() << std::endl;
        schedule.at(i-1) = omp_get_thread_num();
        for(size_t j = 1; j < i * i * i; ++j) {
            result += 1.0 / static_cast<double>(j);
        }
    }

    std::cout << std::setw(5) << "Iter";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl << std::setw(5) << "Th";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << schedule.at(i);
    }
    std::cout << std::endl;


    std::cout << "static 1" << std::endl;
    #pragma omp parallel for reduction(+:result) schedule(static, 1) 
    for (i = 1; i <= num; ++i) 
    {
        //std::cout << "iter " << i << ", thread " << omp_get_thread_num() << std::endl;
        schedule.at(i-1) = omp_get_thread_num();
        for(size_t j = 1; j < i * i * i; ++j) {
            result += 1.0 / static_cast<double>(j);
        }
    }

    std::cout << std::setw(5) << "Iter";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl << std::setw(5) << "Th";
    for(i = 0; i < schedule.size(); ++i) {
        std::cout << std::setw(3) << schedule.at(i);
    }
    std::cout << std::endl;
    double end_time = omp_get_wtime();


    std::cout << "Result: " << result << std::endl;
    std::cout << "Time: " << end_time - start_time << std::endl;

    return 0;
}