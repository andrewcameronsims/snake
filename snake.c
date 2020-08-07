#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define DELAY 100000

int max_rows;
int max_cols;

typedef struct Node {
  int x;
  int y;
  Node *next;
} Node;

typedef struct {
  Node *head;
  Node *tail;
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
  Node *new_tail;
  new_tail = (Node*) malloc(sizeof(Node));
  if (snake == NULL || new_tail == NULL) return false;

  new_tail->y = y;
  new_tail->x = x;

  snake->tail->next = new_tail;
  snake->tail = new_tail;
  snake->size++;
  return true;
}

int snake_size(Snake *snake)
{
  if (snake == NULL) return 0;
  return snake->size;
}

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
