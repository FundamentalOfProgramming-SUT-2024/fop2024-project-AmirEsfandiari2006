#include "header/include.h"

int main(){

    srand(time(0));

    /*
    int command;
    while(command != EXIT && is_game_plying == false){
        command = get_command_main_menu(); open_items_menu(command);
        
    }  
    */
    //play_the_game();
    
    Level level1;
    generate_random_room(&level1);
    printf_level(&level1);
    
    return 0;
}