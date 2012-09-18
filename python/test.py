# coding=utf-8
#!/usr/bin/env python

import curses
import locale
import subprocess

locale.setlocale(locale.LC_CTYPE, '')
code = locale.getpreferredencoding()

myscreen = curses.initscr()
curses.noecho()
curses.cbreak()

# Use color
if curses.has_colors():
    curses.start_color()

curses.init_pair(1, curses.COLOR_BLACK, curses.COLOR_CYAN)
curses.init_pair(2, curses.COLOR_WHITE, curses.COLOR_BLUE)

curses.init_pair(13, curses.COLOR_CYAN, curses.COLOR_BLACK)


#left window
l_begin_x = 0 ; l_begin_y = 0
l_height = 24; l_width = 29
l_win = curses.newwin(l_height, l_width, l_begin_y, l_begin_x)
l_win.keypad(1)

l_title = "Channels"
channels = [(1, "华语"),
            (2, "欧美"),
            (4, "八零"),
            (32, "咖啡"),
            (9, "轻音乐"),
            (61, "新歌"),
            (6, "粤语"),
            #(76, "小清新"),
            (5, "九零"),]
l_win.border(0)
l_win.addstr(l_title.encode(code), curses.A_BOLD)

chan_y = 2
for chan_id, chan_name in channels:
    l_win.addstr(chan_y, 2, chan_name + " MHz", curses.color_pair(1))
    chan_y += 2



l_win.refresh()
#l_win.getch()

#right window
r_begin_x = 30; r_begin_y = 0
r_height = 16; r_width = 49
r_win = curses.newwin(r_height, r_width, r_begin_y, r_begin_x)

r_title = "Playing"
mp3_title = "歌曲名"
mp3_albumtitle = "专辑名"
player = "歌手"

r_win.border(0)
r_win.addstr(r_title.encode(code), curses.A_BOLD)
r_win.addstr(2, 2, player, curses.A_BOLD)
r_win.addstr(3, 2, mp3_albumtitle)
r_win.addstr(5, 2, mp3_title, curses.color_pair(13))
r_win.addstr(14, 30, "加红心")
r_win.addstr(14, 40, "下一首")
r_win.refresh()

l_win.move(2, 2)

while l_win.getyx()[1] == 2:
    ch = l_win.getch()
    if ch == ord('j'):
        if l_win.getyx()[0] < 16:
            l_win.move(l_win.getyx()[0] + 2, 2)
        else:
            continue
    elif ch == ord('k'):
        if l_win.getyx()[0] > 2:
            l_win.move(l_win.getyx()[0] - 2, 2)
        else:
            continue
    elif ch == ord('p'):
        subprocess.call(['python', 'doubanfm.py'])
    else:
        break

r_win.getch()
curses.endwin()
