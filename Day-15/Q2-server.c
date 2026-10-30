
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 1234

struct message
{
    long type;
    char fifo[128];
    char filename[128];
};

int main(void)
{
    int qid, fd, pfd, n, status;
    char buf[1024];
    FILE *fp;
    struct message msg;

    // Create message queue
    qid = msgget(KEY, IPC_CREAT | 0666);
    if (qid == -1)
    {
        perror("msgget");
        exit(1);
    }

    printf("Server waiting for request...\n");

    // Receive FIFO name and filename
    if (msgrcv(qid, &msg, sizeof(msg) - sizeof(long),
               1, 0) == -1)
    {
        perror("msgrcv");
        exit(1);
    }

    printf("FIFO: %s\n", msg.fifo);
    printf("File: %s\n", msg.filename);

    // Open requested file
    fp = fopen(msg.filename, "rb");
    status = (fp == NULL) ? -1 : 0;

    // Open FIFO for writing
    pfd = open(msg.fifo, O_WRONLY);
    if (pfd == -1)
    {
        perror("open FIFO");
        if (fp != NULL)
            fclose(fp);
        exit(1);
    }

    // Send file-open status
    write(pfd, &status, sizeof(status));

    if (fp == NULL)
    {
        perror("fopen");
        close(pfd);
        return 1;
    }

    // Send file contents through FIFO
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
    {
        if (write(pfd, buf, n) != n)
        {
            perror("write FIFO");
            break;
        }
    }

    fclose(fp);
    close(pfd);

    printf("File transfer completed.\n");

    return 0;
}

