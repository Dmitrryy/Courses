#include <errno.h>

#include <signal.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/prctl.h>

int getBit(char sim_, int num_);
void nop() {}

int cur_bit_ = 0;
int go_next_ = 0;

void receivedUSR1(int i) {
    cur_bit_ = 0;
    if (go_next_ == 1) {
        printf("err go_next_\n");
        exit(1);
    }
    go_next_ = 1;
}
void receivedUSR2(int i) {
    cur_bit_ = 1;
    if (go_next_ == 1) {
        printf("err go_next_\n");
        exit(1);
    }
    go_next_ = 1;
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
    sa.sa_handler = receivedUSR1;
    sigaction(SIGUSR1, &sa, NULL);
    sa.sa_handler = receivedUSR2;
    sigaction(SIGUSR2, &sa, NULL);

    int ppid = getpid();
    int is_parent = fork();
    if (is_parent == 0)
    {
        //is child
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
        sigset_t mask;
        sigemptyset(&mask);
        //sigdelset(&mask, SIGUSR1);
        //sigdelset(&mask, SIGUSR2);
        //sigdelset(&mask, SIGTERM);
        while(read(df_source, &cur_s, sizeof(char)) > 0)
        {
            //sleep(1);
            for(int i = 0; i < 8; i++)
            {
                sigsuspend(&mask);
                int bit = getBit(cur_s, i);                

                if (bit == 0) {
                    if (kill(ppid, SIGUSR1) == -1) {
                        printf("err\n");
                    }
                }
                else {
                    if (kill(ppid, SIGUSR2) == -1) {
                        printf("err\n");
                    }
                }
                if (go_next_ != 1) {//wrong
                    sigsuspend(&mask);
                }
                go_next_ = 0;
            }
        }
        //sleep(1);
        for(int i = 0; i < 8; i++)
        {
            kill(ppid, SIGUSR1);
            if (go_next_ != 1) {
                sigsuspend(&mask);
            }
            go_next_ = 0;
        }
        return 0;
    }
    else if (is_parent > 0)
    {
        //is parent
        sigset_t mask;
        sigemptyset(&mask);
        //sigdelset(&mask, SIGUSR1);
        //sigdelset(&mask, SIGUSR2);
        //sigdelset(&mask, SIGTERM);
        //sigdelset(&mask, SIGSTOP);
        char cur_sim = 0;
        do {
            cur_sim = 0;
            for (int i = 0; i < 8; i++)
            {
                if (kill(is_parent, SIGUSR1) == -1) {
                    printf("err\n");
                }
                if (go_next_ != 1) {
                    //a signal can come here and that's it all
                    sigsuspend(&mask);
                }
                cur_sim = cur_sim | (cur_bit_ << i);
                go_next_ = 0;
                if (kill(is_parent, SIGUSR1) == -1) {
                    printf("err\n");
                }
            }
            printf ("%c", cur_sim);
            fflush(0);

        } while (cur_sim != 0);
        printf("\n");
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