#include "interface.h"

static struct interface_s {
  WINDOW
    *mainwin,
    *room_browser,
    *input_win,
    *chat_win
} interface_s;

void draw_borders(WINDOW *screen) {
  int x, y, i;
  getmaxyx(screen, y, x);
  // 4 corners
  mvwprintw(screen, 0, 0, "+");
  mvwprintw(screen, y - 1, 0, "+");
  mvwprintw(screen, 0, x - 1, "+");
  mvwprintw(screen, y - 1, x - 1, "+");
  // sides
  for (i = 1; i < (y - 1); i++) {
    mvwprintw(screen, i, 0, "|");
    mvwprintw(screen, i, x - 1, "|");
  }
  // top and bottom
  for (i = 1; i < (x - 1); i++) {
    mvwprintw(screen, 0, i, "-");
    mvwprintw(screen, y - 1, i, "-");
  }
}

void interface_setup(){
  interface_s.mainwin = initscr();
  interface_s.room_browser = newwin(LINES, 15, 0, 0);
  interface_s.input_win = newwin(3, COLS - 15, LINES - 3, 15);
  interface_s.chat_win = newwin(LINES - 3, COLS - 15, 0, 15);

  cbreak();
  noecho();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);

  mvwprintw(interface_s.mainwin, 13, 33, "Hello, World!");

  draw_borders(interface_s.room_browser);
  draw_borders(interface_s.input_win);
  draw_borders(interface_s.chat_win);
}

void interface_full_refresh(){
  wrefresh(interface_s.room_browser);
  wrefresh(interface_s.input_win);
  wrefresh(interface_s.chat_win);
}

void interface_teardown(){
  delwin(interface_s.room_browser);
  delwin(interface_s.input_win);
  delwin(interface_s.chat_win);
  delwin(interface_s.mainwin);

  endwin();
  refresh();
}
