/*************************************************************************************************
 *
 *   main.c
 *
 *   Created by dmitry
 *   10.05.2021
 *
 ***/

#include "include/common.h"
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>


const char *serv_flag = "-s";
const char *client_flag = "-c";


int main (int argc, char *argv[]) {

    struct start_pack startPack = {-1, 0, 0};

    if (argc >= 2)
    {
        if (strcmp (argv[1], serv_flag) == 0)
        {
            startPack.server = 1;

            if (argc != 4) {
                printf("invalid arguments command line:\n argc = %d\n", argc);
                printf("They are:\n");
                for (int i = 0; i < argc; i++)
                {
                    printf ("[%d] %s\n", i, argv[i]);
                }
                exit(1);
            }

            char* cur_end = NULL;
            startPack.n_machines = (int) strtoull(argv[2], &cur_end, 10);
            if (errno || *cur_end != 0 || startPack.n_machines <= 0) {
                printf("incorrect number of threads: %s, (interpret as: %lu(int))\n", argv[2], startPack.n_machines);
                exit(2);
            }
            startPack.n_threads = (int) strtoull(argv[3], &cur_end, 10);
            if (errno || *cur_end != 0 || startPack.n_threads <= 0) {
                printf("incorrect number of threads: %s, (interpret as: %lu(int))\n", argv[3], startPack.n_threads);
                exit(2);
            }
        }
        if (strcmp (argv[1], client_flag) == 0)
        {
            startPack.server = 0;

            if (argc > 2) {
                printf("The following command line parameters are ignored:\n");
                for (int i = 2; i < argc; i++)
                {
                    printf ("[%d] %s\n", i, argv[i]);
                }
            }
        }
    }
    else {
        startPack.server = 0;
    }


    if (startPack.server == -1)
        return 0;

    if (startPack.server == 1)
        run_server (startPack.n_threads, startPack.n_machines);
    else if (startPack.server == 0)
        run_worker();

    return 0;
}