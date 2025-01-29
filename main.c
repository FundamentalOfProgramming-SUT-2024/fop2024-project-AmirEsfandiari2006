#include "header/include.h"

int main() {
    
    setlocale(LC_ALL, "");

    srand(time(0));
    set_up_display();
    
    //Game set_up
    Level level[MAX_LEVEL];
    init_level(level);
    

    //Player set_up
    Player player;
    init_player(&player, &level[level_map]);
  
    
    int command = 0;   
    while (command != EXIT) {
        if(!is_game_playing) {

            while (command != EXIT && !is_game_playing) {
                command = get_command_main_menu();
                open_items_menu(command);

            }
        }

        if (is_game_playing && is_logged_in) {

            while (command != ESCAPE) {
                if(!is_treasure_room){
                    printf_level(&level[level_map],&player);
                } else {
                    print_treasure_room(level,&player);
                }      
                print_player(&player);

                command = getch();
                move_player(command, &player);
                update_player_room(&player,&(level[level_map]));

                if(handle_command(command,&player) || handle_movement(player.position,level,&player)){
                }
                else{
                    clear();
                } 
            }
            is_game_playing = false;
        }
    }

    refresh();
    endwin();
    
    return 0;
}
