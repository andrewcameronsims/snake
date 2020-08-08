#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define DELAY 100000
#define SNAKE '#'
#define APPLE 'o'

#define DOWN 258
#define UP 259
#define LEFT 260
#define RIGHT 261

int max_rows;
int max_cols;

int score = 0;

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

typedef struct {
  Node *head;
} Apples;

// Apples data structure functions

Apples* apples_init()
{
  Apples *apples = (Apples*) malloc(sizeof(Apples));
  if (apples == NULL) return NULL;

  apples->head = NULL;
  return apples;
}

void apples_free(Apples *apples)
{
  if (apples == NULL) return;

  Node *current_node = apples->head;

  while (current_node != NULL)
  {
    Node *node_to_free = current_node;
    current_node = current_node->next;
    free(node_to_free);
  }
  free(apples);
}

bool apples_add(Apples *apples)
{
  Node *new_apple = (Node*) malloc(sizeof(Node));
  if (new_apple == NULL) return false;

  new_apple->y = rand() % max_rows;
  new_apple->x = rand() % max_cols;
  new_apple->next = apples->head;
  apples->head = new_apple;
  return true;
}

bool apples_remove(Apples *apples, int y, int x)
{
  Node *before_node = apples->head;

  if (before_node->x == x && before_node->y == y) {
    apples->head = apples->head->next;
    free(before_node);
    return true;
  }

  Node *current_node = before_node->next;
  while (current_node != NULL)
  {
    if (current_node->x == x && current_node->y == y) {
      before_node->next = current_node->next;
      free(current_node);
      return true;
    } else {
      before_node = current_node;
      current_node = current_node->next;
    }
  }
  return false;
}

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

void render_apples(Apples *apples)
{
  Node *current_node = apples->head;
  while (current_node != NULL)
  {
    mvprintw(current_node->y, current_node->x, "%c", APPLE);
    current_node = current_node->next;
  }
}

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

bool snake_alive(Snake *snake)
{
  Node *current_node = snake->head;
  int head_x = current_node->x;
  int head_y = current_node->y;

  // Check that the snake's head is within the game screen
  if (
    head_x > max_cols || head_x < 0 ||
    head_y > max_rows || head_y < 0
  ) {
    return FALSE;
  }

  // Check that the snake has not bitten itself
  current_node = current_node->next;
  while (current_node != NULL)
  {
    int current_x = current_node->x;
    int current_y = current_node->y;

    if (current_x == head_x && current_y == head_y) return FALSE;
    current_node = current_node->next;
  }
  return TRUE;
}

void feed_snake(Snake *snake, Apples *apples)
{
  int snake_x = snake->head->x;
  int snake_y = snake->head->y;

  Node *current_node = apples->head;
  while (current_node != NULL)
  {
    int apple_x = current_node->x;
    int apple_y = current_node->y;
    if (snake_x == apple_x && snake_y == apple_y) {
      apples_remove(apples, apple_y, apple_x);
      apples_add(apples);
      score++;
      return;
    }
    current_node = current_node->next;
  }
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

  Apples *apples = apples_init();
  apples_add(apples);

  while(TRUE)
  {
    usleep(DELAY);
    int key = getch();
    handle_key(key, snake);
    update_snake(snake);
    erase();
    render_apples(apples);
    render_snake(snake);
    bool alive = snake_alive(snake);
    if (!alive) {
      endwin();
      exit(0);
    }
    feed_snake(snake, apples);
  }
}

int main(void)
{
  game_init();
  game_loop();
  return 0;
}
