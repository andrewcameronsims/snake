#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT 'h'
#define DOWN 'j'
#define UP 'k'
#define RIGHT 'l'

int max_rows;
int max_cols;

void game_loop(void)
{
  while(1)
  {
    char key = getch();
    mvprintw(max_rows / 2, max_cols / 2, "%c", key);
  }
}

int main(void)
{
  initscr();
  curs_set(0);
  noecho();
  getmaxyx(stdscr, max_rows, max_cols);

  game_loop();
  return 0;
}
