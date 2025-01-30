#include "header/include.h"

int main() {
    
    setlocale(LC_ALL, "");

    srand(time(0));
    set_up_display();
    
    Level level[MAX_LEVEL];
    Player player;
  
    int command = 0;   
    while (command != EXIT) {
        if(!is_game_playing) {

            while (command != EXIT && !is_game_playing) {
                command = get_command_main_menu();
                open_items_menu(command);
            }
        }

        if (is_game_playing && is_logged_in) {
            if(player_load_game){
                    if(is_game_stop == true){

                    } else if(is_there_savegame){
                        //load_game(player_username);
                        have_game_played = true;
                    } else {
                        is_game_playing = false;
                        continue;
                    }
            } else if(player_new_game){
                
                reset_level(level);
                reset_player(&player);
            
                init_level(level);
                init_player(&player, &level[level_map]);

                is_game_ended = false; have_game_played = true; is_game_stop = false; delete_savegane(player_username);
            }
            while (command != ESCAPE) {
                if(!is_treasure_room){
                    printf_level(&level[level_map],&player);
                } else {
                    print_treasure_room(level,&player);
                }      
                print_player(&player);

                command = getch();
                if(command == ESCAPE){
                    is_game_stop = true;
                }
                move_player(command, &player);
                update_player_room(&player,&(level[level_map]));

                if(handle_command(command,&player) || handle_movement(player.position,level,&player)){
                    player.total_player_moves++;
                }
                else{
                    clear();
                }
                if(is_treasure_room && level->treasure_room.total_places == 0){
                    clear();
                    handle_ending(&player);
                    break;
                }
            }
            is_game_playing = false;
        }
    }

    if(is_game_ended == false && is_game_stop == true){
        //save_game(username,Level level, Player player);
    }

    refresh();
    endwin();
    
    return 0;
}
