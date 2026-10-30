
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define SIZE 5
#define COUNT 20

#define EMPTY 0
#define FULL  1
#define MUTEX 2

struct queue
{
    int arr[SIZE];
    int front;
    int rear;
};

union semun
{
    int val;
    unsigned short *array;
};

// Semaphore wait operation
void sem_wait_op(int semid, int num)
{
    struct sembuf op;

    op.sem_num = num;
    op.sem_op = -1;
    op.sem_flg = 0;

    if (semop(semid, &op, 1) == -1)
    {
        perror("semop wait");
        exit(1);
    }
}

// Semaphore signal operation
void sem_signal_op(int semid, int num)
{
    struct sembuf op;

    op.sem_num = num;
    op.sem_op = 1;
    op.sem_flg = 0;

    if (semop(semid, &op, 1) == -1)
    {
        perror("semop signal");
        exit(1);
    }
}

int main(void)
{
    int shmid, semid, i;
    struct queue *q;
    union semun arg;
    unsigned short values[3] = {SIZE, 0, 1};

    // Create shared memory
    shmid = shmget(IPC_PRIVATE, sizeof(struct queue),
                   IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    q = (struct queue *)shmat(shmid, NULL, 0);
    if (q == (void *)-1)
    {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    q->front = 0;
    q->rear = 0;

    // Create 3 semaphores: empty, full, mutex
    semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget");
        shmdt(q);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    // Initialize semaphore values
    arg.array = values;
    if (semctl(semid, 0, SETALL, arg) == -1)
    {
        perror("semctl");
        semctl(semid, 0, IPC_RMID);
        shmdt(q);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    srand((unsigned int)time(NULL));

    // Create child process
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        semctl(semid, 0, IPC_RMID);
        shmdt(q);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    if (pid == 0)
    {
        // Child: Consumer
        for (i = 0; i < COUNT; i++)
        {
            int value;

            sem_wait_op(semid, FULL);
            sem_wait_op(semid, MUTEX);

            value = q->arr[q->front];
            q->front = (q->front + 1) % SIZE;

            sem_signal_op(semid, MUTEX);
            sem_signal_op(semid, EMPTY);

            printf("Child removed: %d\n", value);
            fflush(stdout);
            sleep(1);
        }

        shmdt(q);
        _exit(0);
    }
    else
    {
        // Parent: Producer
        for (i = 0; i < COUNT; i++)
        {
            int value = rand() % 100;

            sem_wait_op(semid, EMPTY);
            sem_wait_op(semid, MUTEX);

            q->arr[q->rear] = value;
            q->rear = (q->rear + 1) % SIZE;

            printf("Parent added: %d\n", value);
            fflush(stdout);

            sem_signal_op(semid, MUTEX);
            sem_signal_op(semid, FULL);

            sleep(1);
        }

        // Wait for consumer to finish
        waitpid(pid, NULL, 0);

        // Cleanup IPC resources
        shmdt(q);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);

        printf("Queue processing completed.\n");
    }

    return 0;
}

