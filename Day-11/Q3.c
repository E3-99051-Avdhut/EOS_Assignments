
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    int count = 0;
    int ret;

    while (1)
    {
        ret = fork();

        if (ret == -1)
        {
            printf("fork failed\n");
            break;
        }

        if (ret == 0)
        {
            // Child process
            while (1)
                pause();
        }
        else
        {
            // Parent process
            count++;
        }
    }

    printf("Maximum child processes created = %d\n", count);

    return 0;
}

