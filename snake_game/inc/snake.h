#ifndef SNAKE_H__
#define SNAKE_H__


#include <stdio.h>
#include <stdint.h>
#include "st7735_user_lib.h"
#include "st7735.h"

#define SIZE_SQUARE                 4
#define B_COLOR                     BLACK
#define S_COLOR                     GREEN

typedef enum{
    UP = 1,
    DOWN,
    LEFT,
    RIGHT
}orient;

typedef struct {
    int x;
    int y;
}coord_t;

typedef struct {
    coord_t *coordinate;
    uint8_t lenght;
    orient orientation;
}snake_t;



void draw_point_coord(coord_t coordinate, uint16_t color);
int check_snake_eat(snake_t *game, coord_t food);
void snake_init(snake_t *game);
void snake_display(snake_t *game);
void snake_move(snake_t *game, coord_t *food);
void snake_set_orient(orient o, snake_t *game);
orient get_control_from_button(void);

void gen_food_coord(coord_t *food);

int check_game_over(snake_t *game);

#endif