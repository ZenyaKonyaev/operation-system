#include <stdio.h>
#include <unistd.h>

#define ERROR_FORK -1
#define CHILD_FORK 0

int main() 
{
    int pidsChilds[2];

    printf("text from parent (start code):\npid = %d\ngroup = %d\n\n", getpid(), getpgrp());

    for (int i = 0; i < 2; ++i) {
        int pid = fork();

        if (pid == ERROR_FORK) {
            printf("Error at fork\n");
            return 1;
        } 
        else if (pid == CHILD_FORK) {
            sleep(1 * i);
            printf("text from %d child:\npid=%d\nppid=%d\ngroup=%d\n\n", i + 1, getpid(), getppid(), getpgrp());
            return 0;
        } 
        else
            pidsChilds[i] = pid;
    }

    fprintf(stdout, "text from parent (end code):\npidChild1=%d\npidChild2=%d\n\n", pidsChilds[0], pidsChilds[1]);

    return 0;
}
