#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    int ret;

    ret = fork();

    if (ret == -1)
    {
        perror("fork failed");
        return 1;
    }

    if (ret == 0)
    {
        // Child process
        execlp("firefox", "firefox",
               "https://www.google.com", NULL);

        perror("execlp failed");
    }
    else
    {
        // Parent process
        printf("Parent process: Browser started by child\n");
    }

    return 0;
}
