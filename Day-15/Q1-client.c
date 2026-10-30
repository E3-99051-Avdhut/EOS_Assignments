#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    int a, b, sum;

    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);

    // Send numbers to server
    fd = open("client_fifo", O_WRONLY);
    write(fd, &a, sizeof(a));
    write(fd, &b, sizeof(b));
    close(fd);

    // Receive sum from server
    fd = open("server_fifo", O_RDONLY);
    read(fd, &sum, sizeof(sum));
    close(fd);

    printf("Sum = %d\n", sum);

    return 0;
}

