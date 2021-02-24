/*
 * Two processes communicate with each other using pipe.
 */

#include <unistd.h>             /* pipe, fork, write */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t cpid;
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == (pid_t)0)       /* child process */
    {
        close(fd[0]);
        char msg[4096] = "Message through pipe";
        if (write(fd[1], msg, sizeof(msg)) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(fd[1]);
        _exit(EXIT_SUCCESS);
    }
    else
    {
        close(fd[1]);
        int stime = 5;
        ssize_t cnt = 0;
        char msg[4096] = {0};
        while (stime > 0)
        {
            printf("%d..\n", stime);
            stime--;
            sleep(1);
        }
        do {
            cnt = read(fd[0], msg + cnt, sizeof(msg));
            if (cnt == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
        } while (cnt > 0);
        close(fd[0]);
        printf("Msg: %s\n", msg);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
