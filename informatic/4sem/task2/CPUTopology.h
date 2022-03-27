/*************************************************************************************************
 *
 *   CPUTopology.h
 *
 *   Created by dmitry
 *   20.04.2021
 *
 ***/

#pragma once



#include <stdbool.h>

typedef struct cpu_topology_t_ cpu_topology_t;

cpu_topology_t* cputopCreate ();
int cputopDestroy (cpu_topology_t** cputop);
int cputopInit    (cpu_topology_t* cputop);

bool cputopVerifier (cpu_topology_t* cputop);

int cputopGetNumLogicCPU (cpu_topology_t* cputop);
int cputopGetNumCoreCPU  (cpu_topology_t* cputop);
int cputopGetLogicCpuId  (cpu_topology_t* cputop, int num_logic_cpu);

int cputopSortUniqSetsCoreId (cpu_topology_t* cputop);

int cputopDump (cpu_topology_t* cputop);