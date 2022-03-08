

#include <mpi.h>


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>

#define D_N 10000


int main(int argc, char **argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

/*    char* cur_end = NULL;
    int num = (int) strtol(argv[1], &cur_end, 10);
    if (errno || *cur_end != 0 || num <= 0) {
        printf("incorrect number of threads: %s, (interpret as: %d(int))\n", argv[1], num);
        exit(2);
    }*/

    // Find out rank, size
    int world_rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    unsigned global_vector_size = D_N;

    unsigned min_vector_size = global_vector_size / world_size;
    unsigned max_vector_size = min_vector_size + ((global_vector_size % world_size) > 0);
    unsigned num_process_with_max_size = global_vector_size % world_size;
    unsigned num_process_with_min_size = world_size - num_process_with_max_size;
    unsigned range_offset = max_vector_size * std::min((int) num_process_with_max_size, world_rank)
                            + min_vector_size * std::max(0, world_rank - (int) num_process_with_max_size);
    printf("rank %d; local_size %d; voffset %u\n", world_rank, max_vector_size, range_offset);
    fflush(0);

    float result = 0;
    std::vector<float> vecBase(min_vector_size + (world_rank < num_process_with_max_size));
    std::iota(vecBase.begin(), vecBase.end(), range_offset);
    std::transform(vecBase.begin(), vecBase.end(), vecBase.begin(), [](float e) { return ((int)e) ? 1 / e : 0; });

    auto local_sum = std::accumulate(vecBase.begin(), vecBase.end(), 0.f);
    float res = 0;
    MPI_Reduce(&local_sum, &res, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        std::cout << "Res: " << res << std::endl;
    }

#if 0
    std::vector<float> vecRes(max_vector_size);
    unsigned current_size = global_vector_size;
    size_t iterations = 0;
    while ((world_size > 1 && current_size >= world_size) || iterations == 0) {
        min_vector_size = current_size / world_size;
        max_vector_size = min_vector_size + ((current_size % world_size) > 0);
        num_process_with_max_size = current_size % world_size;
        num_process_with_min_size = world_size - num_process_with_max_size;
        range_offset = max_vector_size * std::min((int) num_process_with_max_size, world_rank)
                       + min_vector_size * std::max(0, world_rank - (int) num_process_with_max_size);
        if (iterations == 0) { range_offset = 0; }

        if(world_rank == world_size - 1 && current_size % max_vector_size) {
            vecBase.back() = 0;
        }
        MPI_Allreduce(vecBase.data() + range_offset, vecRes.data(), max_vector_size, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

        vecBase.resize(current_size);
        vecRes.resize(current_size);
        std::swap(vecBase, vecRes);
        printf("[%u] rank %d; global_vec_size %u; local_size %d; voffset %u\n", iterations, world_rank, current_size, max_vector_size, range_offset);
        fflush(0);

        current_size = current_size / world_size + ((current_size % world_size) > 0);
        iterations++;
    }
    const auto &vecResult = vecBase;


    if (world_rank == 0) {
        result = std::accumulate(vecResult.cbegin(), vecResult.cend(), 0.f);
        std::cout << "Result: " << result << std::endl;
    }
#endif
    MPI_Finalize();
}
