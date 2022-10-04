#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <iostream>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#define ISIZE 10000
#define JSIZE 10000

std::vector<std::array<double, JSIZE>> sequential(const std::vector<std::array<double, JSIZE>> &a)
{
    auto result = a;
    double start_time = omp_get_wtime();

    for (size_t i = 3; i < ISIZE; i++)
    {
        for (size_t j = 0; j < JSIZE - 2; j++)
        {
            result[i][j] = std::sin(3 * result[i - 3][j + 2]);
        }
    }
    double end_time = omp_get_wtime();
    std::cout << "Sequential time: " << end_time - start_time << std::endl;

    return result;
}

std::vector<std::array<double, JSIZE>> parallel(const std::vector<std::array<double, JSIZE>> &a)
{
    auto result = a;
    double start_time = omp_get_wtime();

    for (int i = 3; i < ISIZE; i++)
    {
#pragma omp parallel for shared(a, result)
        for (int j = 0; j < JSIZE - 2; j++)
        {
            result[i][j] = std::sin(3 * result[i - 3][j + 2]);
        }
    }
    double end_time = omp_get_wtime();
    std::cout << "Sequential time: " << end_time - start_time << std::endl;

    return result;
}

int main(int argc, char **argv)
{
    std::vector<std::array<double, JSIZE>> a(ISIZE);
    for (size_t i = 0; i < ISIZE; i++)
    {
        for (size_t j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }

    auto a_seq = sequential(a);
    auto a_par = parallel(a);

    size_t loss = 0;
    for (size_t i = 0; i < ISIZE; i++)
    {
        for (size_t j = 0; j < JSIZE; j++)
        {
            loss += (a_seq[i][j] != a_par[i][j]);
        }
    }
    std::cout << "Loss: " << loss << std::endl;

    return 0;
}
