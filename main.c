#include "header/include.h"

int main(){
    
    setlocale(LC_ALL, "");

    srand(time(0));
    set_up_display();
    
    Level level[MAX_LEVEL];
    Player player;
  
    int command = 0;   
    while (true) {
        if(!is_game_playing) {
            pthread_create(&music_thread2, NULL, play_music_mainmenu, NULL);
            while (command != EXIT && !is_game_playing) {
                command = get_command_main_menu();
                open_items_menu(command,level,&player);

            }
            pthread_join(music_thread2, NULL);
        }

        if (is_game_playing && is_logged_in) {
            pthread_create(&music_thread, NULL, play_music, NULL);
            if(player_load_game){
                    if(is_savefile_exits() == true && have_game_played == false){
                        load_game(level ,&player);
                        have_game_played = true;
                    } else if(is_game_stop == true && is_game_ended == false){
                        
                    } else {
                        have_game_played = true;
                        is_game_playing = false;
                        continue;
                    }
            } else if(player_new_game){
                
                reset_level(level);
                reset_player(&player);
                reset_seen();
            
                init_level(level);
                init_player(&player, &level[level_map]);

                level_map = 0;

                is_game_ended = false; have_game_played = true; is_game_stop = false; delete_savegane(player_username);
            }
            while (true) {
                if(command == ESCAPE){
                    is_game_stop = true;
                    break;
                }
                if(!is_treasure_room){
                    printf_level(&level[level_map],&player);
                } else {
                    print_treasure_room(level,&player);
                }      
                print_player(&player);

                command = getch();
                clear_message();
                if(command == ESCAPE){
                    is_game_stop = true;
                    
                }
                clear_message();

                move_player(command, &player,level);
                if(player.hunger > 0){
                    player.hunger--;
                } else {
                    player.hunger = 0;
                    player.health--;
                }
                
                update_player_room(&player,&(level[level_map]));

                handle_player_spell(&player);
                heal_full_food(&player);
                handle_rotten_food(&player);
                
                if(handle_command(command,&player,level) || handle_movement(player.position,level,&player)){
                    player.total_player_moves++;
                }
                else{
                    if(attack_monster(command,&player,level)){

                    } else {
                        if(!throw_dart){
                            clear();
                            
                        } else {
                            throw_dart = false;
                        }
                        
                    }
                }
                if(is_treasure_room == false){
                    handle_monsters_movement(level,&player);
                }
                


                if(handle_player_death(&player)){
                    reset_level(level);
                    reset_player(&player);
                    level_map = 0;
                    clear();
                    break;
                }

                handle_time();

                if(is_treasure_room && level->treasure_room.total_places == 0){
                    clear();
                    handle_ending(&player);
                    level_map = 0;
                    reset_level(level);
                    reset_player(&player);
                    break;
                }
                
            }
            is_game_playing = false;
            pthread_join(music_thread, NULL);
            
        }
    }



    refresh();
    endwin();
    
    return 0;
}
