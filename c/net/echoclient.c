#include <ncurses.h>
#include "csapp.h"

struct pt_struct
{
  int clientfd;
  char wbuf[MAXLINE];
  char rbuf[MAXLINE];
  rio_t rio;
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
  //  int startx, starty, width, height;
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
  pt.wbuf[i] = ':';

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

  send.height = 5;
  people.height = LINES;

  send.width = COLS - 20;
  people.width = 20;

  send.startx = 0;
  people.startx = COLS - 20;

  people.starty = 0;
  send.starty = LINES - 5;

  refresh();
  
  send_win = create_newwin(send, "SEND");
  people_win = create_newwin(people, "PEOPLE:");
  
  /* attron(COLOR_PAIR(2));
  mvprintw(send.starty, send.startx, "%s", "SEND");
  attroff(COLOR_PAIR(2));

  mvprintw(people.starty + 1, people.startx + 1, "%s", "PEOPLE: ");
  */
  refresh();
  pt.clientfd = open_clientfd(host, port);
  Rio_readinitb(&(pt.rio), pt.clientfd);
  pthread_create(&read_pid, NULL, read_thread, &pt);

  int name_len = strlen(argv[3]) + 1;
  while (mvwgetnstr(send_win, 0, 0, pt.wbuf + name_len, MAXLINE - name_len) != ERR) {
    pt.wbuf[strlen(pt.wbuf)] = '\n';
    Rio_writen(pt.clientfd, pt.wbuf, strlen(pt.wbuf));
    bzero(pt.wbuf + name_len, MAXLINE - name_len);
    werase(send_win);
    //box(send_win, 0, 0);
    //mvwprintw(send_win, 0, 0, "%s", "SEND");
  }

  /*while((ch = getch()) != KEY_F(2)) {
    }*/
  endwin();
  return 0;
}

WINDOW *create_newwin(win_s w, char *t)
{
  WINDOW *local_win, *border_win;
  border_win = newwin(w.height, w.width, w.starty, w.startx);
  local_win = newwin(w.height - 2, w.width - 2, w.starty + 1, w.startx + 1);
  box(border_win, 0, 0);
  attron(A_BOLD);
  mvwprintw(border_win, 0, 0, "%s", t);
  attroff(A_BOLD);
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
  WINDOW *show_win;
  win_s show;
  int l = -1, y, x;

  show.startx = show.starty = 0;
  show.height = LINES - 5;
  show.width = COLS - 20;
  show_win = create_newwin(show, "MESSAGE");
  //  attron(COLOR_PAIR(1));
  //mvwprintw(show_win, show.starty, show.startx, "%s", "MESSAGE");
  //attroff(COLOR_PAIR(1));
  //wrefresh(show_win);
  //scroll
  scrollok(show_win, true);
  wsetscrreg(show_win, 0, show.height - 3);
  while (Rio_readlineb(&(pt->rio), pt->rbuf, MAXLINE)) {
    if (l < show.height - 3)
      l++;
    mvwprintw(show_win, l, 0, "%s", pt->rbuf);
    wrefresh(show_win);
  }
}
