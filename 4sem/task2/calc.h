/*************************************************************************************************
 *
 *   calc.h
 *
 *   Created by dmitry
 *   20.04.2021
 *
 ***/

#pragma once


#define _GNU_SOURCE
#include <sched.h>


typedef double (* icFunc)(double);


///  function wor calculate integral for 'func' on range [begin, end]
/// \param numThreads - num of threads for calculate
/// \param begin      - left side of range
/// \param end        - right side of range
/// \param func       - integration function
/// \param err        - if any wrong, in 'err' sets code of error
/// \return result of calculation
double icCalculateMT(int numThreads, double begin, double end, icFunc func);