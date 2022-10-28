#include "fft.hpp"

#include <fstream>

int main(int argc, char **argv)
{
    // generate signal
    size_t N = std::pow(2, 20);
    std::cout << "N: " << N << std::endl;
    const double m = 4.0;
    std::vector<std::complex<double>> x(N);
    for (size_t i = 0; i < N; ++i) {
        x[i] = std::sin(2 * M_PI / N * m * i) + std::sin(2 * M_PI / N * (m + 0.25) * i);
    }

    double start_time = omp_get_wtime();
    auto && res_rec = fft::fft_rec(x);
    double end_time = omp_get_wtime();
    std::cout << "Time rec: " << end_time - start_time << std::endl;

    //start_time = omp_get_wtime();
    //#pragma omp parallel
    //{
    //    #pragma omp single
    //    {
    //        auto && res_rec_par = fft::fft_rec_par(x);
    //    }
    //}
    //end_time = omp_get_wtime();
    //std::cout << "Time rec par: " << end_time - start_time << std::endl;

    start_time = omp_get_wtime();
    auto && res_opt = fft::fft_opt(x);
    end_time = omp_get_wtime();
    std::cout << "Time opt: " << end_time - start_time << std::endl;

    start_time = omp_get_wtime();
    auto && res_opt_par = fft::fft_opt_par(x);
    end_time = omp_get_wtime();
    std::cout << "Time opt par: " << end_time - start_time << std::endl;

    std::ofstream fileRes("result.txt");
    for (int i = 0; i < N; ++i)
        fileRes << static_cast<double>(i) / N << " " << std::abs(res_opt[i]) << "\n";
    fileRes.close();

    return 0;
}
