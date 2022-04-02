//
// Created by dadro on 02.04.2022.
//


#include <iostream>
#include <errno.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {

    if (argc != 2) {
        printf("invalid arguments command line:\n argc = %d\n", argc);
        printf("Required 1: [num circuits]");
        printf("We provide:\n");
        for (int i = 0; i < argc; i++)
        {
            printf ("[%d] %s\n", i, argv[i]);
        }
        exit(1);
    }
    char* cur_end1 = nullptr;
    int n = (int) strtol(argv[1], &cur_end1, 10);
    if (errno || *cur_end1 != 0 || n <= 0) {
        std::cout << "incorrect numbers: \n[1] " << argv[1] << " (interpret as: " << n << " (int))\n";
        exit(0);
    }
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Find out rank, size
    int world_rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double startTime = {};
    if (world_rank == 0) {
        startTime = MPI_Wtime();
    }

    int num_sends = world_size * n;
    int number = 0;
    for(size_t i = world_rank; i < num_sends; i += world_size) {
        if (i == 0) {
            MPI_Send(&number, 1, MPI_INT, (i + 1) % world_size, 99, MPI_COMM_WORLD);
        }
        int source = (i) ? (i - 1) % world_size : world_size - 1;
        MPI_Recv(&number, 1, MPI_INT, source, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        number += 1;
        if (i != num_sends) {
            MPI_Send(&number, 1, MPI_INT, (i + 1) % world_size, 99, MPI_COMM_WORLD);
        }
    }

    if (world_rank == 0) {
        auto endTime = MPI_Wtime();
        auto time = (endTime - startTime) * 1000;
        std::cout << "Result 'number': " << number << std::endl;
        std::cout << "Num sends: " << num_sends << std::endl;
        std::cout << "Time: " << (endTime - startTime) * 1000 << std::endl;
        std::cout << "1 send time: " << time / num_sends << std::endl;
    }

    MPI_Finalize();
}
