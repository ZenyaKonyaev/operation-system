#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ERROR_FORK -1
#define CHILD_FORK 0

int main() 
{
    int pidsChilds[2];
    char *msgs[2] = { "Message from first child\n", "Message from second child\n" };
    int fd[2];
    pipe(fd);

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
            
            close(fd[0]);
            write(fd[1], msgs[i], strlen(msgs[i])); 
            
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
    
    char buf[256];
    close(fd[1]);
    read(fd[0], buf, 256);
    printf("text from childs to parent:\n%s\n", buf);

    printf("text from parent (end code):\npidChild1=%d\npidChild2=%d\n\n", pidsChilds[0], pidsChilds[1]);

    return 0;
}
