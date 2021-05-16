//
// Created by artem on 01.03.2021.
//

#include "common.h"
#include "Integration/calc.h"


const double step = 1e-7;

typedef double (*func) (double);


double CalcIntegral (double begin, double end, func foo) {

    if (end < begin) {
        double tmp = end;
        end = begin;
        begin = tmp;
    }

    double res = 0;
    double x = begin;
    double f1 = foo (x);
    double f2 = foo (x + step);

    while (x < end) {
        res += step * (f1 + f2) / 2;
        x += step;
        f1 = f2;
        f2 = foo (x);
    }

    return res;
}

void* start_routine (void* arg) {

    struct thread_info* params = arg;
    params->res = CalcIntegral(params->begin, params->end, params->foo);
    return NULL;
}

int Integrate (size_t n_threads, double begin, double end, func foo, double *const result) {

    //if error is occurred, then we put an appropriate value and return it
    int error = SUCCESS;

    if (n_threads == 0 || foo == NULL || result == NULL)
        return E_INVAL;

    // number of cpus we have on this system
    size_t n_cpus = (size_t) get_nprocs();
    size_t n_thread_create = n_threads > n_cpus ? n_threads : n_cpus;

    struct cpu_info cpuInfo = get_mycpu_info();

    if (cpuInfo.cpus == NULL) {
        perror("get_cpu_info");
        return E_MEM;
    }


    pthread_t *threads = (pthread_t *) malloc(n_thread_create * sizeof(pthread_t));

    if (threads == NULL) {
        perror("threads allocation");
        error = E_MEM;
        goto exit_threads;
    }

    struct thread_info **infosp = build_cache_aligned_thread_info(n_thread_create);

    if (infosp == NULL) {
        perror("infosp allocation");
        error = E_MEM;
        goto exit_infosp;
    }

    struct thread_info init = {
            .begin = begin,
            .end = end,
            .foo = foo
    };

    if (fill_thread_info(infosp, n_thread_create, init, n_threads) != 0) {
        error = E_INVAL;
        goto exit_infosp;
    }

    pthread_attr_t *attrs = (pthread_attr_t *) malloc(n_thread_create * sizeof(pthread_attr_t));

    if (attrs == NULL) {
        perror("attrs allocation");
        error = E_MEM;
        goto exit_attrs;
    }

    for (int i = 0; i < n_thread_create; ++i) {

        int check = pthread_attr_init(attrs + i);
        if (check != 0) {
            perror("pthread_attr_init");
            error = E_THREAD;
            goto exit_attrs;
        }
    }

    if (n_threads <= n_thread_create) {

        if (set_attrs(cpuInfo, attrs, n_thread_create) != 0) {
            perror("set_attrs");
            error = E_THREAD;
            goto exit_all;
        }
    }


    int check = 0;
    for (int i = 0; i < n_thread_create; ++i) {

        check = pthread_create(&threads[i], &attrs[i], start_routine, (void *) infosp[i]);
        if (check != 0) {
            perror("pthread_create");
            error = E_THREAD;
            goto exit_all;
        }
    }

    double res = 0;
    for (int i = 0; i < n_threads; ++i) {

        check = pthread_join(threads[i], NULL);
        if (check != 0) {
            perror("pthread_join calc:");
            error = E_THREAD;
            goto exit_all;
        }

        res += infosp[i]->res;
    }

    *result = res;

exit_all:

    for (int i = 0; i < n_thread_create; ++i)
        pthread_attr_destroy(&attrs[i]);

    free(attrs);

exit_attrs:

    free(infosp[0]);
    free(infosp);

exit_infosp:

    free(threads);

exit_threads:

    for (int i = 0; i < cpuInfo.n_cpu; ++i)
        free(cpuInfo.cpus[i].processors);

    free(cpuInfo.cpus);

    return error;
}
