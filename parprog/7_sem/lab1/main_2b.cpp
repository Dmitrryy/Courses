#include <mpi.h>
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

    for (size_t i = 3; i < ISIZE; i++)
    {
        for (size_t j = 0; j < JSIZE - 2; j++)
        {
            result[i][j] = std::sin(3 * result[i - 3][j + 2]);
        }
    }

    return result;
}

std::vector<std::array<double, JSIZE>> parallel(const std::vector<std::array<double, JSIZE>> &a)
{
    auto result = a;

    for (int i = 3; i < ISIZE; i++)
    {
        #pragma omp parallel for shared(a, result)
        for (int j = 0; j < JSIZE - 2; j++)
        {
            result[i][j] = std::sin(3 * result[i - 3][j + 2]);
        }
    }

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

    if (auto rc = MPI_Init(&argc, &argv))
    {
        std::cout << "Ошибка запуска, выполнение остановлено " << std::endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    int num_processes = 0;
    int proc_id = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    double startTime = 0.f, endTime = 0.f;
    if (proc_id == 0)
    {
        std::cout << "Num processes: " << num_processes << std::endl;
        startTime = MPI_Wtime();
    }

    //=---
    std::vector<std::array<double, JSIZE>> a_par;
    int step = JSIZE / num_processes; // TODO maybe not all covered
    int start = proc_id * step;
    int end = start + step;

    if(proc_id == 0) {
        // left side.
        // recv on the right border.
        a_par = a_seq;
    } else if (proc_id == num_processes - 1) {
        // right side.
        // send left border.
        a_par.resize(step - 2);
        for(int i = 3; i < ISIZE; ++i) {
            for(int j = start; j < end - 2; ++j) {
                //a_par[]
            }
        }
    } else {
        // central.
        // recv right and send left borders.

    }

    //auto a_par = a;

    //=---

    if (proc_id == 0)
    {
        endTime = MPI_Wtime();
        size_t loss = 0;
        for (size_t i = 0; i < ISIZE; i++)
        {
            for (size_t j = 0; j < JSIZE; j++)
            {
                loss += (a_seq[i][j] != a_par[i][j]);
            }
        }
        std::cout << "Par time: " << endTime - startTime << std::endl;
        std::cout << "Loss: " << loss << std::endl;
    }

    MPI_Finalize();

    return 0;
}
