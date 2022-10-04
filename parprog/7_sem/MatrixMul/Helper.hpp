#pragma once

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
#include <iomanip>

template <typename T>
class Matrix
{
    std::vector<T> m_data;
    size_t m_size = 0;

public:
    Matrix(size_t size)
        : m_data(size * size), m_size(size)
    {
    }
    Matrix(const Matrix &) = default;
    Matrix &operator=(const Matrix &) = default;
    Matrix(Matrix &&) = default;
    Matrix &operator=(Matrix &&) = default;

public:
    T &at(size_t l, size_t c) { return const_cast<T &>(static_cast<const Matrix *>(this)->at(l, c)); }
    const T &at(size_t l, size_t c) const
    {
        return m_data[m_size * l + c];
    }
    size_t getSize() const { return m_size; }

public:
    Matrix transpose() const
    {
        Matrix<T> result(m_size);
        for (int i = 0; i < m_size; ++i)
        {
            for (int j = 0; j < m_size; ++j)
            {
                result.at(i, j) = at(j, i);
            }
        }
        return result;
    }

public:
    static Matrix combine(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22)
    {
        size_t m = c11.getSize();
        size_t n = m * 2;
        Matrix<T> result(n);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i < m && j < m)
                    result.at(i, j) = c11.at(i, j);
                else if (i < m)
                    result.at(i, j) = c12.at(i, j - m);
                else if (j < m)
                    result.at(i, j) = c21.at(i - m, j);
                else
                    result.at(i, j) = c22.at(i - m, j - m);
            }
        }
        return result;
    }

    static Matrix add(const Matrix &A, const Matrix &B)
    {
        size_t n = A.getSize();
        Matrix<T> result(n);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                result.at(i, j) = A.at(i, j) + B.at(i, j);
            }
        }
        return result;
    }

    static Matrix sub(const Matrix &A, const Matrix &B)
    {
        size_t n = A.getSize();
        Matrix<T> result(n);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                result.at(i, j) = A.at(i, j) - B.at(i, j);
            }
        }
        return result;
    }

    static Matrix getSlice(const Matrix &src, size_t size, size_t offseti, size_t offsetj)
    {
        Matrix<T> result(size);

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                result.at(i, j) = src.at(offseti + i, offsetj + j);
            }
        }

        return result;
    }

    static Matrix multiply_t(const Matrix &A, const Matrix &B_t)
    {
        size_t n = A.getSize();
        Matrix<T> result(n);

        for (size_t l = 0; l < n; l++)
        {
            for (size_t c = 0; c < n; c++)
            {
                T sum = 0;
//#pragma omp simd
                for (size_t N = 0; N < n; N++)
                {
                    sum += A.at(l, N) * B_t.at(c, N);
                }
                result.at(l, c) = sum;
            }
        }

        return result;
    }

    static Matrix multiply_par_t(const Matrix &A, const Matrix &B_t)
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
                    sum += A.at(l, N) * B_t.at(c, N);
                }
                result.at(l, c) = sum;
            }
        }

        return result;
    }

    static Matrix strassen_par(const Matrix &lhs, const Matrix &rhs)
    {
        if (lhs.getSize() < 32)
        {
            auto tmp = rhs.transpose();
            return multiply_t(lhs, tmp);
        }

        size_t m = lhs.getSize() / 2;

        Matrix a = getSlice(lhs, m , 0, 0);
        Matrix b = getSlice(lhs, m , 0, m);
        Matrix c = getSlice(lhs, m , m, 0);
        Matrix d = getSlice(lhs, m , m, m);

        Matrix e = getSlice(rhs, m ,0 , 0);
        Matrix f = getSlice(rhs, m ,0 , m);
        Matrix g = getSlice(rhs, m ,m , 0);
        Matrix h = getSlice(rhs, m ,m , m);

        Matrix<T> s1(0);
        #pragma omp task shared(s1)
        {
            Matrix<T> bds = sub(b, d);
            Matrix<T> gha = add(g, h);
            s1 = strassen(bds, gha);
        }


        Matrix<T> s2(0);
        #pragma omp task shared(s2)
        {
            Matrix<T> ada = add(a, d);
            Matrix<T> eha = add(e, h);
            s2 = strassen(ada, eha);
        }

        Matrix<T> s3(0);
        #pragma omp task shared(s3)
        {
            Matrix<T> acs = sub(a, c);
            Matrix<T> efa = add(e, f);
            s3 = strassen(acs, efa);
        }

        Matrix<T> s4(0);
        #pragma omp task shared(s4)
        {
            Matrix<T> aba = add(a, b);
            s4 = strassen(aba, h);
        }

        Matrix<T> s5(0);
        #pragma omp task shared(s5)
        {
            Matrix<T> fhs = sub(f, h);
            s5 = strassen(a, fhs);
        }

        Matrix<T> s6(0);
        #pragma omp task shared(s6)
        {
            Matrix<T> ges = sub(g, e);
            s6 = strassen(d, ges);
        }

        Matrix<T> s7(0);
        #pragma omp task shared(s7)
        {
            Matrix<T> cda = add(c, d);
            s7 = strassen(cda, e);
        }

        #pragma omp taskwait


        Matrix<T> c11(0);
        #pragma omp task shared(c11)
        {
            Matrix<T> s1s2a = add(s1, s2);
            Matrix<T> s6s4s = sub(s6, s4);
            c11 = add(s1s2a, s6s4s);
        }

        Matrix<T> c12(0);
        #pragma omp task shared(c12)
        {
            c12 = add(s4, s5);
        }

        Matrix<T> c21(0);
        #pragma omp task shared(c21)
        {
            c21 = add(s6, s7);
        }

        Matrix<T> c22(0);
        #pragma omp task shared(c22)
        {
            Matrix<T> s2s3s = sub(s2, s3);
            Matrix<T> s5s7s = sub(s5, s7);
            c22 = add(s2s3s, s5s7s);
        }

        #pragma omp taskwait

        return combine(c11, c12, c21, c22);
    }

    static Matrix strassen(const Matrix &lhs, const Matrix &rhs)
    {
        if (lhs.getSize() < 32)
        {
            auto tmp = rhs.transpose();
            return multiply_t(lhs, tmp);
        }

        size_t m = lhs.getSize() / 2;

        Matrix a = getSlice(lhs, m , 0, 0);
        Matrix b = getSlice(lhs, m , 0, m);
        Matrix c = getSlice(lhs, m , m, 0);
        Matrix d = getSlice(lhs, m , m, m);

        Matrix e = getSlice(rhs, m ,0 , 0);
        Matrix f = getSlice(rhs, m ,0 , m);
        Matrix g = getSlice(rhs, m ,m , 0);
        Matrix h = getSlice(rhs, m ,m , m);

        Matrix<T> s1(0);
        //#pragma omp task shared(s1)
        {
            Matrix<T> bds = sub(b, d);
            Matrix<T> gha = add(g, h);
            s1 = strassen(bds, gha);
        }


        Matrix<T> s2(0);
        //#pragma omp task shared(s2)
        {
            Matrix<T> ada = add(a, d);
            Matrix<T> eha = add(e, h);
            s2 = strassen(ada, eha);
        }

        Matrix<T> s3(0);
        //#pragma omp task shared(s3)
        {
            Matrix<T> acs = sub(a, c);
            Matrix<T> efa = add(e, f);
            s3 = strassen(acs, efa);
        }

        Matrix<T> s4(0);
        //#pragma omp task shared(s4)
        {
            Matrix<T> aba = add(a, b);
            s4 = strassen(aba, h);
        }

        Matrix<T> s5(0);
        //#pragma omp task shared(s5)
        {
            Matrix<T> fhs = sub(f, h);
            s5 = strassen(a, fhs);
        }

        Matrix<T> s6(0);
        //#pragma omp task shared(s6)
        {
            Matrix<T> ges = sub(g, e);
            s6 = strassen(d, ges);
        }

        Matrix<T> s7(0);
        //#pragma omp task shared(s7)
        {
            Matrix<T> cda = add(c, d);
            s7 = strassen(cda, e);
        }

        //#pragma omp taskwait


        Matrix<T> c11(0);
        //#pragma omp task shared(c11)
        {
            Matrix<T> s1s2a = add(s1, s2);
            Matrix<T> s6s4s = sub(s6, s4);
            c11 = add(s1s2a, s6s4s);
        }

        Matrix<T> c12(0);
        //#pragma omp task shared(c12)
        {
            c12 = add(s4, s5);
        }

        Matrix<T> c21(0);
        //#pragma omp task shared(c21)
        {
            c21 = add(s6, s7);
        }

        Matrix<T> c22(0);
        //#pragma omp task shared(c22)
        {
            Matrix<T> s2s3s = sub(s2, s3);
            Matrix<T> s5s7s = sub(s5, s7);
            c22 = add(s2s3s, s5s7s);
        }

        //#pragma omp taskwait

        return combine(c11, c12, c21, c22);
    }

public:
    void dump() const {
        for(size_t i = 0; i < m_size; ++i) {
            for(size_t j = 0; j < m_size; ++j) {
                std::cout << std::setw(5) << at(i, j);
            }
            std::cout << std::endl;
        }
    }
};
