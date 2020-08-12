#include <stddef.h>

#include "snake.h"

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

bool snake_alive(Snake *snake, int y_limit, int x_limit)
{
  Node *current_node = snake->head;
  int head_x = current_node->x;
  int head_y = current_node->y;

  // Check that the snake's head is within the game screen
  if (
    head_x > x_limit || head_x < 0 ||
    head_y > y_limit || head_y < 0
  ) {
    return false;
  }

  // Check that the snake has not bitten itself
  current_node = current_node->next;
  while (current_node != NULL)
  {
    int current_x = current_node->x;
    int current_y = current_node->y;

    if (current_x == head_x && current_y == head_y) return false;
    current_node = current_node->next;
  }
  return true;
}

void grow_snake(Snake *snake)
{
  int direction = snake->direction;
  Node *tail = snake->tail;

  switch (direction)
  {
  case UP:
    snake_enqueue(snake, tail->y - 1, tail->x);
    break;

  case DOWN:
    snake_enqueue(snake, tail->y + 1, tail->x);
    break;

  case LEFT:
    snake_enqueue(snake, tail->y, tail->x - 1);
    break;

  case RIGHT:
    snake_enqueue(snake, tail->y, tail->x + 1);
    break;

  default:
    break;
  }
}

void feed_snake(Snake *snake, Food *food, int *score, int y_limit, int x_limit)
{
  int snake_x = snake->head->x;
  int snake_y = snake->head->y;
  int direction = snake->direction;

  switch (direction)
  {
  case UP:
    snake_y--;
    break;
  case DOWN:
    snake_y++;
    break;
  case LEFT:
    snake_x--;
    break;
  case RIGHT:
    snake_x++;
  default:
    break;
  }

  Node *current_node = food->head;
  while (current_node != NULL)
  {
    int food_x = current_node->x;
    int food_y = current_node->y;
    if (snake_x == food_x && snake_y == food_y) {
      food_remove(food, food_y, food_x);
      food_add(food, rand() % y_limit, rand() % x_limit);
      grow_snake(snake);
      score++;
      return;
    }
    current_node = current_node->next;
  }
}

void render_snake(Snake *snake)
{
  Node *current_node = snake->head;
  while (current_node != NULL)
  {
    attron(COLOR_PAIR(SNAKE_PAIR));
    mvaddch(current_node->y, current_node->x, SNAKE_CHAR);
    attroff(COLOR_PAIR(SNAKE_PAIR));
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
