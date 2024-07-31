#ifndef APP_H__
#define APP_H__

#include <stdio.h>
#include <stdlib.h>

#include "st7735.h"
#include "snake.h"

typedef enum{
    EASY = 1,
    MEDIUM,
    HARD,
}level;

typedef struct 
{
    snake_t snake_game;
    uint16_t color_b;
    level speed_level;
    coord_t food;
    int highcore[10];
}app_t;


void delay_ms(long milliseconds);
int play_game(app_t *game);
int game_over_handle(app_t *game);

void draw_arrow(int num_order, int offset);
int print_main_menu(void);
void reset_arrow(void);

void highcore_init(app_t *game);

void sort_list_highcores(app_t *game);
void add_highcores(app_t *game, int highscore);

void print_highcores(app_t *game);

void print_list_level(app_t *game);
void draw_x(int value);
void reset_x(int value);

void exit_handle(void);

#endif