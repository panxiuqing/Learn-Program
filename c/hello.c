#include<ncurses.h>

int main()
{
    int ch;
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    printw("Type any character to see bold\n");
    ch = getch();
    if (ch == KEY_F(2))
        printw("F2 Key pressed");
    else
    {
        printw("The pressed key is ");
        attron(A_BOLD);
        printw("%c", ch);
        attron(A_BOLD);
    }
    refresh();
    getch();
    endwin();
    return 0;
}
