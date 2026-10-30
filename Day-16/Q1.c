
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
    int val;
};

int main(void)
{
    int semid;
    pid_t pid;
    union semun arg;

    // Create semaphore
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }

    // Initial value 1: parent prints first
    arg.val = 1;

    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        semctl(semid, 0, IPC_RMID);
        exit(1);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        semctl(semid, 0, IPC_RMID);
        exit(1);
    }

    if (pid == 0)
    {
        // Child process
        struct sembuf wait_op = {0, -1, 0};
        struct sembuf signal_op = {0, 1, 0};

        while (1)
        {
            // Wait for parent
            semop(semid, &wait_op, 1);

            printf("Infotech\n");
            fflush(stdout);

            // Allow parent to print
            semop(semid, &signal_op, 1);
        }
    }
    else
    {
        // Parent process
        struct sembuf wait_op = {0, -1, 0};
        struct sembuf signal_op = {0, 1, 0};

        while (1)
        {
            // Wait for child to finish its turn
            semop(semid, &wait_op, 1);

            printf("Sunbeam\n");
            fflush(stdout);

            // Allow child to print
            semop(semid, &signal_op, 1);
        }

        wait(NULL);
        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}

