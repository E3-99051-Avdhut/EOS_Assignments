#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    int ret, status;
    pid_t pid;

    // Process A
    printf("Process A started. PID = %d\n", getpid());
    fflush(stdout);

    ret = fork();

    if (ret == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ret == 0)
    {
        // Process B
        printf("Process B started. PID = %d, PPID = %d\n",
               getpid(), getppid());
        fflush(stdout);

        ret = fork();

        if (ret == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (ret == 0)
        {
            // Process C
            printf("Process C started. PID = %d, PPID = %d\n",
                   getpid(), getppid());
            fflush(stdout);

            ret = fork();

            if (ret == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (ret == 0)
            {
                // Process D
                printf("Process D started. PID = %d, PPID = %d\n",
                       getpid(), getppid());
                fflush(stdout);

                for (int count = 1; count <= 5; count++)
                {
                    printf("Process D: PID = %d, Count = %d\n",
                           getpid(), count);
                    sleep(1);
                }

                printf("Process D terminating.\n");
                exit(EXIT_SUCCESS);
            }
            else
            {
                // Process C waits for D
                for (int count = 1; count <= 5; count++)
                {
                    printf("Process C: PID = %d, Count = %d\n",
                           getpid(), count);
                    sleep(1);
                }

                pid = wait(&status);
                printf("Process C: Child D (PID = %d) cleaned up.\n",
                       pid);

                exit(EXIT_SUCCESS);
            }
        }
        else
        {
            // Process B waits for C
            for (int count = 1; count <= 5; count++)
            {
                printf("Process B: PID = %d, Count = %d\n",
                       getpid(), count);
                sleep(1);
            }

            pid = wait(&status);
            printf("Process B: Child C (PID = %d) cleaned up.\n",
                   pid);

            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        // Process A
        for (int count = 1; count <= 5; count++)
        {
            printf("Process A: PID = %d, Count = %d\n",
                   getpid(), count);
            sleep(1);
        }

        pid = wait(&status);
        printf("Process A: Child B (PID = %d) cleaned up.\n",
               pid);

        printf("Process A terminating.\n");
    }

    return 0;
}
