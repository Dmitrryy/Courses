
#include <errno.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  // Find out rank, size
  int world_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int number = 0;
  MPI_Sendrecv(&world_rank, 1, MPI_INT, (world_rank + 1) % world_size, 0
      , &number, 1, MPI_INT, ((world_rank) ? (world_rank - 1) : (world_size - 1)), 0
      , MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  printf("Process %d rec number %d\n", world_rank, number);
  fflush(0);

  // Finalize the MPI environment.
  MPI_Finalize();
}
