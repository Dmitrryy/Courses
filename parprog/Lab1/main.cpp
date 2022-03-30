//
// Created by dadro on 29.03.2022.
//
#include <iostream>
#include <vector>

#include <mpi.h>

template<typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T> &v) {
    for(auto&& elem: v) {
        out << elem << ' ';
    }
    return out;
}

int main(int argc, char *argv[]) {

    const float c = 1.f;
    auto&& F = [](float t, float x) -> float {
        return 0.f;
    };
    auto&& fi = [](float x) -> float {
        return x > 2;
    };
    auto&& ksi = [](float t) -> float {
        return 0;
    };
    const float t_max = 7;
    const float x_max = 8;
    const float tau = 1.f;
    const float h = 1.f;

    const size_t N_t = t_max / tau + 1;
    const size_t N_x = x_max / h + 1;


    if (auto rc = MPI_Init(&argc, &argv)) {
        std::cout << "Ошибка запуска, выполнение остановлено " << std::endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    int num_processes = 0;
    int proc_id = 0;
    MPI_Comm_size(MPI_COMM_WORLD,&num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD,&proc_id);


    std::vector< std::vector<float> > matrix;
    matrix.resize(N_t);
    for (std::vector<float>& v : matrix) {
        v.resize(N_x);
    }
    //if (proc_id == 0) {
        for (size_t k = 0; k < N_t; ++k)
            matrix[k][0] = ksi(tau * k);
    //}

    double startTime = {};
    if (proc_id == 0) {
        startTime = MPI_Wtime();
    }

    for(size_t iteration = proc_id; iteration < N_t; iteration += num_processes) {
        for (size_t m = 1; m < N_x; m++) {
            if (iteration == 0) {
                matrix[iteration][m] = fi(h*m);
            } else {
                //receive
                size_t prev_proc = iteration % num_processes;
                prev_proc = (prev_proc) ? (prev_proc - 1) : num_processes - 1;
                MPI_Recv(&(matrix[iteration - 1][m]), 1, MPI_FLOAT
                         , prev_proc, m, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                //calc
                size_t k = iteration;
                matrix[iteration][m] = (2*h*tau/(h + c * tau))
                        * (F((k-1/2)*tau, (m-1/2)*h)
                        - (matrix[k][m-1] - matrix[k-1][m-1] - matrix[k-1][m]) / (2*tau)
                        - c * (-matrix[k][m-1] + matrix[k-1][m] - matrix[k-1][m-1]) / (2*h) );
                //matrix[iteration][m] = iteration;
            }
            MPI_Request req;
            MPI_Isend(&(matrix[iteration][m]), 1, MPI_FLOAT
                      , (iteration + 1) % num_processes, m
                      , MPI_COMM_WORLD, &req);
            //send
        }
    }

    std::vector<float> res;
    if (proc_id == 0) {
        res.resize(N_t * N_x);
    }
    for (size_t i = proc_id; i < N_t; i += num_processes) {
        //std::cout << i << " : " << matrix[i] << std::endl;
        MPI_Gather(matrix[i].data(), N_x, MPI_FLOAT
                   , res.data() + i * N_x, N_x, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }
    if(proc_id == 0) {
        for (size_t l = 0; l < N_t; l++) {
            for (size_t col = 0; col < N_x; col++) {
                std::cout << res[l * N_x + col] << ' ';
            }
            std::cout << std::endl;
        }
    }

    MPI_Finalize();
}
