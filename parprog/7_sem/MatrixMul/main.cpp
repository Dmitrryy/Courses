#include <omp.h>

#include <optional>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <iostream>
#include <random>

#define SIZE 1500

namespace ezg
{
    class Random
    {
    public:
        Random(unsigned _min = std::mt19937::min(), unsigned _max = std::mt19937::max())
            : m_rd(), m_gen(m_rd()), m_dist{static_cast<int>(_min), static_cast<int>(_max)}
        {
        }

        int operator()()
        {
            return get();
        }

        int get() { return m_dist(m_gen); }

    private:
        std::random_device m_rd;
        std::mt19937 m_gen;

        std::uniform_int_distribution<> m_dist;
    };
} // namespace ezg

size_t check(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B);
std::vector<std::array<uint32_t, SIZE>> add(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B);

std::vector<std::array<uint32_t, SIZE>> transpose(const std::vector<std::array<uint32_t, SIZE>> &A)
{
    std::vector<std::array<uint32_t, SIZE>> result(SIZE);

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            result[i][j] = A[j][i];
        }
    }

    return result;
}

std::vector<std::array<uint32_t, SIZE>> mul_seq(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B)
{
    std::vector<std::array<uint32_t, SIZE>> result(SIZE);

    auto B_t = transpose(B);

    for (size_t l = 0; l < SIZE; l++)
    {
        for (size_t c = 0; c < SIZE; c++)
        {
            uint32_t sum = 0;
            for (size_t N = 0; N < SIZE; N++)
            {
                sum += A[l][N] * B_t[c][N];
            }
            result[l][c] = sum;
        }
    }

    return result;
}

std::vector<std::array<uint32_t, SIZE>> mul_par(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B)
{
    std::vector<std::array<uint32_t, SIZE>> result(SIZE);

    auto B_t = transpose(B);

#pragma omp parallel for shared(result)
    for (int l = 0; l < SIZE; l++)
    {
#pragma omp parallel for shared(result)
        for (int c = 0; c < SIZE; c++)
        {
            uint32_t sum = 0;
            for (int N = 0; N < SIZE; N++)
            {
                sum += A[l][N] * B_t[c][N];
            }
            result[l][c] = sum;
        }
    }

    return result;
}

std::vector<std::array<uint32_t, SIZE>> mul_par_simd(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B)
{
    std::vector<std::array<uint32_t, SIZE>> result(SIZE);

    auto B_t = transpose(B);

#pragma omp parallel for shared(result)
    for (int l = 0; l < SIZE; l++)
    {
#pragma omp parallel for shared(result)
        for (int c = 0; c < SIZE; c++)
        {
            uint32_t sum = 0;
#pragma omp simd
            for (int N = 0; N < SIZE; N++)
            {
                sum += A[l][N] * B_t[c][N];
            }
            result[l][c] = sum;
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    std::vector<std::array<uint32_t, SIZE>> A(SIZE);
    std::vector<std::array<uint32_t, SIZE>> B(SIZE);

    ezg::Random randomizer(static_cast<uint64_t>(0), static_cast<uint64_t>(10));
    for (size_t i = 0; i < SIZE; ++i)
    {
        for (size_t k = 0; k < SIZE; ++k)
        {
            A[i][k] = randomizer();
            B[i][k] = randomizer();
        }
    }

    

    //omp_set_num_threads(4);

    double start_time = omp_get_wtime();
    auto C_seq = mul_seq(A, B);
    double end_time = omp_get_wtime();
    std::cout << "Time seq = " << end_time - start_time << std::endl;

    start_time = omp_get_wtime();
    auto C_par = mul_par(A, B);
    end_time = omp_get_wtime();
    std::cout << "Time par = " << end_time - start_time << ", loss = " << check(C_par, C_seq) << std::endl;

    start_time = omp_get_wtime();
    auto C_par_simd = mul_par_simd(A, B);
    end_time = omp_get_wtime();
    std::cout << "Time simd= " << end_time - start_time << ", loss = " << check(C_par_simd, C_seq) << std::endl;

    return 0;
}

size_t check(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B)
{
    size_t loss = 0;
    for (size_t i = 0; i < SIZE; ++i)
    {
        for (size_t j = 0; j < SIZE; ++j)
        {
            loss += (A[i][j] != B[i][j]);
        }
    }
    return loss;
}
