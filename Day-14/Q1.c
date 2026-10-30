#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
    int fd[2];
    int ret;
    int count = 0;
    char buf[1024];

    // Create pipe
    ret = pipe(fd);

    if (ret == -1)
    {
        perror("pipe");
        exit(1);
    }

    // Set write end to non-blocking mode
    ret = fcntl(fd[1], F_SETFL, O_NONBLOCK);

    if (ret == -1)
    {
        perror("fcntl");
        exit(1);
    }

    // Fill buffer with data
    while (1)
    {
        ret = write(fd[1], buf, sizeof(buf));

        if (ret == -1)
        {
            perror("write");
            break;
        }

        count += ret;
    }

    printf("Pipe buffer size = %d bytes\n", count);

    close(fd[0]);
    close(fd[1]);

    return 0;
}
