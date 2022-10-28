#include "fft.hpp"

namespace fft
{
    std::vector<std::complex<double>> fft_rec(const std::vector<std::complex<double>> &xIn)
    {
        auto N = xIn.size();
        // Check if it is splitted enough
        if (N <= 1)
        {
            return xIn;
        }

        // Split even and odd
        std::vector<std::complex<double>> odd(N / 2);
        std::vector<std::complex<double>> even(N / 2);

        for (int i = 0; i < N / 2; i++)
        {
            even[i] = xIn[i * 2];
            odd[i] = xIn[i * 2 + 1];
        }

        // Split on tasks
        even = fft_rec(even);
        odd = fft_rec(odd);

        // Calculate DFT
        std::vector<std::complex<double>> res(N);
        for (int k = 0; k < N / 2; k++)
        {
            std::complex<double> t = exp(std::complex<double>(0, -2 * M_PI * k / N)) * odd[k];
            res[k] = even[k] + t;
            res[N / 2 + k] = even[k] - t;
        }

        return res;
    }

    std::vector<std::complex<double>> fft_rec_par(const std::vector<std::complex<double>> &xIn)
    {
        auto N = xIn.size();
        // Check if it is splitted enough
        if (N == 2)
            return {xIn[0] + xIn[1], xIn[0] - xIn[1]};

        // Split even and odd
        std::vector<std::complex<double>> odd(N / 2);
        std::vector<std::complex<double>> even(N / 2);

        //#pragma omp parallel for shared(xIn, even, odd)
        //for (int i = 0; i < N / 2; i++)
        {
        //    even[i] = xIn[i * 2];
        //    odd[i] = xIn[i * 2 + 1];
        }

        // Split on tasks
        std::vector<std::complex<double>> even_res;
        #pragma omp task
        even_res = fft_rec(even);

        std::vector<std::complex<double>> odd_res;
        #pragma omp task
        odd_res = fft_rec(odd);

        #pragma omp taskwait

        // Calculate DFT
        std::vector<std::complex<double>> res(N);
        //#pragma omp parallel for shared(res, odd_res, even_res)
        //for (int k = 0; k < N / 2; k++)
        {
        //    std::complex<double> t = exp(std::complex<double>(0, -2 * M_PI * k / N)) * odd_res[k];
        //    res[k] = even_res[k] + t;
        //    res[N / 2 + k] = even_res[k] - t;
        }

        return res;
    }

    size_t bitReverse(size_t x, size_t log2n)
    {
        int n = 0;
        for (int i = 0; i < log2n; i++)
        {
            n <<= 1;
            n |= (x & 1);
            x >>= 1;
        }
        return n;
    }

    std::vector<std::complex<double>> fft_opt(std::vector<std::complex<double>> &xIn)
    {
        std::complex<double> J = {0, 1};
        auto N = xIn.size();
        auto log2n = std::bit_width(N) - 1;
        int n = 1 << log2n;

        std::vector<std::complex<double>> res(N);
        for (unsigned int i = 0; i < n; ++i)
        {
            res[bitReverse(i, log2n)] = xIn[i];
        }
        for (int s = 1; s <= log2n; ++s)
        {
            int m = 1 << s;
            int m2 = m >> 1;
            std::complex<double> w(1, 0);
            std::complex<double> wm = exp(-J * (M_PI / m2));
            for (int j = 0; j < m2; ++j)
            {
                for (int k = j; k < n; k += m)
                {
                    std::complex<double> t = w * res[k + m2];
                    std::complex<double> u = res[k];
                    res[k] = u + t;
                    res[k + m2] = u - t;
                }
                w *= wm;
            }
        }
        return res;
    }

    std::vector<std::complex<double>> fft_opt_par(std::vector<std::complex<double>> &xIn)
    {
        std::complex<double> J = {0, 1};
        auto N = xIn.size();
        auto log2n = std::bit_width(N) - 1;
        int n = 1 << log2n;

        std::vector<std::complex<double>> res(N);
        #pragma omp parallel for shared(res, xIn)
        for (int i = 0; i < n; ++i)
        {
            res[bitReverse(i, log2n)] = xIn[i];
        }
        
        //#pragma omp parallel for shared(res)
        for (int s = 1; s <= log2n; ++s)
        {
            int m = 1 << s;
            int m2 = m >> 1;
            std::complex<double> w(1, 0);
            std::complex<double> wm = exp(-J * (M_PI / m2));
            #pragma omp parallel for shared(res)
            for (int j = 0; j < m2; ++j)
            {
                for (int k = j; k < n; k += m)
                {
                    std::complex<double> t = w * res[k + m2];
                    std::complex<double> u = res[k];
                    res[k] = u + t;
                    res[k + m2] = u - t;
                }
                w *= wm;
            }
        }
        return res;
    }

} // namespace fft