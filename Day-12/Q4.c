
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

void sigchld_handler(int sig)
{
    int status;

    /* Clean up all completed background children */
    while (waitpid(-1, &status, WNOHANG) > 0)
    {
        printf("\nBackground child completed\n");
        fflush(stdout);
    }
}

int main(void)
{
    char command[100];
    char *args[10];
    int i, background;
    pid_t pid;

    /* Handle Ctrl+C */
    signal(SIGINT, SIG_IGN);

    /* Handle completed background processes */
    signal(SIGCHLD, sigchld_handler);

    while (1)
    {
        printf("\nMyShell$ ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        command[strcspn(command, "\n")] = '\0';

        if (command[0] == '\0')
            continue;

        if (strcmp(command, "exit") == 0)
            break;

        /* Check for & */
        background = 0;

        int len = strlen(command);

        if (len > 0 && command[len - 1] == '&')
        {
            background = 1;
            command[len - 1] = '\0';
        }

        /* Tokenize command */
        i = 0;

        char *token = strtok(command, " ");

        while (token != NULL && i < 9)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        pid = fork();

        if (pid == 0)
        {
            /* Child process */
            signal(SIGINT, SIG_DFL);
            signal(SIGCHLD, SIG_DFL);

            execvp(args[0], args);

            perror("execvp");
            exit(1);
        }
        else if (pid > 0)
        {
            if (background)
            {
                printf("Background process started. PID = %d\n", pid);
            }
            else
            {
                int status;

                waitpid(pid, &status, 0);

                if (WIFEXITED(status))
                    printf("Child exit status: %d\n",
                           WEXITSTATUS(status));

                else if (WIFSIGNALED(status))
                    printf("Child terminated by signal: %d\n",
                           WTERMSIG(status));
            }
        }
        else
        {
            perror("fork");
        }
    }

    return 0;
}

