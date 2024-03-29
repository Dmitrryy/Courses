
//for splice
#define _GNU_SOURCE

#include "CircleBuffer.h"

#include <math.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>


#define MAX(a, b) ((a > b) ? a : b)


struct ChannelInfo {
    int fd_from, fd_to;

    CircleBuffer buffer;
};


void ChildSigHandler(int sig_);
void free_all(struct ChannelInfo* arr, size_t n);


int main(int argc, char* argv[])
{
    ///////////////////////////////////////////////////////////////
    ///command line arguments
    if (argc != 3) {
        printf("invalid arguments command line:\n argc = %d\n", argc);
        printf("They are:\n");
        for (int i = 0; i < argc; i++)
        {
            printf ("[%d] %s\n", i, argv[i]);
        }
        exit(1);
    }

    char* source_file_name = argv[1];
    long n = 0;
    {
        char *cur_end = NULL;
        n = strtol(argv[2], &cur_end, 10);
        if (errno || *cur_end != 0 || n <= 0) {
            printf("incorrect number of children: %s\n", argv[2]);
            exit(2);
        }
    }
    ///
    ///////////////////////////////////////////////////////////////

    {
        struct sigaction sa = {};
        sa.sa_handler = ChildSigHandler;

        sigaction(SIGCHLD, &sa, NULL);
    }


    struct ChannelInfo *channels_info = (struct ChannelInfo*) calloc (n, sizeof (struct ChannelInfo));
    assert(channels_info != NULL);

    for (long i = 0; i < n; i++)
    {
        int fdp_to[2] = {};
        pipe(fdp_to);

        int fdp_from[2] = {};
        pipe(fdp_from);

        pid_t ppid = getpid();
        pid_t pid = fork();

        if (pid < 0)
        {
            printf("error in fork! (i = %ld, n = %ld)", i, n);
            exit(5);
        }

        if (pid == 0)
        {// child

            for (int j = 0; j < i; ++j) {
                close (channels_info[j].fd_from);
                close (channels_info[j].fd_to);
            }

            if (prctl(PR_SET_PDEATHSIG, SIGTERM) == -1)
            {
                printf("ch: error prctl()\n");
                exit(8);
            }
            if (ppid != getppid())
            {
                printf("ch: parent is dead\n");
                exit(6);
            }

            int fd_to = -1, fd_from = -1;
            if (i == 0)
            {//first child
                close(fdp_to[0]);
                close(fdp_to[1]);
                close(fdp_from[0]);

                fd_to = fdp_from[1];
                fd_from = open(source_file_name, O_RDONLY);
                if (fd_from <= 0) {
                    printf("can't open a file: %s\n", source_file_name);
                    exit(666);
                }
            }
            else
            {//non first child
                close(fdp_from[0]);
                close(fdp_to[1]);

                fd_to = fdp_from[1];
                fd_from = fdp_to[0];
            }

            ///transfer of resources in the child
            ssize_t splice_res = 0;
            do {
                splice_res = splice(fd_from, NULL, fd_to, NULL, 10000, SPLICE_F_MOVE);
            } while(splice_res > 0);

            if (splice_res < 0) {
                printf("error in splice\n");
                exit(111);
            }

            close(fd_to);
            close(fd_from);

            return 0;
        }
        else
        {// parent
            if (cbCreate((size_t)(pow(3, n - i) * 1024), &(channels_info[i].buffer)) != CB_SUCCESS)
            {
                printf("cant create buffer!\n");
                exit(4);
            }

            channels_info[i].fd_to = -1;
            channels_info[i].fd_from = -1;


            close(fdp_to[0]);
            close(fdp_from[1]);
            if(i == 0)
            {
                close(fdp_to[1]);

                channels_info[0].fd_from = fdp_from[0];

                if (n == 1) {
                    channels_info[0].fd_to = STDOUT_FILENO;
                }
            }
            else
            {
                channels_info[i - 1].fd_to = fdp_to[1];

                channels_info[i].fd_from = fdp_from[0];

                if (i == n - 1)
                {
                    channels_info[i].fd_to = STDOUT_FILENO;
                }
            }
        }//end parent case
    }


    for (long i = 0; i < n; i++)
    {
        int res = fcntl(channels_info[i].fd_from, F_SETFL, O_NONBLOCK);
        if (res < 0) {
            printf("fcntl for read error\n");
            exit(32);
        }

        res = fcntl(channels_info[i].fd_to, F_SETFL, O_NONBLOCK);
        if (res < 0) {
            printf("fcntl for write error\n");
            exit(33);
        }
    }


    fd_set rfds = {};
    fd_set wfds = {};
    long last_alive = 0;
    while(channels_info[n - 1].fd_from >= 0)
    {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);

        int nSelect = -1;
        for (int i = 0; i < n; i++)
        {
            if (channels_info[i].fd_from >= 0 && !cbIsFull(channels_info[i].buffer)) {
                FD_SET(channels_info[i].fd_from, &rfds);

                nSelect = MAX(nSelect, channels_info[i].fd_from);
            }

            if (channels_info[i].fd_to >= 0 && !cbIsEmpty(channels_info[i].buffer)) {
                FD_SET(channels_info[i].fd_to, &wfds);

                nSelect = MAX(nSelect, channels_info[i].fd_to);
            }
        }

        if (select(nSelect + 1, &rfds, &wfds, NULL, NULL) < 0)
        {
            printf("select error\n");
            exit(11);
        }

        for (long i = last_alive; i < n; i++)
        {

            if (FD_ISSET(channels_info[i].fd_from, &rfds))
            {
                ////
                int read_res = 0;
                if (cbReadFromFD(channels_info[i].buffer, channels_info[i].fd_from, cbGetSize(channels_info[i].buffer), &read_res) != CB_SUCCESS) {
                    printf("error read!\n");
                    exit(10);
                }

                if (read_res < 0)
                {
                    perror("error in parent read");
                    exit(EXIT_FAILURE);
                }
                if (read_res == 0)
                {
                    close(channels_info[i].fd_from);
                    channels_info[i].fd_from = -1;
                }
                ////
            }


            if (FD_ISSET(channels_info[i].fd_to, &wfds))
            {
                ////
                int write_res = 0;
                if (cbWriteToFD(channels_info[i].buffer, channels_info[i].fd_to, cbGetSize(channels_info[i].buffer), &write_res) != CB_SUCCESS) {
                    printf("error read!\n");
                    exit(10);
                }

                if (write_res < 0)
                {
                    printf("error write!\n");
                    free_all(channels_info, n);
                    exit(EXIT_FAILURE);
                }

                ////
            }

            if (channels_info[i].fd_from < 0 && cbIsEmpty(channels_info[i].buffer))
            {
                if (i != last_alive)
                {
                    printf("some child died too early!\n");
                    exit(132);
                }

                close(channels_info[i].fd_to);
                channels_info[i].fd_to = -1;
                last_alive++;
            }
        }
    }

    free_all(channels_info, n);

    return 0;
}


void ChildSigHandler(int sig_)
{
    if (sig_ == SIGCHLD)
    {
        int status__ = 0;
        wait(&status__);

        if (status__ != 0) {
            printf("one of the children died\n");
            exit(4);
        }
    }
}


void free_all(struct ChannelInfo* arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        cbDestroy(arr[i].buffer);
        arr[i].buffer = NULL;
    }
    free(arr);
}
