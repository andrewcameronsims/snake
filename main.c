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
  nodelay(stdscr, 1);
  getmaxyx(stdscr, max_rows, max_cols);
}

void game_loop()
{
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
  game_loop();
  return 0;
}
