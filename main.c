#include "header/functions.h"
#include "header/global.h"

int main(){
    int command;
    while(command != 0){
        command = get_command_main_menu();
        open_items_menu(command);
    }  
    

    return 0;
}