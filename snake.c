#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define DELAY 100000

int max_rows;
int max_cols;

void handle_key(int key)
{
  erase();
  switch (key)
  {
  case KEY_LEFT:
    mvprintw(max_rows / 2, max_cols / 2, "%s", "left");
    break;

  case KEY_UP:
    mvprintw(max_rows / 2, max_cols / 2, "%s", "up");
    break;

  case KEY_RIGHT:
    mvprintw(max_rows / 2, max_cols / 2, "%s", "right");
    break;

  case KEY_DOWN:
    mvprintw(max_rows / 2, max_cols / 2, "%s", "down");
    break;

  default:
    mvprintw(max_rows / 2, max_cols / 2, "%s", "none");
    break;
  }
}

void game_loop(void)
{
  int counter = 0;

  while(1)
  {
    usleep(DELAY);
    int key = getch();
    mvprintw(0, 0, "%d", counter);
    handle_key(key);
    counter++;
  }
}

int main(void)
{
  initscr();
  curs_set(0);
  keypad(stdscr, 1);
  noecho();
  cbreak();
  nodelay(stdscr, 1);
  getmaxyx(stdscr, max_rows, max_cols);

  game_loop();
  return 0;
}
