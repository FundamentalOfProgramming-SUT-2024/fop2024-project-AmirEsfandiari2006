#include "header/include.h"

int main(){

    srand(time(0));
    
    int command;
    while(command != EXIT && is_game_plying == false){
        command = get_command_main_menu(); open_items_menu(command);
        
    }  
    

    Level level1;
    generate_random_room(&level1);

    Player player; init_player(&player,&level1);

    
    while(command != ESCAPE){

        printf_level(&level1);
        print_player(&player);

        command = getch();
        move_player(command,&player);
        clear();
    }


    refresh();
    endwin();

    return 0;
}