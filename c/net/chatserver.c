/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#define MAX_CLIENT 20

struct connfd_vn
{
  int connfd; //文件标识
  char *name; //客户端登录名
};

typedef struct connfd_vn connfd_t;

struct cfd
{
  int cn; //新产生的连接编号
  int cnd; //已连接数
  int connfdv[MAX_CLIENT]; //客户端信息结构
  char *name[MAX_CLIENT];
};

typedef struct cfd cfd_t;

void echo(cfd_t *);

int main(int argc, char **argv) 
{
  int listenfd, connfd, port, clientlen;
  struct sockaddr_in clientaddr;
  struct hostent *hp;
  char *haddrp;
  cfd_t client_fd;
  int i = 0;
  
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }
  port = atoi(argv[1]);
  client_fd.cn = 0;
  for (i = 0; i < 20; ++i) {
    client_fd.connfdv[i] = 0;
    client_fd.name[i] = NULL;
  }
  i = 0;
  
  listenfd = Open_listenfd(port);
  while (1) {
    clientlen = sizeof(clientaddr);
    client_fd.connfdv[i] = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    
    /* determine the domain name and IP address of the client */
    hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
		       sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    haddrp = inet_ntoa(clientaddr.sin_addr);
    client_fd.cn = i;
    new_thread(&client_fd);
    i++;
  }
  exit(0);
}

int new_thread(cfd_t *client_fd)
{
  pthread_t pid;
  pthread_create(&pid, NULL, echo, client_fd);
  //Close(connfd[i]);
}

void echo(cfd_t *client_fd) 
{
  int cn = client_fd->cn; // not thread safe !!!
  size_t n;
  char buf[MAXLINE];
  int connfd = client_fd->connfdv[cn];
  char name[MAXLINE];
  char *name_tmp;
  rio_t rio;
  int i = 1, name_len;
  
  /* 通知新用户登录 */
  Rio_readinitb(&rio, connfd);
  n = Rio_readlineb(&rio, name, MAXLINE);
  printf("OK in 88 and n = %d\n", n);
  name_len = n + 1;
  printf("comes in %s", name);
  name[name_len - 2] = cn + 48;
  name[name_len - 1] = '\n';
  printf("name become to %s", name);
  for (i = 0; i < MAX_CLIENT; ++i) {
    if (client_fd->name[i]) {
      Rio_writen(client_fd->connfdv[i], name, name_len);
      printf("notify %s successfully to %s", name, client_fd->name[i]);
    }
  }
  client_fd->name[cn] = name;
  for (i = 0; i < MAX_CLIENT; ++i) {
    if (name_tmp = client_fd->name[i]) {
      Rio_writen(connfd, name_tmp, strlen(name_tmp));
    }
  }

  /* 开始聊天 */
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    //puts(buf); //打印客户端发来的消息
    buf[n - 1] = '#';
    buf[n] = '\n';
    puts(buf);
    for (i = 0; i < MAX_CLIENT; ++i) {
      if (client_fd->connfdv[i]) {
	Rio_writen(client_fd->connfdv[i], buf, n + 1);
      }
    }
  }
  
  /* 客户端断开连接 */
  Close(connfd);
  printf("out %s", client_fd->name[cn]);
  client_fd->connfdv[cn] = 0;
  for (i = 0; i < MAX_CLIENT; ++i) {
    if (client_fd->connfdv[i]) {
      Rio_writen(client_fd->connfdv[i], client_fd->name[cn], name_len); //向其他客户端发送"\0"作为登出信号????漏洞
    }
  }
  client_fd->name[cn] = NULL;
}
/* $end echoserverimain */
