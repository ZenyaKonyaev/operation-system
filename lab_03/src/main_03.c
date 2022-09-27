#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ERROR_FORK -1
#define CHILD_FORK 0

int main() 
{
    int pidsChilds[2];
    char* cmds[2] = { "pwd", "ls" };

    printf("text from parent (start code):\npid = %d\ngroup = %d\n\n", getpid(), getpgrp());

    for (int i = 0; i < 2; ++i) {
        int pid = fork();

        if (pid == ERROR_FORK) {
            printf("Error at fork\n");
            return 1;
        } 
        else if (pid == CHILD_FORK) {
            sleep(1 * (i + 1));
            printf("text from %d child:\npid=%d\nppid=%d\ngroup=%d\n\n", i + 1, getpid(), getppid(), getpgrp());
            
            if (execlp(cmds[i], cmds[i], 0) == -1) 
            {
                printf("Error at exec\n");
                return 1;
            } 

            
            return 0;
        } 
        else
            pidsChilds[i] = pid;
    }
    
    for (int i = 0; i < 2; i++) 
    {
        int status;

        pid_t childpid = wait(&status);
        printf("text from parent (child process finished):\nchildPid = %d\nstatus=%d\n", childpid, status);

        if (WIFEXITED(status)) 
        {
            printf("%d child:\nfinished with code=%d\n\n", i + 1, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("%d child:\nfinished from signal with code=%d\n\n", i + 1, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("%d child:\nfinished stopped with code=%d\n\n", i + 1, WSTOPSIG(status));
        }
    }

    fprintf(stdout, "text from parent (end code):\npidChild1=%d\npidChild2=%d\n\n", pidsChilds[0], pidsChilds[1]);

    return 0;
}
