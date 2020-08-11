#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "snake.h"
#include "food.h"

#define DELAY 100000

int max_rows;
int max_cols;

int score = 0;

void handle_key(int key, Snake *snake);
void game_init();
void game_loop();
