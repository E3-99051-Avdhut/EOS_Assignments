
#include <stdio.h>
#include <pthread.h>

#define SIZE 10

void *thread_func(void *param)
{
    int *arr = (int *)param;
    int i, j, min, temp;

    // Selection sort
    for (i = 0; i < SIZE - 1; i++)
    {
        min = i;

        for (j = i + 1; j < SIZE; j++)
        {
            if (arr[j] < arr[min])
            {
                min = j;
            }
        }

        // Swap elements
        if (min != i)
        {
            temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }

    return NULL;
}

int main(void)
{
    pthread_t tid;
    int arr[SIZE];
    int i;

    printf("Enter 10 integers:\n");

    for (i = 0; i < SIZE; i++)
    {
        scanf("%d", &arr[i]);
    }

    // Pass array to thread
    pthread_create(&tid, NULL, thread_func, arr);

    // Wait for sorting to complete
    pthread_join(tid, NULL);

    // Print sorted array
    printf("Sorted array:\n");

    for (i = 0; i < SIZE; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    return 0;
}

