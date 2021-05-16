//
// Created by artem on 17.03.2021.
//
#pragma once

#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#include <sys/sysinfo.h>
#include <malloc.h>


struct cpu_info {
    struct cpu* cpus;
    size_t n_cpu;
};

struct cpu {
    int* processors;
    size_t n_proc;
    size_t capacity;
};

struct thread_info {
    double begin;
    double end;
    double res;
    double (*foo) (double);
};

struct cpu_info get_mycpu_info ();
int set_attrs (struct cpu_info cpuInfo, pthread_attr_t* attrs, size_t size);
struct thread_info** build_cache_aligned_thread_info (size_t n_threads);
int fill_thread_info (struct thread_info** infosp, size_t info_size, struct thread_info init, size_t n_threads);
