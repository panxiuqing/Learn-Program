/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

void echo(int connfdv[]);

int main(int argc, char **argv) 
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    int connfdv[20], i = 1;

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfdv[i] = Accept(listenfd, (SA *)&clientaddr, &clientlen);

	/* determine the domain name and IP address of the client */
	hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			   sizeof(clientaddr.sin_addr.s_addr), AF_INET);
	haddrp = inet_ntoa(clientaddr.sin_addr);
	printf("server connected to %s (%s)\n", hp->h_name, haddrp);	
	connfdv[0] = i;
	new_thread(connfdv);
	//	new_thread(connfd);
	//Close(connfd);
	i++;
    }
    exit(0);
}

int new_thread(int connfdv[])
{
  pthread_t pid;
  pthread_create(&pid, NULL, echo, connfdv);
  //Close(connfd[i]);
}

void echo(int connfdv[]) 
{
  size_t n;
  char buf[MAXLINE], *user, control[12]; 
  rio_t rio;
  int i = 0, num = connfdv[0];
  
  Rio_readinitb(&rio, connfdv[connfdv[0]]);
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
    printf("server received %d bytes\n", n);

    puts(buf);
    for (i = 1; i <= connfdv[0]; i++) {
	Rio_writen(connfdv[i], buf, n);
    }
  }
}
/* $end echoserverimain */
