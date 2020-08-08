#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define DELAY 100000
#define SNAKE '#'

#define DOWN 58
#define UP 59
#define LEFT 60
#define RIGHT 61

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

// Snake (queue) methods

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

// Snake (game logic) methods

void render_snake(Snake *snake)
{
  Node *current_node = snake->head;
  while (current_node != NULL)
  {
    mvprintw(current_node->y, current_node->x, "%c", SNAKE);
    current_node = current_node->next;
  }
}

void update_snake(Snake *snake, int direction);

// IO functions

void handle_key(int key, Snake *snake)
{
  if (key == 'q') {
    endwin();
    exit(0);
  }
  snake->direction = key;
}

void game_loop()
{
  Snake *snake = snake_init();
  snake_enqueue(snake, max_rows / 2, max_cols / 2);

  while(TRUE)
  {
    usleep(DELAY);
    int key = getch();
    handle_key(key, snake);
    render_snake(snake);
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
