#include "header/functions.h"
#include "header/global.h"

int main(){
    int command;
    while(command != EXIT){
        command = get_command_main_menu(); open_items_menu(command);
    }  
    

    return 0;
}