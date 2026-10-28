
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct message
{
    long type;
    int a;
    int b;
    int sum;
};

int main(void)
{
    int qid;
    int ret;
    struct message msg;

    // Create message queue
    qid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    if (qid == -1)
    {
        perror("msgget");
        return 1;
    }

    ret = fork();

    if (ret == -1)
    {
        perror("fork");
        msgctl(qid, IPC_RMID, NULL);
        return 1;
    }

    if (ret == 0)
    {
        // Child process
        printf("Enter two numbers: ");
        scanf("%d %d", &msg.a, &msg.b);

        msg.type = 1;

        // Send two numbers to parent
        msgsnd(qid, &msg, sizeof(msg) - sizeof(long), 0);

        // Receive sum from parent
        msgrcv(qid, &msg, sizeof(msg) - sizeof(long), 2, 0);

        printf("Sum = %d\n", msg.sum);

        exit(0);
    }
    else
    {
        // Parent process

        // Receive two numbers from child
        msgrcv(qid, &msg, sizeof(msg) - sizeof(long), 1, 0);

        msg.sum = msg.a + msg.b;

        printf("Parent calculated sum = %d\n", msg.sum);

        // Send result to child using same queue
        msg.type = 2;
        msgsnd(qid, &msg, sizeof(msg) - sizeof(long), 0);

        // Wait for child to finish
        wait(NULL);

        // Remove message queue
        msgctl(qid, IPC_RMID, NULL);

        printf("Parent process completed.\n");
    }

    return 0;
}

