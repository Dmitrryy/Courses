//
// Created by artem on 23.03.2021.
//
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include "pthread_manager.h"


double CalcIntegral (double begin, double end, double (*foo) (double x));
int Integrate (size_t n_threads, double begin, double end, double (*foo) (double), double *const result);
