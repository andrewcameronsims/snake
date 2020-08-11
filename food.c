#include <stddef.h>

#include "food.h"

Food* food_init()
{
  Food *food = (Food*) malloc(sizeof(Food));
  if (food == NULL) return NULL;

  food->head = NULL;
  return food;
}

void food_free(Food *food)
{
  Node *current_node = food->head;

  while (current_node != NULL)
  {
    Node *node_to_free = current_node;
    current_node = current_node->next;
    free(node_to_free);
  }
  free(food);
}

bool food_add(Food *food, int y, int x)
{
  Node *new_food = (Node*) malloc(sizeof(Node));
  if (new_food == NULL) return false;

  new_food->y = y;
  new_food->x = x;
  new_food->next = NULL;

  Node *current_node = food->head;
  while (current_node->next != NULL) current_node = current_node->next;
  current_node->next = new_food;

  return true;
}

bool food_remove(Food *food, int y, int x)
{
  Node *before_node = food->head;

  if (before_node->x == x && before_node->y == y) {
    food->head = food->head->next;
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

void render_food(Food *food)
{
  Node *current_node = food->head;
  while (current_node != NULL)
  {
    mvprintw(current_node->y, current_node->x, "%c", FOOD_CHAR);
    current_node = current_node->next;
  }
}
