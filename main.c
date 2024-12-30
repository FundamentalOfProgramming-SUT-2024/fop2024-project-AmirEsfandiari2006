#include "header/functions.h"
#include "header/global.h"

int main(){
    int command;
    while(command != EXIT && is_game_plying == false){
        command = get_command_main_menu(); open_items_menu(command);
        
    }  
    //play_the_game();
    

    return 0;
}