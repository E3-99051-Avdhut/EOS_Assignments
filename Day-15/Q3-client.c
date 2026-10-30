
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
    int sfd;
    int a, b, sum;
    struct sockaddr_in addr;

    // Create TCP socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        perror("socket");
        exit(1);
    }

    // Configure server address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sfd, (struct sockaddr *)&addr,
                sizeof(addr)) == -1)
    {
        perror("connect");
        close(sfd);
        exit(1);
    }

    // Accept two numbers
    printf("Enter two numbers: ");
    if (scanf("%d %d", &a, &b) != 2)
    {
        printf("Invalid input.\n");
        close(sfd);
        return 1;
    }

    // Send numbers to server
    if (write(sfd, &a, sizeof(a)) != sizeof(a) ||
        write(sfd, &b, sizeof(b)) != sizeof(b))
    {
        perror("write");
        close(sfd);
        return 1;
    }

    // Receive sum
    if (read(sfd, &sum, sizeof(sum)) != sizeof(sum))
    {
        printf("Failed to receive sum.\n");
        close(sfd);
        return 1;
    }

    printf("Sum = %d\n", sum);

    close(sfd);

    return 0;
}

