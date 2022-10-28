#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <iterator>
#include <iostream>
#include <complex>
#include <bit>
#include <vector>

#include <omp.h>

namespace fft
{
    std::vector<std::complex<double>> fft_rec(const std::vector<std::complex<double>> &xIn);
    std::vector<std::complex<double>> fft_rec_par(const std::vector<std::complex<double>> &xIn);

    size_t bitReverse(size_t x, size_t log2n);
    std::vector<std::complex<double>> fft_opt(std::vector<std::complex<double>> &xIn);
    std::vector<std::complex<double>> fft_opt_par(std::vector<std::complex<double>> &xIn);
} // namespace fft