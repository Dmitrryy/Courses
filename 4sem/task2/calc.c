/*************************************************************************************************
 *
 *   calc.c
 *
 *   Created by dmitry
 *   20.04.2021
 *
 ***/

/*#define _GNU_SOURCE*/
#include <sched.h>

#include "calc.h"
#include "CPUTopology.h"

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>

#define _POSIX_PRIORITY_SCHEDULING
#include <unistd.h>



#define PRINT_ERROR(str) print_error_line (str, __FILE__, __LINE__);

void print_error_line (const char strerr[], const char name_file[], int line) {
    char str_error[256];
    sprintf (str_error, "%s\n"
                        "LINE: %s: %d\n", strerr, name_file, line);
    perror (str_error);
}



const double eps = 1e-9;


typedef struct {
    double a, b, dx;
    double (* func) (double x);
    double result;
} integral_arg_t;



static void swap_double (double* first, double* second) {
    double temp = *first;
    *first = *second;
    *second = temp;
}




static void* pthread_calc_integral_ (void* pointer_int_arg) {
    register integral_arg_t int_arg = *(integral_arg_t*) pointer_int_arg;

    int_arg.result = 0;
    while (int_arg.a < int_arg.b) {
        int_arg.result += int_arg.func (int_arg.a);
        int_arg.a += int_arg.dx;
    }

    ((integral_arg_t*) pointer_int_arg)->result = int_arg.result;

    return NULL;
}




static void detachThreads_ (pthread_t* arr_tid, size_t num_arr) {
    for (int i = 0; i < num_arr; ++i)
        pthread_detach (arr_tid[i]);
}

static int destroyAttrThread_ (pthread_attr_t* pid_attr_arr, unsigned num_arr) {
    if (pid_attr_arr == NULL) {
        return -1; //TODO
    }
    for (int i = 0; i < num_arr; ++i)
        pthread_attr_destroy (&pid_attr_arr[i]);

    return 0;
}


static int distributeAttrThreads_ (pthread_attr_t* tid_attr_arr, unsigned num_threads, cpu_topology_t* cputop) {

    const int num_logic_cpu = cputopGetNumLogicCPU (cputop);
    if (num_logic_cpu == -1) {
        PRINT_ERROR ("cputopGetNumLogicCPU");
        return -1;
    }

    cpu_set_t cpuset = {0};

    for (unsigned num_thread = 0; num_thread < num_threads; ++num_thread) {

        pthread_attr_t* tid_attr = &tid_attr_arr[num_thread];

        if (pthread_attr_init (tid_attr)) {
            PRINT_ERROR ("pthread_attr_init");
            return -1;
        }

        const int logic_cpu_id = cputopGetLogicCpuId (cputop, num_thread % num_logic_cpu);
        if (logic_cpu_id == -1) {
            PRINT_ERROR ("cputopGetLogicCpuId");
            return -1;
        }

        CPU_ZERO (&cpuset);
        CPU_SET (logic_cpu_id, &cpuset);

        if (pthread_attr_setaffinity_np (tid_attr, sizeof (cpuset), &cpuset)) {
            PRINT_ERROR ("pthread_setaffinity_np");
            return -1;
        }
    }

    return 0;
}


static double integrate (double a, double b, double (* func) (double),
        int num_threads, cpu_topology_t* cputop) {


    int num_dummy = 0, num_logic_cpu = cputopGetNumLogicCPU (cputop);
    if (num_threads < num_logic_cpu)
        num_dummy = num_logic_cpu - num_threads;

    pthread_attr_t* tid_attr_arr = (pthread_attr_t*) malloc ( sizeof(pthread_attr_t) * (num_threads + num_dummy) );
    if (tid_attr_arr == NULL) {
        PRINT_ERROR("cant allocate memory 1");
        return NAN;
    }

    int state = distributeAttrThreads_(tid_attr_arr, num_threads + num_dummy, cputop);
    if (state) {
        //free(tid_attr_arr);
        PRINT_ERROR ("_distributeAttrThreads");
        return NAN;
    }

    // Set a < b
    bool sign_int = a <= b;
    if (!sign_int)
        swap_double (&a, &b);

    const double dx  = (b - a) * eps;
    const double len = (b - a) / num_threads;


    pthread_t*      tid_arr = (pthread_t*)malloc(sizeof(pthread_t) * (num_threads + num_dummy) );
    if (tid_arr == NULL) { PRINT_ERROR("cant allocate memory 2"); /*free(tid_attr_arr);*/ return NAN; }
    integral_arg_t* int_args = (integral_arg_t*)malloc(sizeof(integral_arg_t) * (num_threads + num_dummy) );
    if (int_args == NULL) { PRINT_ERROR("cant allocate memory 3"); /*free(tid_attr_arr); free(tid_arr);*/ return NAN; }


    // Create pthreads with special attributs
    for (int i = 0; i < num_threads; ++i) {
        integral_arg_t* int_arg = &int_args[i];
        int_arg->dx   = dx;
        int_arg->func = func;
        int_arg->a    = a;
        int_arg->b    = a += len;

        state = pthread_create(&tid_arr[i], &tid_attr_arr[i], pthread_calc_integral_, (void*) int_arg);
        if (state) {
            PRINT_ERROR ("cant create a thread!");
            //free(tid_attr_arr); free(tid_arr); free(int_args);
            //detachThreads_(tid_arr, i);
            //destroyAttrThread_(tid_attr_arr, num_threads + num_dummy);
            return NAN;
        }
    }

    // n v t = s n
    // m v t = s m

    if (num_dummy != 0) {
        const double dx_dummy  = dx  * num_threads / num_dummy;
        const double len_dummy = len * num_threads / num_dummy;

        a -= len * num_threads;

        // Create dummy pthreads with special attributs
        for (int i = num_threads; i < num_threads + num_dummy; ++i) {
            integral_arg_t* int_arg = &int_args[i];
            int_arg->dx   = dx_dummy;
            int_arg->func = func;
            int_arg->a    = a;
            int_arg->b    = a += len_dummy;

            state = pthread_create(&tid_arr[i], &tid_attr_arr[i], pthread_calc_integral_, (void*) int_arg);
            if (state) {
/*                PRINT_ERROR ("pthread_create");
                free(tid_attr_arr); free(tid_arr); free(int_args);
                detachThreads_(tid_arr, i);
                destroyAttrThread_(tid_attr_arr, num_threads + num_dummy);*/
                return NAN;
            }
        }
    }

    if (destroyAttrThread_(tid_attr_arr, num_threads + num_dummy) == -1) {
        PRINT_ERROR ("_destroyAttrThread");
/*        free(tid_attr_arr); free(tid_arr);
        detachThreads_(tid_arr, num_threads);*/
        return -1;
    }

    // Join all pthreads and check for error by ret_val
    double res = 0;
    for (int i = 0; i < num_threads + num_dummy; ++i) {
        int* ret_val = NULL;
        state = pthread_join (tid_arr[i], (void**) &ret_val);

        if (state || ret_val != NULL) {
            errno = EINVAL;
            PRINT_ERROR ("pthread_join");
            detachThreads_(tid_arr + i + 1, num_threads + num_dummy - i - 1);
            free(tid_attr_arr); free(tid_arr); free(int_args);

            return NAN;
        }

        if (i < num_threads)
            res += int_args[i].result;
    }

    free(tid_attr_arr); free(tid_arr); free(int_args);


    return res * (2 * sign_int - 1) * dx;
}



double icCalculateMT(int numThreads, double begin, double end, icFunc func)
{
    if ((isfinite (begin) && isfinite (end) && func != NULL && numThreads > 0) == false) {
        errno = EINVAL;
        PRINT_ERROR ("(isfinite (a) && isfinite (b) && func == NULL && num_threads > 0) is false");
        return NAN;
    }

    cpu_topology_t* cputop = cputopCreate ();
    if (cputop == NULL) {
        PRINT_ERROR ("cputopCreate");
        return NAN;
    }

    if (cputopInit (cputop) == -1) {
        PRINT_ERROR ("cputopInit");
        cputopDestroy (&cputop);
        return NAN;
    }

    if (cputopSortUniqSetsCoreId (cputop) == -1) {
        PRINT_ERROR ("cputopSortUniqSetsCoreId");
        cputopDestroy (&cputop);
        return NAN;
    }

    double result = integrate(begin, end, func, numThreads, cputop);

    if (cputopDestroy (&cputop) == -1) {
        PRINT_ERROR ("cputopDestroy");
        return -1;
    }

    return result;
}



double icCalculate(double a, double b, double dx, icFunc func)
{
    bool sign_int = a <= b;

    if (!sign_int)
        swap_double (&a, &b);

    double res = 0;

    while (a < b) {
        res += func (a);
        a += dx;
    }

    return res * (2 * sign_int - 1) * dx;
}