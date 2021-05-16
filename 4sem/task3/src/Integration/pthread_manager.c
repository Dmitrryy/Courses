//
// Created by artem on 17.03.2021.
//

#include "Integration/pthread_manager.h"

static int find_in_array (const int* arr, int size, int elem) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == elem)
            return i;
    }
    return -1;
}

static int add_to_cpus_array (struct cpu** arr, size_t* size, struct cpu elem, size_t position) {

    if (position >= *size) {
        *arr = (struct cpu*) realloc (*arr, (2 * position + 1) * sizeof (struct cpu));
        *size = 2 * position;
    }

    if (*arr == NULL)
        return -1;

    (*arr)[position] = elem;
    return 0;
}

static int add_to_processors_array (int** arr, size_t* size, int elem, size_t position) {

    if (position >= *size) {
        *arr = (int*) realloc (*arr, (2 * position + 1) * sizeof (int));
        *size = 2 * position;
    }

    if (*arr == NULL)
        return -1;

    (*arr)[position] = elem;
    return 0;
}

struct cpu_info get_mycpu_info () {

    FILE * cmd = popen("grep \"core id\" /proc/cpuinfo", "r");
    char buff[14] = {0};
    int core_ids[256];
    int n_core_ids = 0;
    short num_read_bytes = 13;

    while(fread(buff, sizeof(char), num_read_bytes, cmd) > 0) {
        core_ids[n_core_ids++] = (int) strtol (buff + 10, NULL, 10);

        if (core_ids[n_core_ids - 1] == 9)
            num_read_bytes++;
    }

    struct cpu_info res = {
            .cpus = (struct cpu*) calloc(n_core_ids, sizeof(struct cpu)),
            .n_cpu = n_core_ids
    };


    if (res.cpus == NULL)
        return res;


    size_t n_cpu = 0;
    for (int i = 0; i < n_core_ids; ++i) {

        int check = 0;
        if (find_in_array(core_ids, i, core_ids[i]) == -1) {

            // if we haven't met this core_id yet
            struct cpu cpu = {};
            check = add_to_cpus_array(&res.cpus, &res.n_cpu, cpu, n_cpu);

            if (check != 0)
                goto exit_error;


            check = add_to_processors_array(&res.cpus[n_cpu].processors,
                                    &res.cpus[n_cpu].capacity,
                                    i,
                                    res.cpus[n_cpu].n_proc++);

            if (check != 0)
                goto exit_error;

            n_cpu++;
        } else {
            // if we have already met such core_id
            check = add_to_processors_array(&res.cpus[core_ids[i]].processors,
                                    &res.cpus[core_ids[i]].capacity,
                                    i,
                                    res.cpus[core_ids[i]].n_proc++);

            if (check != 0)
                goto exit_error;
        }
    }

    goto exit_success;

exit_error:

    for (int j = 0; j < n_cpu; ++j)
        free (res.cpus[j].processors);

    free (res.cpus);
    res.cpus = NULL;

exit_success:

    res.n_cpu = n_cpu;
    fclose(cmd);
    return res;
}

int set_attrs (struct cpu_info cpu_info, pthread_attr_t* attrs, size_t size) {

    for (int i = 0; i < size; ++i) {
        size_t num_cpu = i % cpu_info.n_cpu;
        size_t num_proc =(i / cpu_info.n_cpu) % cpu_info.cpus[num_cpu].n_proc;

        cpu_set_t  cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(cpu_info.cpus[num_cpu].processors[num_proc], &cpu_set);

        if (pthread_attr_setaffinity_np(&attrs[i], sizeof(cpu_set_t), &cpu_set) != 0)
            return -1;
    }

    return 0;
}

struct thread_info** build_cache_aligned_thread_info (size_t n_threads) {

    long cache_line_size = sysconf (_SC_LEVEL1_DCACHE_LINESIZE);
    long page_size = sysconf (_SC_PAGESIZE);
    size_t one_info_size = (sizeof (struct thread_info) / cache_line_size + 1) * cache_line_size;

    char* aligned_mem = (char*) memalign(page_size, n_threads * one_info_size);
    if (aligned_mem == NULL) {
        return NULL;
    }

    struct thread_info** infosp = (struct thread_info**) malloc(sizeof(struct thread_info*) * n_threads);
    if (infosp == NULL) {
        free (aligned_mem);
        return NULL;
    }

    for (int i = 0; i < n_threads; ++i)
        infosp[i] = (struct thread_info *) (aligned_mem + i * one_info_size);

    return infosp;
}

int fill_thread_info (struct thread_info** infosp, size_t info_size,
                       struct thread_info init, size_t n_threads) {

    if (infosp == NULL || n_threads == 0)
        return -1;

    const double interval = (init.end - init.begin) / (double) n_threads;

    double th_begin = init.begin;
    double th_end   = init.begin + interval;

    const double c_begin = th_begin;
    const double c_end = th_end;

    double (*foo) (double) = init.foo;

    for (int i = 0; i < n_threads - 1; ++i) {

        infosp[i]->begin   = th_begin;
        infosp[i]->end     = th_end;
        infosp[i]->foo     = foo;
        th_begin = th_end;
        th_end += interval;
    }

    infosp[n_threads - 1]->begin   = th_begin;
    infosp[n_threads - 1]->end     = init.end;
    infosp[n_threads - 1]->foo     = foo;

    //for threads, which must work not for result
    for (size_t i = n_threads; i < info_size; ++i) {
        infosp[i]->begin = c_begin;
        infosp[i]->end = c_end;
        infosp[i]->foo = foo;
    }

    return 0;
}