#include "include.h"

#ifndef PLAYER_H
#define PLAYER_H

int random_number(int,int);


int random_room(const Level* level) {
    int result;
    do {
        result = random_number(0, 7);
    } while (level->is_there_room[result] != 1);
    return result;
}

Point random_position(Player *player, const Level* level){
    int x = random_number(level->rooms[player->room].start.x + 1,level->rooms[player->room].start.x + level->rooms[player->room].width);
    int y = random_number(level->rooms[player->room].start.y + 1,level->rooms[player->room].start.y + level->rooms[player->room].height);
    return (Point){x,y};
}

void init_player(Player *player,const Level* level){
    player->gold = 0;
    player->hunger = START_HUNGER;
    player->strength = START_STRENGHTH;
    player->health = START_HEALTH;
    player->room = random_room(level);
    player->position.x = random_position(player,level).x;
    player->position.y = random_position(player,level).y;
    strcpy(player->player_weapon[0].name, "Mace");
    player->number_of_player_weapon = 1;
    player->number_of_player_spell = 0;
    player->number_of_player_food = 0;
    player->total_player_moves = 0;
    for(int i = 0; i < MAX_FOOD; i++){
        player->number_of_each_food[i] = 0;
        player->number_of_each_spell[i] = 0;
        player->number_of_each_weapon[i] = 0;
    }
    player->number_of_each_weapon[MACE_INDEX] = 1;
}


void print_player(const Player* player){
    start_color();
    set_up_colors();
    attron(COLOR_PAIR(1));
    mvprintw(player->position.x,player->position.y,PLAYER_SYM);
    attroff(COLOR_PAIR(1));
}

bool valid_tile(int x,int y){
    char tile = mvinch(x,y) & A_CHARTEXT;
    switch (tile)
    {
    case '.':
    case '#':
    case '+':
    case 'W':
    case 'S':
    case 'R':
    case 'M':
    case 'D':
    case 'G':
    case 'H':
    case 'Z':
    case 'C':
    case 'F':
    case '^':
    return true;
    case '_':
    case 'O':
    case ' ':
    case '|':
    case 'I':
    case 'U':
    case 'E':
    case 'N':
    case 'B':
    
        return false;
    }
}

void handle_treasure_room(Level *level, Player *player){
    player->position.x = level->treasure_room.start.x + random_number(2,level->treasure_room.width);
    player->position.y = level->treasure_room.start.y + random_number(2,level->treasure_room.height);
}

bool attack_monster(int command,Player* player,Level* level){
        Point position = player->position;
        switch(command){
        case KEY_LEFT:
            position.y--;
        break;
        case KEY_RIGHT:
            position.y++;
        break;
        case KEY_UP:
            position.x--;
        break;
        case KEY_DOWN:
            position.x++;
        break;
        case KEY_HOME:
            position.x--;position.y--;
        break;
        case KEY_PPAGE:
            position.x--; position.y++;
        break;
        case KEY_NPAGE:
            position.x++; position.y++;
        break;
        case KEY_END:
            position.x++; position.y--;
        }
        char tile = mvinch(position.x,position.y);
        if(tile == 'B' || tile == 'E' || tile == 'N' || tile == 'U' || tile == 'I'){
            Monster* monster = get_monster_by_point(position,level,player);
            monster->health -= player->strength + damage_spell_bonus;
            if(monster->health <= 0){
                
                mvprintw(1,1,"You killed %s. Good luck!",get_monster_name(*monster));
                monster->position.x = -1;
                monster->position.y = -1;
            } else {
                mvprintw(1,1,"You hit %s and give it %d damage!",get_monster_name(*monster),player->strength);
            }
            return true;
        }
        return false;
}



void move_player(int command,Player* player){
    switch(command){
        case KEY_LEFT:
        if(valid_tile(player->position.x,player->position.y - (1 + speed_spell_bonus)))
            player->position.y -= (1 + speed_spell_bonus);
        break;
        case KEY_RIGHT:
        if(valid_tile(player->position.x,player->position.y + (1 + speed_spell_bonus)))
            player->position.y += (1 + speed_spell_bonus);
        break;
        case KEY_UP:
        if(valid_tile(player->position.x - (1 + speed_spell_bonus),player->position.y))
            player->position.x -= (1 + speed_spell_bonus);
        break;
        case KEY_DOWN:
        if(valid_tile(player->position.x + (1 + speed_spell_bonus),player->position.y))
            player->position.x += (1 + speed_spell_bonus);
        break;
        case KEY_HOME:
        if(valid_tile(player->position.x - (1 + speed_spell_bonus),player->position.y - (1 + speed_spell_bonus))){
            player->position.x -= (1 + speed_spell_bonus);player->position.y -= (1 + speed_spell_bonus);
        } 
        break;
        case KEY_PPAGE:
        if(valid_tile(player->position.x - (1 + speed_spell_bonus),player->position.y + (1 + speed_spell_bonus))){
            player->position.x -= (1 + speed_spell_bonus); player->position.y += (1 + speed_spell_bonus);
        }
        break;
        case KEY_NPAGE:
        if(valid_tile(player->position.x + (1 + speed_spell_bonus),player->position.y + (1 + speed_spell_bonus))){
            player->position.x += (1 + speed_spell_bonus); player->position.y += (1 + speed_spell_bonus);
        }
        break;
        case KEY_END:
        if(valid_tile(player->position.x + (1 + speed_spell_bonus),player->position.y - (1 + speed_spell_bonus))){
            player->position.x += (1 + speed_spell_bonus); player->position.y -= (1 + speed_spell_bonus);
        }
        break;
    }
}


bool handle_movement(Point position,Level *level,Player *player){
    int value = mvinch(position.x,position.y) & A_CHARTEXT;
    switch (value)
    {
    case '.':
        for(int i = 0; i < level->rooms[player->room].total_trap;i++){
            if(level->rooms[player->room].traps[i].position.x == position.x && level->rooms[player->room].traps[i].position.y == position.y){
                clear();
                player->health -= 1;
                level->rooms[player->room].traps[i].position.x = -1;
                level->rooms[player->room].traps[i].position.y = -1;
                mvprintw(1,1,"You stand on a trap! Becareful next time!");
            } 
        }
        return true;
    case '^':
        is_treasure_room = true;
        handle_treasure_room(level,player);
        clear();
        return true;
    case '>':
        level_map++;
        player->position = find_point_by_char(level[level_map],'<');
        clear();
        return true;
    case '<':
        level_map--;
        player->position = find_point_by_char(level[level_map],'>');
        clear();
        return true;
    case 'G':
        if(can_get_item){
            clear();
            if(!is_treasure_room){
                player->gold += (&level[level_map])->rooms[player->room].places[item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position)].amout;
                mvprintw(1,1,"You collect %d gold!",(&level[level_map])->rooms[player->room].places[item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position)].amout);
                remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            } else {
                player->gold += level->treasure_room.places[item_of_room_index_in_room(&(level->treasure_room),player->position)].amout;
                mvprintw(1,1,"You collect %d gold!",(level->treasure_room.places[item_of_room_index_in_room(&level->treasure_room,player->position)].amout));
                 remove_place(&(level->treasure_room),item_of_room_index_in_room(&level->treasure_room,player->position));
            }

            return true;
        }
    case 'H':
        if(can_get_item){
            clear();
            strcpy(player->plaeyr_spell[player->number_of_player_spell].name, "Health");
            player->plaeyr_spell[player->number_of_player_spell].symbol = 'H';
            player->number_of_player_spell++;
            mvprintw(1,1,"You collect a Health Spell!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            player->number_of_each_spell[HEALTH_SPELL_INDEX]++;
            return true;
        }
    case 'Z':
        if(can_get_item){
            clear();
            strcpy(player->plaeyr_spell[player->number_of_player_spell].name, "Damage");
            player->plaeyr_spell[player->number_of_player_spell].symbol = 'Z';
            player->number_of_player_spell++;
            mvprintw(1,1,"You collect a Damage Spell!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            player->number_of_each_spell[DAMAGE_SPELL_INDEX]++;    
            return true;
        }
    case 'C':
        if(can_get_item){
            clear();
            strcpy(player->plaeyr_spell[player->number_of_player_spell].name, "Speed");
            player->plaeyr_spell[player->number_of_player_spell].symbol = 'C';
            player->number_of_player_spell++;
            mvprintw(1,1,"You collect a Speed Spell!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            player->number_of_each_spell[SPEED_SPELL_INDEX]++;
            return true;
        }
    case 'F':
        if(can_get_item){
            clear();
            int random_num = random_number(1,10);
            if(random_num >= 0 && random_num <= 6){
                strcpy(player->player_food[player->number_of_player_food].name, "Normal Food");
                player->number_of_each_food[NORMAL_FOOD_INDEX]++;
                mvprintw(1,1,"You collect a Normal Food!");
            }
            if(random_num >= 7 && random_num <= 8){
                strcpy(player->player_food[player->number_of_player_food].name, "Pure Food");
                player->number_of_each_food[PURE_FOOD_INDEX]++;
                mvprintw(1,1,"You collect a Pure Food!");
            }
            if(random_num >= 9 && random_num <= 10){
                strcpy(player->player_food[player->number_of_player_food].name, "Magic Food");
                player->number_of_each_food[MAGIC_FOOD_INDEX]++;
                mvprintw(1,1,"You collect a Magic Food!");
            }
            player->number_of_player_food++;
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            return true;
        }
    case MACE:
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Mace");
            player->player_weapon[player->number_of_player_weapon].symbol = 'M';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a mace!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            player->number_of_each_weapon[MACE_INDEX]++;
            return true;
        }
    case DAGGER:
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Dagger");
            player->player_weapon[player->number_of_player_weapon].symbol = 'D';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a Dagger!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            player->number_of_each_weapon[DAGGER_INDEX]++;
            return true;
        }
    case MAGIC_WAND:
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Magic Wand");
            player->player_weapon[player->number_of_player_weapon].symbol = 'W';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a Magic wand!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            player->number_of_each_weapon[MAGIC_WAND_INDEX]++;
            return true;
        }
    case NORMAL_ARROW:
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Normal Arrow");
            player->player_weapon[player->number_of_player_weapon].symbol = 'R';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a Normal Arrow!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            player->number_of_each_weapon[NORMAL_ARROW_INDEX]++;
            return true;
        }
    case SWORD:
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Sword");
            player->player_weapon[player->number_of_player_weapon].symbol = 'S';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a Sword!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            player->number_of_each_weapon[SWORD_INDEX]++;
            return true;
        }  
    }
    return false;
}

bool handle_command(char command,Player*player){
    switch(command){
        case 'q':
        if(can_get_item == true){
            can_get_item = !can_get_item;
            print_message("Getting Items is turned off Press 'q' to turn on.");
        } else {
            can_get_item = !can_get_item;
            print_message("Getting Items is turned on Press 'q' to turn off.");
        }
        return true;
        case 'i':
            open_and_handle_inventory(player);
        return true;
    }
    return false;
}

void clear_message(){
    mvprintw(1,1,"                                                  ");
    mvprintw(2,1,"                                                  ");
    mvprintw(3,1,"                                                  ");
}

void use_food(int food_index,Player *player){
    clear_message();
    switch(food_index){
        case NORMAL_FOOD_INDEX:
            if(player->number_of_each_food[NORMAL_FOOD_INDEX] >= 1){
                player->health += 3;
                player -> hunger += 30;
                if(player->hunger > 100){
                    player-> hunger = 100;
                }
                if(player->health > START_HEALTH){player->health = START_HEALTH;}
                player->number_of_each_food[NORMAL_FOOD_INDEX]--;
                mvprintw(1,1,"You Consumed a normal food!");
            } else {
                mvprintw(1,1,"You don't have normal food to consume!");
            }
   
    }
}

void use_spell(int spell_index,Player *player){
    clear_message();
    switch(spell_index){
        case DAMAGE_SPELL_INDEX:
        if(player->number_of_each_spell[DAMAGE_SPELL_INDEX] > 0){
            damage_spell_life  += 15;
            player->number_of_each_spell[DAMAGE_SPELL_INDEX]--;
            mvprintw(1,1,"You consumed a damage spell!");
        } else {
            mvprintw(1,1,"You don't have damage spell to consume!");
        }
        return;
        case HEALTH_SPELL_INDEX:
        if(player->number_of_each_spell[HEALTH_SPELL_INDEX] > 0){
            health_spell_life  += 15;
            player->number_of_each_spell[HEALTH_SPELL_INDEX]--;
            mvprintw(1,1,"You consumed a health spell!");
        } else {
            mvprintw(1,1,"You don't have health spell to consume!");
        }
        return;
        case SPEED_SPELL_INDEX:
        if(player->number_of_each_spell[SPEED_SPELL_INDEX] > 0){
            speed_spell_life  += 15;
            player->number_of_each_spell[SPEED_SPELL_INDEX]--;
            mvprintw(1,1,"You consumed a speed spell!");
        } else {
            mvprintw(1,1,"You don't have speed spell to consume!");
        }
        return;
    }
}

void handle_ending(const Player *player){
    clear();
    for(int x = lines/2 - 10; x <  lines/2 +10; x++){
        mvprintw(x,cols/2 - 20, "|");
        mvprintw(x,cols/2 + 20, "|");
    }
    for(int y = cols/2 - 20; y <  cols/2 + 20; y++){
        mvprintw(lines/2 - 10, y, "-");
        mvprintw(lines/2 + 10, y, "-");
    }
    mvprintw(lines/2 - 10,cols/2 - 20,"+");
    mvprintw(lines/2 - 10,cols/2 + 20,"+");
    mvprintw(lines/2 + 10,cols/2 - 20,"+");
    mvprintw(lines/2 + 10,cols/2 + 20,"+");

    mvprintw(lines/2 - 9,cols/2 - 15,"   Congragulation! You win!");
    mvprintw(lines/2 - 6,cols/2 - 19," ->Total time playing: %d",player->total_player_moves);
    mvprintw(lines/2 - 3,cols/2 - 19," ->Total gold collected: %d",player->gold);
    mvprintw(lines/2 - 0,cols/2 - 19," ->Total score: %d",player->gold * (game_diff + 2));
    mvprintw(lines/2 + 5,cols/2 - 15,"   Press any key to exit...");

    update_score_file(player->gold * (game_diff + 2),player->total_player_moves,player->gold);
    is_game_ended = true;
    delete_savegane(player_username);
    
    is_game_stop = true;
    getch();
    clear();
    return;
}


void handle_player_spell(Player *player){
    if(health_spell_life > 0 && player->health < START_HEALTH){
        player->health += 1;
    }
    if(damage_spell_life > 0){
        damage_spell_bonus = player->strength;
    } else {
        damage_spell_bonus = 0;
    }
    if(speed_spell_life > 0){
        speed_spell_bonus = 1;
    } else {
        speed_spell_bonus = 0;
    }
}

void heal_full_food(Player* player){
    if(player->hunger > START_HUNGER - 20){
        if(player->health < START_HEALTH){
            player->health++;
        }
    }
}


#endif 