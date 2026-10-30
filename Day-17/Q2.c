#include <stdio.h>
#include <pthread.h>

struct array
{
    int *arr;
    int size;
};

void *thread_func(void *param)
{
    struct array *a = (struct array *)param;
    int i, j, temp;

    for (i = 0; i < a->size - 1; i++)
    {
        for (j = 0; j < a->size - 1 - i; j++)
        {
            if (a->arr[j] > a->arr[j + 1])
            {
                temp = a->arr[j];
                a->arr[j] = a->arr[j + 1];
                a->arr[j + 1] = temp;
            }
        }
    }

    return NULL;
}

int main(void)
{
    pthread_t t1;
    struct array a;
    int i;

    printf("Enter number of elements: ");
    scanf("%d", &a.size);

    if (a.size <= 0)
    {
        printf("Invalid size\n");
        return 1;
    }

    int arr[a.size];
    a.arr = arr;

    printf("Enter %d elements:\n", a.size);

    for (i = 0; i < a.size; i++)
    {
        scanf("%d", &a.arr[i]);
    }

    pthread_create(&t1, NULL, thread_func, &a);

    pthread_join(t1, NULL);

    printf("Sorted array:\n");

    for (i = 0; i < a.size; i++)
    {
        printf("%d ", a.arr[i]);
    }

    printf("\n");

    return 0;
}
