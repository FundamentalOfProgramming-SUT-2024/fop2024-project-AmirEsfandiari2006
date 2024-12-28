#include "header/functions.h"

int main(){
    int command;
    while(command != 0){
        command = get_command_main_menu();
        open_items_menu(command);
    }  
    

    return 0;
}