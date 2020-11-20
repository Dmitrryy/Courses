#include <errno.h>

#include <signal.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/prctl.h>

int getBit(char sim_, int num_);
void nop() {}

int cur_bit_ = 0;

#define NDEBUG

void SigHandler(int sig_)
{
    switch(sig_)
    {
        case SIGUSR1:
            cur_bit_ = 0;
            break;

        case SIGUSR2:
            cur_bit_ = 1;
            break;

        case SIGCHLD: {
            int status__ = 0;
            wait(&status__);
            if (status__ != 0) {
                printf("child died\n");
                exit(1);
            }
            break;
        }

        case SIGTERM:
            printf("terminate\n");
            exit(2);
    }
}

//it doesn't work correctly
int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf("invalid arguments command line:\n argc = %d\n", argc);
        printf("They are:\n");
        for (int i = 0; i < argc; i++)
        {
            printf ("[%d] %s\n", i, argv[i]);
        }
        exit(1);
    }


    int df_source = open(argv[1], O_RDONLY);
    if (df_source == -1)
    {
        printf ("error: failed open source file: %s\n", argv[1]);
        exit(2);
    }

    struct sigaction sa = {};
    sa.sa_handler = SigHandler;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    sigset_t sproc_mask;
    sigemptyset(&sproc_mask);
    sigaddset(&sproc_mask, SIGUSR1);
    sigaddset(&sproc_mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &sproc_mask, NULL);

    int ppid = getpid();
    int is_parent = fork();
    if (is_parent == 0)
    {//is child
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

        char cur_s = 0;

        sigset_t ssus_mask = {};
        sigfillset(&ssus_mask);
        sigdelset(&ssus_mask, SIGUSR1);
        sigdelset(&ssus_mask, SIGTERM);

        char cont = 1;
        while(1)
        {
            cont = read(df_source, &cur_s, sizeof(char));
            if (cont <= 0) {
                kill(ppid, SIGUSR1);
                break;
            }
            else {
                kill(ppid, SIGUSR2);
            }
            sigsuspend(&ssus_mask);

            for (int i = 0; i < 8; i++)
            {
                int bit = getBit(cur_s, i);

                if (bit == 0) {
                    kill(ppid, SIGUSR1);
                }
                else {
                    kill(ppid, SIGUSR2);
                }
                sigsuspend(&ssus_mask);
            }//for

        }//while

        return 0;
    }
    else if (is_parent > 0)
    {//is parent
        sigset_t ssus_mask = {};
        sigfillset(&ssus_mask);
        sigdelset(&ssus_mask, SIGUSR1);
        sigdelset(&ssus_mask, SIGUSR2);
        sigdelset(&ssus_mask, SIGTERM);
        sigdelset(&ssus_mask, SIGSTOP);
        sigdelset(&ssus_mask, SIGCHLD);

        char cur_sim = 0;
        while(1) {
            cur_sim = 0;

            sigsuspend(&ssus_mask);
            if (cur_bit_ == 0) {
                //the child announced the end of the transmission
                break;
            }
            kill(is_parent, SIGUSR1);

            for (int i = 0; i < 8; i++)
            {
                sigsuspend(&ssus_mask);
                kill(is_parent, SIGUSR1);

                cur_sim = cur_sim | (cur_bit_ << i);
            }

            printf("%c", cur_sim);
            fflush(0);
        }


        waitpid(is_parent, NULL, 0);
    }
    else
    {
        printf("fork error\n");
        exit(1);
    }


    return 0;
}


int getBit(char sim_, int num_)
{
    if (num_ > 7 || num_ < 0) { return 0;}
    return (sim_ & (1 << num_)) != 0;
}
