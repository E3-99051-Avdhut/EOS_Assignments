#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include<string.h>
int main(void)
{
    char *args[10];
    char command[100];
    int i, status;

    /* Ignore SIGINT in shell */
    signal(SIGINT, SIG_IGN);

    while (1)
    {
        printf("\nMyShell$ ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        /* Remove newline */
        command[strcspn(command, "\n")] = '\0';

        if (command[0] == '\0')
            continue;

        if (strcmp(command, "exit") == 0)
            break;

        /* Prepare command arguments */
        i = 0;
        char *token = strtok(command, " ");

        while (token != NULL && i < 9)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        pid_t pid = fork();

        if (pid == 0)
        {
            /* Child should respond normally to Ctrl+C */
            signal(SIGINT, SIG_DFL);

            execvp(args[0], args);

            perror("execvp");
            exit(1);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);

            if (WIFEXITED(status))
                printf("Child exit status: %d\n",
                       WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                printf("Child terminated by signal: %d\n",
                       WTERMSIG(status));
        }
        else
        {
            perror("fork");
        }
    }

    printf("Shell terminated normally.\n");

    return 0;
}

