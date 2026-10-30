
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define KEY 1234

struct message
{
    long type;
    char fifo[128];
    char filename[128];
};

int main(void)
{
    int qid, pfd, n, status;
    char source[128], dest[128];
    char buf[1024];
    FILE *fp;
    struct message msg;

    // Get filenames
    printf("Enter server file name: ");
    scanf("%127s", source);

    printf("Enter destination file name: ");
    scanf("%127s", dest);

    // Create FIFO
    strcpy(msg.fifo, "/tmp/client_fifo");
    if (mkfifo(msg.fifo, 0666) == -1)
    {
        // Continue if FIFO already exists
        if (access(msg.fifo, F_OK) == -1)
        {
            perror("mkfifo");
            exit(1);
        }
    }

    // Open message queue
    qid = msgget(KEY, 0666);
    if (qid == -1)
    {
        perror("msgget - start server first");
        unlink(msg.fifo);
        exit(1);
    }

    // Prepare request
    msg.type = 1;
    strcpy(msg.filename, source);

    // Send FIFO name and filename
    if (msgsnd(qid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
    {
        perror("msgsnd");
        unlink(msg.fifo);
        exit(1);
    }

    // Open FIFO for reading
    pfd = open(msg.fifo, O_RDONLY);
    if (pfd == -1)
    {
        perror("open FIFO");
        unlink(msg.fifo);
        exit(1);
    }

    // Receive file status
    if (read(pfd, &status, sizeof(status)) != sizeof(status))
    {
        printf("Failed to receive file status.\n");
        close(pfd);
        unlink(msg.fifo);
        exit(1);
    }

    if (status != 0)
    {
        printf("Server could not open the requested file.\n");
        close(pfd);
        unlink(msg.fifo);
        return 1;
    }

    // Create destination file
    fp = fopen(dest, "wb");
    if (fp == NULL)
    {
        perror("fopen destination");
        close(pfd);
        unlink(msg.fifo);
        exit(1);
    }

    // Receive and save file contents
    while ((n = read(pfd, buf, sizeof(buf))) > 0)
    {
        if (fwrite(buf, 1, n, fp) != (size_t)n)
        {
            perror("fwrite");
            break;
        }
    }

    fclose(fp);
    close(pfd);
    unlink(msg.fifo);

    printf("File received and saved as %s\n", dest);

    return 0;
}

