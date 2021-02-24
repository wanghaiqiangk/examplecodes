/*
 * The parent process creates a child process.
 *
 * The parent process waits until the child process terminates.
 * The child process sleep for N seconds.
 */
#include <unistd.h>             /* fork(), _exit(), getpid() */
#include <sys/types.h>
#include <sys/wait.h>           /* wait() */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t cpid, w;
    int stime = 5;
    int cstatus;

    printf("Parent process identified as %ld starts!\n", (long)getpid());

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)              /* child process */
    {
        printf("Child process identified as %ld starts!\n", (long)getpid());
        printf("Child process is going to sleep for %d secs\n", stime);

        while (stime > 0)
        {
            printf("Count down %d..\n", stime);
            sleep(1);
            stime--;
        }

        _exit(EXIT_SUCCESS);
    }
    else
    {
        w = wait(&cstatus);
        if (w == -1)
        {
            perror("wait");
            exit(EXIT_FAILURE);
        }

        printf("Child process %ld terminated!\n", (long)w);

        if (WIFEXITED(cstatus))
        {
            printf("Child process %ld terminated normally with %d\n",
                   (long)w, WEXITSTATUS(cstatus));
        }
        else
        {
            printf("Child process %ld terminated abnormally.\n", (long)w);
        }

        exit(EXIT_SUCCESS);
    }

    return 0;
}

