#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int turn = 0;

void *sunbeam(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (turn != 0)
            pthread_cond_wait(&cond, &mutex);

        printf("SUNBEAM\n");
        fflush(stdout);

        turn = 1;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *infotech(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (turn != 1)
            pthread_cond_wait(&cond, &mutex);

        printf("INFOTECH\n");
        fflush(stdout);

        turn = 0;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, sunbeam, NULL);
    pthread_create(&t2, NULL, infotech, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
