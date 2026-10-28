#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    int i, ret, status;

    printf("Parent Process Started. PID = %d\n", getpid());

    // Create 5 child processes
    for (i = 0; i < 5; i++)
    {
        ret = fork();

        if (ret == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (ret == 0)
        {
            // Child process
            int count = 1;

            printf("Child Process Started. PID = %d\n", getpid());

            while (count <= 5)
            {
                printf("PID = %d, Count = %d\n", getpid(), count);
                sleep(1);
                count++;
            }

            printf("Child PID = %d Completed\n", getpid());

            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for all 5 children
    for (i = 0; i < 5; i++)
    {
        ret = wait(&status);

        if (ret > 0)
        {
            printf("Parent: Child PID = %d terminated\n", ret);
        }
    }

    printf("Parent Process Completed. PID = %d\n", getpid());

    return 0;
}

