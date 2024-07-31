#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "snake.h"
#include "st7735_user_lib.h"
#include "app.h"
#include "keyboard.h"

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] < arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}

void delay_ms(long milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

int play_game(app_t *game)
{
    orient o;
    int score;
    gen_food_coord(&game->food);
    snake_init(&game->snake_game);
    bsp_lcd_set_background_color(B_COLOR);
    snake_display(&game->snake_game);
    while(1)
    {
        draw_point_coord(game->food, RED);
        snake_move(&game->snake_game, &game->food);
        snake_display(&game->snake_game);
        if (check_game_over(&game->snake_game))
        {
            score = game_over_handle(game);
            add_highcores(game, score);
            return score;
        }
        o = get_control_from_button();
        snake_set_orient(o, &game->snake_game);
        
        delay_ms(300/(game->speed_level));
    }
    return 0;
}

int game_over_handle(app_t *game)
{
    char msg[30];
    orient choose;

    bsp_lcd_print_str(20, 20, 2, ORANGE, B_COLOR, "GAME OVER!!!");
    sprintf(msg, "%d", (game->snake_game.lenght)*(game->speed_level));
    delay_ms(500);
    bsp_lcd_print_str(20, 40, 2, ORANGE, B_COLOR, "Your score:");
    bsp_lcd_print_str(20, 60, 2, BLUE, B_COLOR, msg);
    bsp_lcd_print_str(20, 90, 1, CYAN, B_COLOR, "Press any button");

    while (1)
    {
        choose = get_control_from_button();
        if (choose != 0)
        {
            break;
        }
        delay_ms(300);
    }
    return (game->snake_game.lenght)*(game->speed_level);
}

void draw_arrow(int num_order, int offset)
{
    bsp_lcd_print_str(0, offset+20*num_order, 2, ORANGE, B_COLOR, "=>");
}

int print_main_menu(void)
{
    orient choose;
    int arrow_pos = 0;
    bsp_lcd_set_background_color(B_COLOR);
    bsp_lcd_print_str(30, 10, 2, RED, B_COLOR, "Play a game");
    bsp_lcd_print_str(30, 30, 2, RED, B_COLOR, "Highscores");
    bsp_lcd_print_str(30, 50, 2, RED, B_COLOR, "Settings");
    bsp_lcd_print_str(30, 70, 2, RED, B_COLOR, "Exit");
    draw_arrow(arrow_pos, 10);

    while (1)
    {
        choose = get_control_from_button();
        if (choose == UP)           arrow_pos -= 1;
        else if (choose == DOWN)    arrow_pos += 1;
        else if (choose == RIGHT)   return arrow_pos+1;

        if (arrow_pos == 4)         arrow_pos = 0;
        else if (arrow_pos == -1)   arrow_pos = 3;

        if ((choose == UP)||(choose == DOWN))
        {
            reset_arrow();
            draw_arrow(arrow_pos, 10);
        }
        delay_ms(300);
    }
}

void reset_arrow(void)
{
    bsp_lcd_fill_rect(B_COLOR, 0, 30, 0, BSP_LCD_HEIGHT);
}

void highcore_init(app_t *game)
{
    for (int i = 0; i < 10; i++)
    {
        game->highcore[i] = 0;
    }
}

void sort_list_highcores(app_t *game)
{
    bubbleSort(game->highcore, 10);
}

void print_highcores(app_t *game)
{
    orient choose;
    bsp_lcd_set_background_color(B_COLOR);
    bsp_lcd_print_str(30, 10, 2, ORANGE, B_COLOR, "HIGHSCORES");
    char msg[10];
    for (uint8_t i = 0; i < 5; i++)
    {
        sprintf(msg, "%d: %d", i+1, game->highcore[i]);
        bsp_lcd_print_str(20, 30+(i*10), 1, GREEN, B_COLOR, msg);
    }

    for (uint8_t i = 5; i < 10; i++)
    {
        sprintf(msg, "%d: %d", i+1, game->highcore[i]);
        bsp_lcd_print_str(100, 30+((i - 5)*10), 1, GREEN, B_COLOR, msg);
    }

    bsp_lcd_print_str(20, 90, 1, CYAN, B_COLOR, "Press any button");

    while (1)
    {
        choose = get_control_from_button();
        if (choose != 0)
        {
            return;
        }
        delay_ms(300);
    }
}

void print_list_level(app_t *game)
{
    orient choose;
    int arrow_pos = 0;
    int cur_x = (game->speed_level - 1)*20 + 30;
    bsp_lcd_set_background_color(B_COLOR);
    bsp_lcd_print_str(30, 10, 2, ORANGE, B_COLOR, "LEVEL");
    bsp_lcd_print_str(30, 30, 2, BLUE, B_COLOR, "EASY");
    bsp_lcd_print_str(30, 50, 2, BLUE, B_COLOR, "MEDIUM");
    bsp_lcd_print_str(30, 70, 2, BLUE, B_COLOR, "HARD");
    bsp_lcd_print_str(30, 90, 2, BLUE, B_COLOR, "Back");
    draw_arrow(arrow_pos, 30);
    draw_x(cur_x);

    while(1)
    {
        choose = get_control_from_button();
        if (choose == UP)           arrow_pos -= 1;
        else if (choose == DOWN)    arrow_pos += 1;

        if (arrow_pos == 4)         arrow_pos = 0;
        else if (arrow_pos == -1)   arrow_pos = 3;

        if ((choose == UP)||(choose == DOWN))
        {
            reset_arrow();
            draw_arrow(arrow_pos, 30);
        }

        if (choose == RIGHT)
        {
            if (arrow_pos == 3)
            {
                return;
            }else{
                game->speed_level = arrow_pos+1;
                reset_x(cur_x);
                cur_x = (game->speed_level - 1)*20 + 30;
                draw_x(cur_x);
            }
        }
        delay_ms(300);
    }
}

void draw_x(int value)
{
    bsp_lcd_print_str(120, value, 2, BLUE, B_COLOR, "X");
}

void reset_x(int value)
{
    bsp_lcd_print_str(120, value, 2, B_COLOR, B_COLOR, "X");
}

void exit_handle(void)
{
    bsp_lcd_set_background_color(B_COLOR);
    bsp_lcd_print_str(10, 60, 2, GREEN, B_COLOR, "GOODBYE !!!");
}

void add_highcores(app_t *game, int highscore)
{
    if (highscore > game->highcore[9])
    {
        game->highcore[9] = highscore;
        sort_list_highcores(game);
    }
}
