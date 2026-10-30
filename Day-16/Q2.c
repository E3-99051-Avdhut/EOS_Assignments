
#include <stdio.h>
#include <pthread.h>

int start, end, sum = 0;

void *calculate_sum(void *arg)
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

    if (start > end)
    {
        printf("Invalid range!\n");
        return 1;
    }

    // Create thread
    pthread_create(&t1, NULL, calculate_sum, NULL);

    // Wait for thread to complete
    pthread_join(t1, NULL);

    // Print result in main
    printf("Sum = %d\n", sum);

    return 0;
}

