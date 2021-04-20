/*************************************************************************************************
 *
 *   main.c
 *
 *   Created by dmitry
 *   20.04.2021
 *
 ***/

#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"


double func(double x)
{
    // y = ...
    return x * x;
}


// integration range [A, B]
const double g_A = 0.0;
const double g_B = 400.0;


int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("invalid arguments command line:\n argc = %d\n", argc);
        printf("They are:\n");
        for (int i = 0; i < argc; i++)
        {
            printf ("[%d] %s\n", i, argv[i]);
        }
        exit(1);
    }

    char* cur_end = NULL;
    int numThreads = (int) strtol(argv[1], &cur_end, 10);
    if (errno || *cur_end != 0 || numThreads <= 0) {
        printf("incorrect number of threads: %s, (interpret as: %d(int))\n", argv[1], numThreads);
        exit(2);
    }

    double res = icCalculateMT(numThreads, g_A, g_B, func);

    printf("res = %f\n", res);

    return 0;
}