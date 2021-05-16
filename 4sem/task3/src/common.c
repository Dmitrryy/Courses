//
// Created by artem on 20.04.2021.
//

#include "common.h"
#include "Network/worker_manager.h"
#include "Network/worker.h"

const double begin = 0.0;
const double end = 500.0;

/**
 * parse user arguments to define a role and parameters
 * @param argc size of argv[]
 * @param argv an array of user parameters
 * @return On success, it returns {@code start_pack} structure,
 * where will be role, number of machines and number of threads placed.
 * The {@code .server} field of structure will be either 1 if it is server
 * or 0 if it is worker part. On error, {@code .server}
 * field of structure will be -1.
 */
struct start_pack parse_args (int argc, char *argv[]) {

    struct start_pack res = {0, 0, 0};

    char *serv_flag = "-s";

    if (argc > 2 && argc < 4) {
        fprintf (stderr, "Not all args for server, and redundant for worker\n");
        res.server = -1;
        return res;
    }

    if (argc == 1) {
        return res;
    }

    if (argc == 4) {

        if (strcmp (argv[1], serv_flag) != 0) {
            fprintf (stderr, "usage: '-s' n_machines n_threads\n");
            res.server = -1;
            return res;
        }

        res.server = 1;
        res.n_machines = (size_t) strtol (argv[2], NULL, 10);
        res.n_threads = (size_t) strtol (argv[3], NULL, 10);
    }

    return res;
}




/**
 * run in new thread, which send hello-messages for discovering peers
 * @param arg ignored
 * @return ignored
 */
static void *send_hellos (void *arg) {

    int error = SUCCESS;
    while (1) {

        error = send_hello_message();

        if (error != SUCCESS) {
            p_error (error);
            pthread_exit (NULL);
        }
    }
}

/**
 * run in new thread and listen for new connections
 * @param arg a pointer to {@code tasks_for_workers} structure,
 * where connected sockets will be placed
 * @return ignored
 */
static void *get_tcps (void *arg) {

    struct tasks_for_workers* tasks = arg;

    int error = get_tcp_connections (tasks);

    if (error != SUCCESS) {
        p_error (error);
        fprintf (stderr, "Computation failed!\n");
    }

    return NULL;
}

/**
 * runs the server part of program
 * @param n_threads number of threads, which should be used for computing
 * @param n_machines number of available machines
 */
void run_server(size_t n_threads, size_t n_machines) {

    int error = SUCCESS;
    struct tasks_for_workers *tasks;

    tasks = divide_work (n_machines, n_threads, begin, end, &error);
    dump_tasks (tasks);

    if (error != SUCCESS) {
        p_error (error);
        fprintf (stderr, "Computation failed!\n");
        return;
    }

    pthread_t th_sender;
    pthread_t th_getter;

    if (pthread_create (&th_getter, NULL, get_tcps, tasks) != 0) {
        perror ("pthread_create getter:");
        return;
    }

    if (pthread_create (&th_sender, NULL, send_hellos, NULL) != 0) {
        perror ("pthread_create sender:");
        return;
    }

    if (pthread_join (th_getter, NULL) != 0) {
        perror ("pthread_join getter:");
        goto exit;
    }

    //TODO: why it crashes?
    pthread_cancel (th_sender);

    double res = 0;
    error = get_result (tasks, &res);

    if (error != SUCCESS) {
        p_error (error);
        fprintf (stderr, "Computation failed!\n");
        goto exit;
    }

    printf ("result is %f\n", res);

exit:
    for (int i = 0; i < tasks->size; ++i) {
        close (tasks->task[i].socket);
    }
}


/**
 * run worker part of program
 * @return
 */
void run_worker () {

    int error = SUCCESS;

    struct sockaddr_in serv_addr = get_hello_message (&error);

    if (error != SUCCESS) {
        p_error (error);
        return;
    }

    int serv_socket = connect_to_server (serv_addr, &error);

    if (error != SUCCESS) {
        p_error (error);
        close (serv_socket);
        return;
    }

    error = do_computation (serv_socket);

    if (error != SUCCESS) {
        close (serv_socket);
        p_error (error);
        return;
    }

    //FIXME: debug
    printf ("Worker finished computation\n");

    close (serv_socket);
}


int set_keepalive (int socket, int keepcnt, int keepidle, int keepintvl) {

    int ret;
    int true = 1;

    ret = setsockopt (socket, SOL_SOCKET, SO_KEEPALIVE, &true, sizeof true);
    if (ret != 0) {
        //FIXME: debug
        perror ("setsockopt (timeout) for server socket:");
        return -1;
    }

    ret = setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
    if (ret != 0) {
        perror ("tcp_keepcnt");
        return -1;
    }

    ret = setsockopt (socket, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
    if (ret != 0) {
        perror ("tcp_keepcnt");
        return -1;
    }

    ret = setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));
    if (ret != 0) {
        perror ("tcp_keepcnt");
        return -1;
    }

    return 0;
}

void p_error (enum error err) {

    switch (err) {

        case SUCCESS:
            fprintf (stderr, "Success\n");
            break;
        case E_INVAL:
            fprintf (stderr, "Invalid arguments were passed to function\n");
            break;
        case E_SOCK:
            fprintf (stderr, "Error while creating, binding or listening the socket\n");
            break;
        case E_CONN:
            fprintf (stderr, "Error while connecting, writing or reading from socket\n");
            break;
        case E_MEM:
            fprintf (stderr, "Lack of memory\n");
            break;
        case E_THREAD:
            fprintf (stderr, "Error while creating or joining the thread\n");
            break;
        default:
            fprintf (stderr, "Unknown error\n");
    }
}
