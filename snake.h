#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>
#include "node.h"
#include "food.h"

#define SNAKE_CHAR '#'
#define DOWN 258
#define UP 259
#define LEFT 260
#define RIGHT 261

typedef struct {
  Node *head;
  Node *tail;
  int direction;
  int size;
} Snake;

Snake* snake_init();

void snake_free(Snake *snake);

bool snake_dequeue(Snake *snake);

bool snake_enqueue(Snake *snake, int y, int x);

int snake_size(Snake *snake);

void render_snake(Snake *snake);

void update_snake(Snake *snake);

bool snake_alive(Snake *snake, int y_limit, int x_limit);

void grow_snake(Snake *snake);

void feed_snake(Snake *snake, Food *food, int *score, int y_limit, int x_limit);

#endif
