
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t create_child(char *args[])
{
    pid_t pid = fork();

    if (pid == 0)
    {
        execvp(args[0], args);

        perror("execvp failed");
        exit(127);
    }

    if (pid == -1)
        perror("fork failed");

    return pid;
}

int check_status(pid_t pid)
{
    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return 0;
    }

    if (WIFEXITED(status))
    {
        int ret = WEXITSTATUS(status);

        printf("Child PID %d exit status: %d\n",
               pid, ret);

        return ret == 0;
    }

    printf("Child PID %d terminated abnormally\n", pid);

    return 0;
}

int main(void)
{
    pid_t pid[4];
    int i, success = 1;

    char *cmd1[] = {"gcc", "-c", "circle.c", NULL};
    char *cmd2[] = {"gcc", "-c", "square.c", NULL};
    char *cmd3[] = {"gcc", "-c", "rectangle.c", NULL};
    char *cmd4[] = {"gcc", "-c", "Area.c", NULL};

    char **commands[] = {cmd1, cmd2, cmd3, cmd4};

    printf("Starting compilation...\n");

    /* Create four child processes concurrently */
    for (i = 0; i < 4; i++)
    {
        pid[i] = create_child(commands[i]);

        if (pid[i] == -1)
            success = 0;
    }

    printf("\nWaiting for compilation children...\n");

    /* Wait for all compilation children */
    for (i = 0; i < 4; i++)
    {
        if (pid[i] > 0)
        {
            if (check_status(pid[i]) == 0)
                success = 0;
        }
    }

    if (success == 0)
    {
        printf("\nCompilation failed! Linking stopped.\n");
        return 1;
    }

    printf("\nAll files compiled successfully.\n");

    /* Step 2: Linking */
    printf("\nStarting linking...\n");

    char *link_cmd[] = {
        "gcc", "-o", "program.out",
        "circle.o", "square.o",
        "rectangle.o", "Area.o", NULL
    };

    pid_t p5 = create_child(link_cmd);

    if (p5 == -1)
        return 1;

    if (check_status(p5) == 0)
    {
        printf("\nLinking failed! Execution stopped.\n");
        return 1;
    }

    printf("\nLinking successful.\n");

    /* Step 3: Execute program */
    printf("\nExecuting program.out...\n");

    char *run_cmd[] = {"./program.out", NULL};

    pid_t p6 = create_child(run_cmd);

    if (p6 == -1)
        return 1;

    if (check_status(p6) == 0)
    {
        printf("\nProgram execution failed.\n");
        return 1;
    }

    printf("\nProgram executed successfully.\n");

    return 0;
}

