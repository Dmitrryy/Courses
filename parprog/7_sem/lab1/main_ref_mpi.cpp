#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <mpi.h>

#include <vector>
#include <array>

const int JSIZE = 10000;
const int ISIZE = 10000;
const bool TEST_SEQ = true;

const int DATA_FORWARDING = 1;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank = 0, comm_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    if (rank == 0) {
        std::cout << "Size: " << JSIZE << "x" << ISIZE << std::endl;
    }

    std::vector<std::array<double, JSIZE>> grid(ISIZE);
    //подготовительная часть – заполнение некими данными
    for (size_t i = 0; i < ISIZE; i++)
    {
        for (size_t j = 0; j < JSIZE; j++)
        {
            grid[i][j] = 10 * i + j;
        }
    }

    double start_time = MPI_Wtime();

    int start_number = rank * JSIZE / comm_size;
    int stop_number = (rank + 1) * JSIZE / comm_size - 1;

    for (int line = start_number; line <= stop_number; line++)
    {
        for (int column = 0; column < ISIZE; column++)
            grid[line][column] = sin(2 * grid[line][column]);
    }

    if (rank != 0)
    {
        for (int sending_line = start_number; sending_line <= stop_number; sending_line++)
            MPI_Send(grid[sending_line].data(), ISIZE, MPI_DOUBLE, 0, DATA_FORWARDING, MPI_COMM_WORLD);
    }

    else
    {
        for (int sender_rank = 1; sender_rank < comm_size; sender_rank++)
        {
            int sender_start_number = sender_rank * JSIZE / comm_size;
            int sender_stop_number = (sender_rank + 1) * JSIZE / comm_size - 1;

            for (int receiving_line = sender_start_number; receiving_line <= sender_stop_number; receiving_line++)
                MPI_Recv(grid[receiving_line].data(), ISIZE, MPI_DOUBLE, sender_rank, DATA_FORWARDING, MPI_COMM_WORLD, &status);
        }
    }

    double duration = MPI_Wtime() - start_time;

    if (rank == 0) {
        std::cout << "Time par: " << duration << " sec" <<  std::endl;

        // compare with seq way
        if (TEST_SEQ) {
            std::vector<std::array<double, JSIZE>> grid_seq(ISIZE);

            for (size_t i = 0; i < ISIZE; i++)
            {
                for (size_t j = 0; j < JSIZE; j++)
                {
                    grid_seq[i][j] = 10 * i + j;
                }
            }
            double start_seq = MPI_Wtime();

            for (size_t i = 0; i < ISIZE; i++)
            {
                for (size_t j = 0; j < JSIZE; j++)
                {
                    grid_seq[i][j] = std::sin(2 * grid_seq[i][j]);;
                }
            }
            std::cout << "Time seq: " << MPI_Wtime() - start_seq << " sec" << std::endl;

            size_t loss = 0;
            for (size_t i = 0; i < ISIZE; i++)
            {
                for (size_t j = 0; j < JSIZE; j++)
                {
                    loss += (grid_seq[i][j] != grid[i][j]);
                }
            }
            std::cout << "Loss: " << loss << std::endl;
        }
    }

    MPI_Finalize();

    return 0;
}
