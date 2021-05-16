#include "include//common.h"


int main (int argc, char *argv[]) {

    struct start_pack start = parse_args (argc, argv);

    if (start.server == -1)
        return 0;

    if (start.server == 1)
        run_server (start.n_threads, start.n_machines);
    else if (start.server == 0)
        run_worker();

    return 0;
}