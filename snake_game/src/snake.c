#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "st7735_user_lib.h"
#include "snake.h"
#include "keyboard.h"


void draw_point_coord(coord_t coordinate, uint16_t color)
{
    bsp_lcd_fill_rect(color, coordinate.x*SIZE_SQUARE, SIZE_SQUARE, coordinate.y*SIZE_SQUARE, SIZE_SQUARE);
}

int check_snake_eat(snake_t *game, coord_t food)
{
    if ((game->coordinate[game->lenght - 1].x == food.x)&&(game->coordinate[game->lenght - 1].y == food.y))             return 1;     
    else return 0;
}

void snake_init(snake_t *game)
{
    game->lenght = 3;
    game->orientation = UP;
    game->coordinate = (coord_t*)malloc(sizeof(coord_t)*(game->lenght));
    game->coordinate[0].x = 20;
    game->coordinate[0].y = 20;
    game->coordinate[1].x = 20;
    game->coordinate[1].y = 21;
    game->coordinate[2].x = 20;
    game->coordinate[2].y = 22;
    return;
}

void snake_display(snake_t *game)
{
    for (int i = 0; i < game->lenght; i++)
    {
        draw_point_coord(game->coordinate[game->lenght - 1 - i], S_COLOR);
    }
}

void snake_move(snake_t *game, coord_t *food)
{
    coord_t new_node;

    switch (game->orientation)
    {
    case UP:
        new_node.x = game->coordinate[game->lenght - 1].x;
        new_node.y = game->coordinate[game->lenght - 1].y + 1;
        break;
    
    case DOWN:
        new_node.x = game->coordinate[game->lenght - 1].x;
        new_node.y = game->coordinate[game->lenght - 1].y - 1;
        break;

    case LEFT:
        new_node.x = game->coordinate[game->lenght - 1].x - 1;
        new_node.y = game->coordinate[game->lenght - 1].y;
        break;
    
    case RIGHT:
        new_node.x = game->coordinate[game->lenght - 1].x + 1;
        new_node.y = game->coordinate[game->lenght - 1].y;
        break;

    default:
        break;
    }

    if (new_node.x > ST7735_C_VALUE/SIZE_SQUARE)            new_node.x = new_node.x - ST7735_C_VALUE/SIZE_SQUARE;
    if (new_node.x < 0)                                     new_node.x = new_node.x + ST7735_C_VALUE/SIZE_SQUARE;

    if (new_node.y > ST7735_R_VALUE/SIZE_SQUARE)            new_node.y = new_node.y - ST7735_R_VALUE/SIZE_SQUARE;
    if (new_node.y < 0)                                     new_node.y = new_node.y + ST7735_R_VALUE/SIZE_SQUARE;

    if (check_snake_eat(game, *food) == 1)
    {
        game->lenght += 1;
        game->coordinate = (coord_t*)realloc(game->coordinate, sizeof(coord_t)*(game->lenght));
        game->coordinate[game->lenght - 1].x = new_node.x;
        game->coordinate[game->lenght - 1].y = new_node.y;
        gen_food_coord(food);
    }else {
        coord_t *temp = game->coordinate;
        coord_t *new_snake = (coord_t*)malloc(sizeof(coord_t)*(game->lenght));
        memcpy(new_snake, &game->coordinate[1], (game->lenght - 1)*sizeof(coord_t));
        game->coordinate = new_snake;
        game->coordinate[game->lenght - 1].x = new_node.x;
        game->coordinate[game->lenght - 1].y = new_node.y;
        draw_point_coord(temp[0], B_COLOR);
        free(temp);
    }
}

void snake_set_orient(orient o, snake_t *game)
{
    if (o == 0)         return;
    
    if ((o == LEFT)||(o == RIGHT))
    {
        if ((game->orientation == UP)||(game->orientation == DOWN))
        {
            game->orientation = o;
        }
    }else if ((o == DOWN)||(o == UP))
    {
        if ((game->orientation == LEFT)||(game->orientation == RIGHT))
        {
            game->orientation = o;
        }
    }
}

orient get_control_from_button(void)
{
    uint8_t button_value;
    int ret;
    ret = button_read_value(&button_value);
    if (ret != 0)           printf("Control error.\n");
    return (orient)button_value;
}

void gen_food_coord(coord_t *food)
{
    food->x = rand()%(ST7735_C_VALUE/SIZE_SQUARE);
    food->y = rand()%(ST7735_R_VALUE/SIZE_SQUARE); 
}

int check_game_over(snake_t *game)
{
    for (int i = 1; i < (game->lenght - 1); i++)
    {
        if ((game->coordinate[game->lenght - 1].x == game->coordinate[i].x)&&(game->coordinate[game->lenght - 1].y == game->coordinate[i].y))
        {
            return 1;
        }
    }
    return 0;
}