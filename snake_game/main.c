#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "st7735_user_lib.h"
#include "snake.h"
#include "app.h"
#include <time.h>


int main(int argc, char const *argv[])
{
    app_t snake_game;
    snake_game.speed_level = EASY;
    highcore_init(&snake_game);
    int ret;
    while(1)
    {
        ret = print_main_menu();
        switch (ret)
        {
        case 1:
            play_game(&snake_game);
            break;

        case 2:
            print_highcores(&snake_game);
            break;
        
        case 3:
            print_list_level(&snake_game);
            break;
        
        case 4:
            exit_handle();
            return 0;

        default:
            break;
        }
    }
    return 0;
}
