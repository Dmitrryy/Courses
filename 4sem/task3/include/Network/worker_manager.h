#pragma once

#define _GNU_SOURCE
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/sysinfo.h>
#include <sys/select.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


struct worker_info {
    double begin;
    double end;
    size_t n_threads;
};

struct handler_info {
    struct worker_info w_info;
    int socket;
};

struct tasks_for_workers {
    struct handler_info *task;
    size_t size;
};


void *work_handler (void *);

struct tasks_for_workers *
divide_work (unsigned n_machines, unsigned n_threads,
             double begin, double end, int *error);

void dump_tasks (struct tasks_for_workers *tasks);

int send_hello_message ();


int get_tcp_connections (struct tasks_for_workers *tasks);

int get_result (struct tasks_for_workers *tasks, double *res);
