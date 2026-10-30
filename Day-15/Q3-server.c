
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
    int sfd, cfd;
    int a, b, sum;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

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
    addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to IP and port
    if (bind(sfd, (struct sockaddr *)&addr,
             sizeof(addr)) == -1)
    {
        perror("bind");
        close(sfd);
        exit(1);
    }

    // Listen for client
    if (listen(sfd, 5) == -1)
    {
        perror("listen");
        close(sfd);
        exit(1);
    }

    printf("Server waiting for client...\n");

    // Accept client connection
    cfd = accept(sfd, (struct sockaddr *)&addr, &len);
    if (cfd == -1)
    {
        perror("accept");
        close(sfd);
        exit(1);
    }

    // Receive two numbers
    if (read(cfd, &a, sizeof(a)) != sizeof(a) ||
        read(cfd, &b, sizeof(b)) != sizeof(b))
    {
        printf("Failed to receive numbers.\n");
        close(cfd);
        close(sfd);
        return 1;
    }

    printf("Received numbers: %d and %d\n", a, b);

    // Calculate sum
    sum = a + b;

    // Send sum to client
    if (write(cfd, &sum, sizeof(sum)) != sizeof(sum))
    {
        perror("write");
    }
    else
    {
        printf("Sum sent: %d\n", sum);
    }

    close(cfd);
    close(sfd);

    return 0;
}

