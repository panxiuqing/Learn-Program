#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid;
    if ((pid = fork()) == 0) {
        printf("Process %d: %c", getpid(), 'b');
    }
    else if ((pid = fork()) == 0) {
        printf("Process %d: %c", getpid(), 'c');
    }
    else {
        printf("Process %d: %c", getpid(), 'a');
    }
    printf("\n");
    return 0;
}
