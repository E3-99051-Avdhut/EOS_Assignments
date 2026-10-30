#include <stdio.h>
#include <pthread.h>

int start, end, sum = 0;

void *thread_func(void *param)
{
    int i;

    for (i = start; i <= end; i++)
    {
        sum = sum + i;
    }

    return NULL;
}

int main(void)
{
    pthread_t t1;

    printf("Enter start number: ");
    scanf("%d", &start);

    printf("Enter end number: ");
    scanf("%d", &end);

    pthread_create(&t1, NULL, thread_func, NULL);

    pthread_join(t1, NULL);

    printf("Sum of integers = %d\n", sum);

    return 0;
}
