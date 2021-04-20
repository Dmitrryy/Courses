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


enum ic_errors
{
    IC_SUCCESS
    , IC_BADARGS
    , IC_MEMLACK
    , IC_THREAD
};


double icCalculateMT(int numThreads, double begin, double end, icFunc func, enum ic_errors* err);