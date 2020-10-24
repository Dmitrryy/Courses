#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include <signal.h>
#include <unistd.h>

struct massage
{
    long type;
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("argc != 2\nThey are:\n");
        for(int i = 0; i < argc; i++)
        {
            printf("[%d] %s\n", i, argv[i]);
        }
        exit(11);
    }

    char *cur_end = NULL;
    long n = strtol(argv[1], &cur_end, 10);

    if (!errno && *cur_end == 0 && n > 0)
    {
        int msg_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

        if (msg_id < 0)
        {
            printf("error msgget() (errno: %d)\n", errno);
            exit(10);
        }

        pid_t pid = 0;
        for (long i = 1; i <= n; i++)
        {
            pid = fork();
            switch (pid)
            {
            case -1:
                printf("p: fork error\n");
                exit(1);

            //child
            case 0:
            {
                struct massage m;
                if (msgrcv(msg_id, &m, sizeof(m) - sizeof(long), i, MSG_NOERROR) == -1)
                {
                    printf("ch%ld: error msgrcv()\n", i);
                    exit(6);
                }
                
                printf("%ld ", i);
                fflush(0);

                m.type = n + 1;
                if (msgsnd(msg_id, &m, sizeof(m) - sizeof(long), MSG_NOERROR) == -1)
                {
                    printf("ch%ld: error msgsnd()\n", i);
                    exit(7);
                }
                
                return 0;
            } //case 0
            } //switch(pid)
        }

        for (long k = 1; k <= n; k++)
        {
            struct massage m;
            m.type = k;
            if (msgsnd(msg_id, &m, sizeof(m) - sizeof(long), MSG_NOERROR) == -1)
            {
                printf("k = %ld: error msgsnd()\n", k);
                exit(5);
            }

            if (msgrcv(msg_id, &m, sizeof(m) - sizeof(long), n + 1, MSG_NOERROR) == -1)
            {
                printf("k = %ld: error msgrcv()\n", k);
                exit(6);
            }
            //wait(NULL);
        }   

        if (msgctl(msg_id, IPC_RMID, NULL) != 0)
        {
            printf("p: error msgctl()\n");
            exit(12);
        }
        printf("\n");
    }
    else
    {
        printf("nop\n");
    }

    return 0;
}
