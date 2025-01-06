#include "header/include.h"

int main() {
    srand(time(0));

    getch();    

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
                
                printf_level(&level[level_map],&player);
                print_player(&player);

                command = getch();
                move_player(command, &player);
                handle_movement(mvinch(player.position.x,player.position.y),level,&player);
                clear();
                handle_command(command);
                update_player_room(&player,&(level[level_map]));
                
            }
            is_game_playing = false;
        }
    }

    refresh();
    endwin();
    
    return 0;
}
