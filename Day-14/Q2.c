#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    int arr[2];
    int pid1, pid2;
    int s1, s2;

    // Create pipe
    if (pipe(arr) == -1)
    {
        perror("pipe");
        exit(1);
    }

    // Create first child process: who
    pid1 = fork();

    if (pid1 == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0)
    {
        // Writer process
        close(arr[0]);

        // Redirect stdout to pipe
        dup2(arr[1], 1);

        close(arr[1]);

        execlp("who", "who", NULL);

        perror("execlp who");
        exit(1);
    }

    // Create second child process: wc
    pid2 = fork();

    if (pid2 == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid2 == 0)
    {
        // Reader process
        close(arr[1]);

        // Redirect stdin from pipe
        dup2(arr[0], 0);

        close(arr[0]);

        execlp("wc", "wc", NULL);

        perror("execlp wc");
        exit(1);
    }

    // Parent process
    close(arr[0]);
    close(arr[1]);

    // Wait for both child processes
    waitpid(pid1, &s1, 0);
    waitpid(pid2, &s2, 0);

    return 0;
}
