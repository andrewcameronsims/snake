#include "main.h"

void handle_key(int key, Snake *snake)
{
  if (key == -1) return;
  if (key == 'q') {
    endwin();
    exit(0);
  }
  // this is where we should limit direction changes (e.g. can't turn direction from right to left or up to down)
  snake->direction = key;
}

void game_init()
{
  initscr();
  curs_set(0);
  keypad(stdscr, 1);
  noecho();
  cbreak();
  getmaxyx(stdscr, max_rows, max_cols);
}

void intro_screen()
{
  mvprintw(max_rows / 2 - 7, max_cols / 2 - 15, "%s", " _____             _        ");
  mvprintw(max_rows / 2 - 6, max_cols / 2 - 15, "%s", "/  ___|           | |       ");
  mvprintw(max_rows / 2 - 5, max_cols / 2 - 15, "%s", "\\ `--. _ __   __ _| | _____ ");
  mvprintw(max_rows / 2 - 4, max_cols / 2 - 15, "%s", " `--. \\ '_ \\ / _` | |/ / _ \\");
  mvprintw(max_rows / 2 - 3, max_cols / 2 - 15, "%s", "/\\__/ / | | | (_| |   <  __/");
  mvprintw(max_rows / 2 - 2, max_cols / 2 - 15, "%s", "\\____/|_| |_|\\__,_|_|\\_\\___|");

  mvprintw(max_rows / 2, max_cols / 2 - 15, "%s", "Arrow keys to move");
  mvprintw(max_rows / 2 + 1, max_cols / 2 - 15, "%s", "Press q to quit");

  mvprintw(max_rows / 2 + 4, max_cols / 2 - 15, "%s", "You die if you hit yourself or the edge of the screen");
  getch();
  clear();
}

void game_loop()
{
  nodelay(stdscr, 1);
  start_color();
  init_pair(SNAKE_PAIR, COLOR_GREEN, COLOR_BLACK);
  init_pair(FOOD_PAIR, COLOR_RED, COLOR_BLACK);

  Snake *snake = snake_init();
  snake_enqueue(snake, max_rows / 2, max_cols / 2);
  snake_enqueue(snake, max_rows / 2, max_cols / 2 + 1);
  snake_enqueue(snake, max_rows / 2, max_cols / 2 + 2);

  Food *food = food_init();
  food_add(food, rand() % max_rows, rand() % max_cols);

  while(TRUE)
  {
    usleep(DELAY);
    int key = getch();
    handle_key(key, snake);
    update_snake(snake);
    erase();
    render_food(food);
    render_snake(snake);
    mvprintw(0, 0, "|Score: %d|", score);
    bool alive = snake_alive(snake, max_rows, max_cols);
    if (!alive) {
      endwin();
      exit(0);
    }
    feed_snake(snake, food, &score, max_rows, max_cols);
  }
}

int main(void)
{
  game_init();
  intro_screen();
  game_loop();
  return 0;
}
