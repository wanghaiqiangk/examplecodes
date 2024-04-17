#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd1 = open("./tmp1", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd1 == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    int fd2 = open("./tmp2", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd2 == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int ret = dup2(fd1, fd2);
    if (ret < 0)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    const char *buf1 = "write to tmp1\n";
    const char *buf2 = "write to tmp2\n";
    write(fd1, buf1, strlen(buf1));
    write(fd2, buf2, strlen(buf2));

    close(fd1);
    close(fd2);

    exit(EXIT_SUCCESS);
}
