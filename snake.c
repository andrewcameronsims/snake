#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define DELAY 100000
#define SNAKE '#'

#define DOWN 258
#define UP 259
#define LEFT 260
#define RIGHT 261

int max_rows;
int max_cols;

typedef struct Node {
  int x;
  int y;
  struct Node *next;
} Node;

typedef struct {
  Node *head;
  Node *tail;
  int direction;
  int size;
} Snake;

// Snake data structure (queue) functions

Snake* snake_init()
{
  Snake *snake = (Snake*) malloc(sizeof(Snake));
  if (snake == NULL) return NULL;

  snake->head = NULL;
  snake->tail = NULL;
  snake->size = 0;
  snake->direction = RIGHT;
  return snake;
}

void snake_free(Snake *snake)
{
  if (snake == NULL) return;

  Node *current_node = snake->head;

  while (current_node != NULL)
  {
    Node *node_to_free = current_node;
    current_node = current_node->next;
    free(node_to_free);
  }
  free(snake);
}

bool snake_dequeue(Snake *snake)
{
  if (snake == NULL || snake->head == NULL) return false;

  Node *old_head = snake->head;
  snake->head = snake->head->next;

  free(old_head);
  snake->size--;
  return true;
}

bool snake_enqueue(Snake *snake, int y, int x)
{
  Node *new_tail = (Node*) malloc(sizeof(Node));
  if (snake == NULL || new_tail == NULL) return false;

  new_tail->y = y;
  new_tail->x = x;
  new_tail->next = NULL;

  if (snake->size == 0) {
    snake->tail = new_tail;
    snake->head = new_tail;
    snake->size++;
  } else {
    snake->tail->next = new_tail;
    snake->tail = new_tail;
    snake->size++;
  }
  return true;
}

int snake_size(Snake *snake)
{
  if (snake == NULL) return 0;
  return snake->size;
}

// Snake (game logic) functions

void render_snake(Snake *snake)
{
  Node *current_node = snake->head;
  while (current_node != NULL)
  {
    mvprintw(current_node->y, current_node->x, "%c", SNAKE);
    current_node = current_node->next;
  }
}

void update_snake(Snake *snake)
{
  snake_dequeue(snake);
  int current_y = snake->tail->y;
  int current_x = snake->tail->x;
  switch (snake->direction)
  {
  case UP:
    snake_enqueue(snake, current_y - 1, current_x);
    break;
  case DOWN:
    snake_enqueue(snake, current_y + 1, current_x);
    break;
  case LEFT:
    snake_enqueue(snake, current_y, current_x - 1);
    break;
  case RIGHT:
    snake_enqueue(snake, current_y, current_x + 1);
    break;
  default:
    break;
  }
}

// IO functions

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

  while(TRUE)
  {
    usleep(DELAY);
    int key = getch();
    handle_key(key, snake);
    update_snake(snake);
    erase();
    render_snake(snake);
  }
}

int main(void)
{
  game_init();
  game_loop();
  return 0;
}
