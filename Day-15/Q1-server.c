
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
    int fd;
    int a, b, sum;

    // Create two FIFOs
    mkfifo("client_fifo", 0666);
    mkfifo("server_fifo", 0666);

    printf("Server waiting for client...\n");

    // Read numbers from client
    fd = open("client_fifo", O_RDONLY);
    read(fd, &a, sizeof(a));
    read(fd, &b, sizeof(b));
    close(fd);

    printf("Server received: %d and %d\n", a, b);

    // Calculate sum
    sum = a + b;

    // Send result to client
    fd = open("server_fifo", O_WRONLY);
    write(fd, &sum, sizeof(sum));
    close(fd);

    printf("Server sent sum: %d\n", sum);

    return 0;
}

