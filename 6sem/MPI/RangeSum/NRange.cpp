
#include <errno.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


#define D_N 8


int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  // Find out rank, size
  int world_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  unsigned range_offset = (int)(D_N / world_size) * world_rank;
  unsigned vector_size = (int)(D_N / world_size) + (world_rank < (D_N % world_size)); //TODO

  printf("rank %d; vsize %u; voffset %u\n", world_rank, vector_size, range_offset);
  int *vector = (int *)calloc(sizeof(int), vector_size + 1);
  int *vectorResult = NULL;
  if(world_rank == 0)
    vectorResult = (int *)calloc(sizeof(int), vector_size + 1);

  for(unsigned i = 0; i < vector_size; ++i) {
    vector[i] = /*1 /*/ (range_offset + i + 1);
  }

  MPI_Reduce(vector
            , vectorResult
            , vector_size
            , MPI_INT
            , MPI_SUM
            , 0
            , MPI_COMM_WORLD);

  if(world_rank == 0) {
    for(unsigned i = 0; i < vector_size; ++i)
      printf("%d\n", vectorResult[i]);
  }

  // Finalize the MPI environment.
  MPI_Finalize();
}
