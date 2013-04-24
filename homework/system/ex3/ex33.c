#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

struct msgbuf {
    long mtype;
    char mtext[2];
};

int main()
{
    pid_t client_pid, server_pid;
    key_t MSGKEY = 75;
    int msgqid;

    if ((server_pid = fork()) < 0) {
        perror("fork client error");
        exit(1);
    }
    if (server_pid == 0) {
        struct msgbuf msgp;
        if ((msgqid = msgget(MSGKEY, IPC_CREAT)) == -1) {
            perror("msgget error");
            exit(1);
        }
        sleep(2);
        while (1) {
            msgrcv(msgqid, &msgp, 2, 0, 0);
            if (msgp.mtype == 1) {
                break;
            }
            printf("Server has received message from Client!\n");
        }
        msgctl(msgqid, IPC_RMID, 0);
    }
    else {
        if ((client_pid = fork()) < 0) {
            perror("fork server error");
            exit(1);
        }
        if (client_pid == 0) {
            sleep(1);
            msgqid = msgget(MSGKEY, IPC_CREAT);
            int i = 0;
            struct msgbuf msgp;
            msgp.mtext[0] = 'j';
            msgp.mtext[1] = 0;
            for (i = 10; i > 0; i--) {
                msgp.mtype = i;
                msgsnd(msgqid, &msgp, 2, 0);
                printf("Client has sent message to Server!\n");
            }
            printf("%ld\n", sizeof(struct msgbuf));
        }
        else {
            waitpid(client_pid, NULL, 0);
            waitpid(server_pid, NULL, 0);
        }
    }
}
