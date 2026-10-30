
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#define SIZE 5
#define COUNT 10

struct shared_data
{
    int buffer[SIZE];
    int in;
    int out;

    pthread_mutex_t mutex;

    sem_t empty;
    sem_t full;
};

volatile sig_atomic_t stop = 0;

void handle_signal(int sig)
{
    stop = 1;
}

/* Wait for semaphore with timeout */
int wait_sem(sem_t *s)
{
    struct timespec ts;

    while (!stop)
    {
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;

        if (sem_timedwait(s, &ts) == 0)
            return 0;

        if (errno != ETIMEDOUT && errno != EINTR)
            return -1;
    }

    return -1;
}

/* Producer process */
void producer(struct shared_data *sh)
{
    int i;

    for (i = 1; i <= COUNT && !stop; i++)
    {
        if (wait_sem(&sh->empty) == -1)
            break;

        pthread_mutex_lock(&sh->mutex);

        sh->buffer[sh->in] = i;

        printf("Producer produced: %d\n", i);
        fflush(stdout);

        sh->in = (sh->in + 1) % SIZE;

        pthread_mutex_unlock(&sh->mutex);

        sem_post(&sh->full);

        sleep(1);
    }
}

/* Consumer process */
void consumer(struct shared_data *sh)
{
    int i, value;

    for (i = 1; i <= COUNT && !stop; i++)
    {
        if (wait_sem(&sh->full) == -1)
            break;

        pthread_mutex_lock(&sh->mutex);

        value = sh->buffer[sh->out];

        printf("Consumer consumed: %d\n", value);
        fflush(stdout);

        sh->out = (sh->out + 1) % SIZE;

        pthread_mutex_unlock(&sh->mutex);

        sem_post(&sh->empty);

        sleep(2);
    }
}

int main(void)
{
    int fd;
    pid_t pid;

    struct shared_data *sh;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /* Create shared memory */
    shm_unlink("/my_shared_memory");

    fd = shm_open("/my_shared_memory",
                  O_CREAT | O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    ftruncate(fd, sizeof(struct shared_data));

    sh = mmap(NULL, sizeof(struct shared_data),
              PROT_READ | PROT_WRITE,
              MAP_SHARED, fd, 0);

    if (sh == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        shm_unlink("/my_shared_memory");
        return 1;
    }

    close(fd);

    /* Initialize shared data */
    sh->in = 0;
    sh->out = 0;

    /* Initialize process-shared mutex */
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);

    pthread_mutexattr_setpshared(
        &attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&sh->mutex, &attr);

    pthread_mutexattr_destroy(&attr);

    /* Initialize semaphores */
    sem_init(&sh->empty, 1, SIZE);
    sem_init(&sh->full, 1, 0);

    /* Create consumer process */
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        /* Child process: Consumer */
        consumer(sh);
        munmap(sh, sizeof(struct shared_data));
        exit(0);
    }
    else
    {
        /* Parent process: Producer */
        producer(sh);

        if (stop)
            kill(pid, SIGTERM);

        waitpid(pid, NULL, 0);

        printf("\nCleaning up resources...\n");

        /* Destroy synchronization objects */
        sem_destroy(&sh->empty);
        sem_destroy(&sh->full);

        pthread_mutex_destroy(&sh->mutex);

        /* Unmap and unlink shared memory */
        munmap(sh, sizeof(struct shared_data));
        shm_unlink("/my_shared_memory");

        printf("Cleanup completed.\n");
    }

    return 0;
}

