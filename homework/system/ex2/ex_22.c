#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    if ((pid = fork()) == -1) {
        printf("Fork error\n");
        exit(1);
    }
    else if (pid == 0) {
        if (execl("/bin/ls", ".", NULL) < 0) {
            printf("Process %d: execl error!\n", getpid());
        }
        exit(0);
    }
    else {
        waitpid(pid, NULL);
        printf("%d\n", pid);
    }
    return 0;
}
