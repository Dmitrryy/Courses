//
// Created by dadro on 29.03.2022.
//
#include <iostream>
#include <vector>
#include <fstream>

#include <mpi.h>

template<typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T> &v) {
    for(auto&& elem: v) {
        out << elem << ' ';
    }
    return out;
}

int main(int argc, char *argv[]) {
    std::string outFName("result.txt");
    if(argc > 1) {
        outFName = argv[1];
    }

    // Условия на уравнение
    const float c = 0.6f;
    auto&& F = [](float t, float x) -> float {
        return t * x;
    };
    auto&& fi = [c](float x) -> float {
        return x * x * x / (12 * c * c);
    };
    auto&& ksi = [c](float t) -> float {
        return c * t * t * t / 12;
    };
    const float t_max = 20;
    const float x_max = 20;
    const float tau = 0.01f;
    const float h = 0.01f;

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
    for (size_t k = 0; k < N_t; ++k)
        matrix[k][0] = ksi(tau * k);

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
            }
            MPI_Send(&(matrix[iteration][m]), 1, MPI_FLOAT
                      , (iteration + 1) % num_processes, m
                      , MPI_COMM_WORLD);
        }
    }

    std::vector<float> res;
    if (proc_id == 0) {
        res.resize((N_t + (num_processes - N_t % num_processes)) * N_x);
    }
    std::vector<float>  vEmpty(N_x, 0);
    for (size_t i = proc_id; i < N_t + (num_processes - N_t % num_processes); i += num_processes) {
        float* sendBuff = vEmpty.data();
        if(i <= N_t - N_t % num_processes) {
            sendBuff = matrix[i].data();
        }
        MPI_Gather(sendBuff, N_x, MPI_FLOAT
                   , res.data() + i * N_x, N_x, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

    auto endTime = MPI_Wtime();
    if (proc_id == 0) {
        std::cout << "Time: "    << (endTime - startTime) * 1000 << std::endl;
    }

    if(proc_id == 0) {
        std::ofstream out(outFName);
        for (size_t l = 0; l < N_t; l++) {
            for (size_t col = 0; col < N_x; col++) {
                out << res[l * N_x + col] << ' ';
            }
            out << std::endl;
        }
        out.close();
    }

    MPI_Finalize();
}
