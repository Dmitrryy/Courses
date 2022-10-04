
#include "Helper.hpp"

#define SIZE 2048

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

size_t check(const Matrix<uint32_t> &A, const Matrix<uint32_t> &B);
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

    for (size_t l = 0; l < SIZE; l++)
    {
        for (size_t c = 0; c < SIZE; c++)
        {
            uint32_t sum = 0;
            for (size_t N = 0; N < SIZE; N++)
            {
                sum += A[l][N] * B[N][c];
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

std::vector<std::array<uint32_t, SIZE>> mul_fast(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B)
{
    std::vector<std::array<uint32_t, SIZE>> result(SIZE);

    return result;
}

std::vector<std::array<uint32_t, SIZE>> mul_fast_par(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B)
{
    std::vector<std::array<uint32_t, SIZE>> result(SIZE);

    return result;
}

template <typename T>
Matrix<T> multiply(const Matrix<T> &A, const Matrix<T> &B)
{
    size_t n = A.getSize();
    Matrix<T> result(n);

    for (size_t l = 0; l < n; l++)
    {
        for (size_t c = 0; c < n; c++)
        {
            T sum = 0;
            for (size_t N = 0; N < n; N++)
            {
                sum += A.at(l, N) * B.at(N, c);
            }
            result.at(l, c) = sum;
        }
    }

    return result;
}

template <typename T>
Matrix<T> multiply_par(const Matrix<T> &A, const Matrix<T> &B)
{
    size_t n = A.getSize();
    Matrix<T> result(n);

#pragma omp parallel for shared(result)
    for (int l = 0; l < n; l++)
    {
#pragma omp parallel for shared(result)
        for (int c = 0; c < n; c++)
        {
            T sum = 0;
            for (size_t N = 0; N < n; N++)
            {
                sum += A.at(l, N) * B.at(N, c);
            }
            result.at(l, c) = sum;
        }
    }

    return result;
}

template <typename T>
Matrix<T> multiply_par_simd(const Matrix<T> &A, const Matrix<T> &B)
{
    size_t n = A.getSize();
    Matrix<T> result(n);

#pragma omp parallel for shared(result)
    for (int l = 0; l < n; l++)
    {
#pragma omp parallel for shared(result)
        for (int c = 0; c < n; c++)
        {
            T sum = 0;
#pragma omp simd
            for (size_t N = 0; N < n; N++)
            {
                sum += A.at(l, N) * B.at(N, c);
            }
            result.at(l, c) = sum;
        }
    }

    return result;
}

int main(int argc, char *argv[])
{
    ezg::Random randomizer(static_cast<uint64_t>(0), static_cast<uint64_t>(100));

    Matrix<uint32_t> test(4);
    for (size_t i = 0; i < test.getSize(); ++i)
    {
        for (size_t k = 0; k < test.getSize(); ++k)
        {
            test.at(i, k) = randomizer();
        }
    }

    Matrix<uint32_t> A_2(SIZE);
    Matrix<uint32_t> B_2(SIZE);

    for (size_t i = 0; i < SIZE; ++i)
    {
        for (size_t k = 0; k < SIZE; ++k)
        {
            //A[i][k] = randomizer();
            //B[i][k] = randomizer();
            A_2.at(i, k) = randomizer();
            B_2.at(i, k) = randomizer();
        }
    }
 

    // omp_set_num_threads(4);

    double start_time = omp_get_wtime();
    auto C_seq_2 = multiply(A_2, B_2);
    double end_time = omp_get_wtime();
    std::cout << "Time seq = " << end_time - start_time << std::endl;
    //C_seq_2.dump();

    // B_2 = B_2.transpose();
    start_time = omp_get_wtime();
    auto C_par_2 = multiply_par(A_2, B_2);
    end_time = omp_get_wtime();
    std::cout << "Time par = " << end_time - start_time << ", loss = " << check(C_par_2, C_seq_2) << std::endl;

   start_time = omp_get_wtime();
    auto C_str_2 = Matrix<uint32_t>::strassen(A_2, B_2);
    end_time = omp_get_wtime();
    std::cout << "Time str = " << end_time - start_time << ", loss = " << check(C_str_2, C_seq_2) << std::endl;

    // B_2 = B_2.transpose();
    omp_set_num_threads(4);
    start_time = omp_get_wtime();
    Matrix<uint32_t> C_str_par_2(0);
    #pragma omp parallel
    {
        #pragma omp single
        {
            C_str_par_2 = Matrix<uint32_t>::strassen_par(A_2, B_2);
        }
    }
    end_time = omp_get_wtime();
    std::cout << "Time str par = " << end_time - start_time << ", loss = " << check(C_str_par_2, C_seq_2) << std::endl;
    //`C_str_2.dump();

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

size_t check(const Matrix<uint32_t> &A, const Matrix<uint32_t> &B)
{
    size_t loss = 0;
    for (size_t i = 0; i < SIZE; ++i)
    {
        for (size_t j = 0; j < SIZE; ++j)
        {
            loss += (A.at(i, j) != B.at(i, j));
        }
    }
    return loss;
}

std::vector<std::array<uint32_t, SIZE>> add(const std::vector<std::array<uint32_t, SIZE>> &A, const std::vector<std::array<uint32_t, SIZE>> &B)
{
    std::vector<std::array<uint32_t, SIZE>> result;

    for (size_t i = 0; i < SIZE; ++i)
    {
        for (size_t j = 0; j < SIZE; ++j)
        {
            result[i][j] = (A[i][j] + B[i][j]);
        }
    }

    return result;
}
