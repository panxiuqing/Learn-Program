#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t sub_process_id;
    int fd[2];
    pid_t msg;

    if (pipe(fd) == -1) {
        perror("pipe error");
        exit(1);
    }
    if ((sub_process_id = fork()) == -1) {
        perror("fork error");
        exit(1);
    }
    if (sub_process_id == 0) {
        close(fd[1]);
        read(fd[0], &msg, sizeof(msg));
        printf("SubProcess Receive: %d\n", msg);
        close(fd[0]);
    }
    else {
        close(fd[0]);
        write(fd[1], &sub_process_id, sizeof(sub_process_id));
        printf("ParProcess Send: %d\n", sub_process_id);
        close(fd[1]);
    }
    return 0;
}
