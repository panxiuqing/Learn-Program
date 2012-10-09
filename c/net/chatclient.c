#include <ncurses.h>
#include "csapp.h"

struct pt_struct
{
  int clientfd; //客户端文件描述符
  char wbuf[MAXLINE]; //发送消息缓冲区
  char rbuf[MAXLINE]; //接受消息缓冲区
  rio_t rio;
  char name_list[128][20];
};

struct win_struct
{
  int startx, starty, width, height;
};

typedef struct win_struct win_s;
typedef struct pt_struct pt_s;

WINDOW *create_newwin(win_s win_size, char *title);
void destroy_win(WINDOW *local_win);
void read_thread(pt_s *pt);

int main(int argc, char *argv[])
{
  WINDOW *send_win, *people_win;
  win_s send, people;
  int ch;

  int port, i = 0;
  char *host, *name;
  pthread_t read_pid;
  pt_s pt;
  if (argc != 4) {
    fprintf(stderr, "usage: %s <host> <port> <nickname>\n", argv[0]);
    return 0;
  }
  host = argv[1];
  port = atoi(argv[2]);
  for (i = 0; i < strlen(argv[3]); i++) {
      pt.wbuf[i] = argv[3][i];
  }

  fprintf(stdout, "OK in 46\n");

  // setlocale(LC_ALL, "zh_CN.UTF-8");
  initscr();
  raw();
  echo();
  keypad(stdscr, TRUE);

  if(has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    return 1;
  }
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);

  send.height = 5;
  send.width = COLS - 20;
  send.startx = 0;
  send.starty = LINES - 5;

  refresh();
  
  send_win = create_newwin(send, "SEND");

  refresh();
  pt.clientfd = open_clientfd(host, port);
  Rio_readinitb(&(pt.rio), pt.clientfd);
  pt.wbuf[strlen(argv[3])] = '\n';
  Rio_writen(pt.clientfd, pt.wbuf, strlen(pt.wbuf));
  pt.wbuf[strlen(argv[3])] = ':';
  
  pthread_create(&read_pid, NULL, read_thread, &pt);

  int name_len = strlen(argv[3]) + 1;
  while (mvwgetnstr(send_win, 0, 0, pt.wbuf + name_len, MAXLINE - name_len) != ERR) {
    pt.wbuf[strlen(pt.wbuf)] = '\n';
    Rio_writen(pt.clientfd, pt.wbuf, strlen(pt.wbuf));
    bzero(pt.wbuf + name_len, MAXLINE - name_len);
    werase(send_win);
  }

  endwin();
  return 0;
}

WINDOW *create_newwin(win_s w, char *t)
{
  WINDOW *local_win, *border_win;
  border_win = newwin(w.height, w.width, w.starty, w.startx);
  local_win = newwin(w.height - 2, w.width - 2, w.starty + 1, w.startx + 1); 
  box(border_win, 0, 0);
  wattron(border_win, A_BOLD);
  mvwprintw(border_win, 0, 0, "%s", t);
  wattroff(border_win, A_BOLD);
  wrefresh(border_win);
  return local_win;
}

void destroy_win(WINDOW *local_win)
{
  wborder(local_win, ' ',' ',' ',' ',' ',' ',' ',' ');
  wrefresh(local_win);
  delwin(local_win);
}

void read_thread(pt_s *pt)
{
  WINDOW *show_win, *people_win;
  win_s show, people;
  int l = -1, y, x, mes_len = 0;
  int i = 0;
  for (i = 0; i < 128; ++i) {
    int j = 0;
    for (j = 0; j < 20; ++j) {
      pt->name_list[i][j] = 0;
    }
  }
  people.startx = COLS - 20;
  people.starty = 0;
  people.width = 20;
  people.height = LINES;
  people_win = create_newwin(people, "PEOPLE");

  show.startx = show.starty = 0;
  show.height = LINES - 5;
  show.width = COLS - 20;
  show_win = create_newwin(show, "MESSAGE");
  scrollok(show_win, true);
  wsetscrreg(show_win, 0, show.height - 3);
  while (Rio_readlineb(&(pt->rio), pt->rbuf, MAXLINE)) {
    int color_num = pt->rbuf[0] % 5;
    mes_len = strlen(pt->rbuf);
    if (pt->rbuf[mes_len - 2] == '#') { // 显示消息
      pt->rbuf[mes_len - 2] = '\n';
      pt->rbuf[mes_len - 1] = '\0';
      if (l < show.height - 3)
	l++;
      wattron(show_win, COLOR_PAIR(color_num));
      mvwprintw(show_win, l, 0, "%s", pt->rbuf);
      wattroff(show_win, COLOR_PAIR(color_num));
      wrefresh(show_win);
    }
    else { // 打印在线列表
      char pos = pt->rbuf[mes_len - 2];
      if (pt->name_list[pos][0]) {
	pt->name_list[pos][0] = 0;
      }
      else {
	strncpy(pt->name_list[pos], pt->rbuf, mes_len - 2);
      }
      int i = 0, ll = 0;
      werase(people_win);
      for (i = 0; i < 128; ++i) {
	if (pt->name_list[i][0]) {
	  wattron(people_win, COLOR_PAIR(pt->name_list[i][0] % 5));
	  mvwprintw(people_win, ll++, 0, "%s", pt->name_list[i]);
	  wattroff(people_win, COLOR_PAIR(pt->name_list[i][0] % 5));
	  wrefresh(people_win);
	}
      }
    }
  }
}
