#ifndef FOOD_H
#define FOOD_H

#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>
#include "node.h"

#define FOOD_CHAR 'o'
#define FOOD_PAIR 1

typedef struct {
  Node *head;
} Food;

Food* food_init();

void food_free(Food *food);

bool food_add(Food *food, int y, int x);

bool food_remove(Food *food, int y, int x);

void render_food(Food *food);

#endif
