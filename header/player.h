#include "include.h"

#ifndef PLAYER_H
#define PLAYER_H

int random_number(int,int);
void move_fast(Level *level, Player* player);

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
    player->current_weapon.symbol = ' ';
    player->thrown_weapon = NORMAL_ARROW_INDEX;
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

void attack_monster_away(Monster* monster,Player*player){
            clear_message();
            throw_dart = true;
            monster->health -= weapons_damage[player->thrown_weapon];
            if(weapons[player->thrown_weapon] == 'W'){
                monster->moves = 0;
            }
            if(monster->health <= 0){
                
                mvprintw(1,1,"You killed %s. Good luck!",get_monster_name(*monster));
                monster->position.x = -1;
                monster->position.y = -1;
            } else {
                mvprintw(1,1,"You hit %s and give it %d damage!",get_monster_name(*monster),weapons_damage[player->thrown_weapon]);
            }
    
}

void handle_seen(int x, int y, Player* player, Level* level) {

        
    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++){
            is_tile_seen[level_map][x + i][y +j] = true;
        }
    }
    for(int i = 0; i < MAX_ROOM;i++){
       for(int j = 0; j < MAX_DOOR;j++){
            if(level[level_map].rooms[i].is_door[j] == 1){
                if(abs(level[level_map].rooms[i].doors[j].x - player->position.x) <= 1 && abs(level[level_map].rooms[i].doors[j].y - player->position.y) <= 1){
                        level[level_map].rooms[i].is_seen = true;
                }
            }   
            
        } 
    }
    
     if(player->room != -1){
        level[level_map].rooms[player->room].is_seen = true;
    }
}




void move_player(int command,Player* player,Level*level){
    switch(command){
        case KEY_LEFT:
        if(valid_tile(player->position.x,player->position.y - (1 + speed_spell_bonus))){
            handle_seen(player->position.x,player->position.y - (1 + speed_spell_bonus),player,level);
            player->position.y -= (1 + speed_spell_bonus);
        }    
        break;
        case KEY_RIGHT:
        if(valid_tile(player->position.x,player->position.y + (1 + speed_spell_bonus))){
            handle_seen(player->position.x,player->position.y + (1 + speed_spell_bonus),player,level);
            player->position.y += (1 + speed_spell_bonus);
        }
            
            
        break;
        case KEY_UP:
        if(valid_tile(player->position.x - (1 + speed_spell_bonus),player->position.y)){
            handle_seen(player->position.x -  (1 + speed_spell_bonus),player->position.y ,player,level);
            player->position.x -= (1 + speed_spell_bonus); 
        } 
        break;
        case KEY_DOWN:
        if(valid_tile(player->position.x + (1 + speed_spell_bonus),player->position.y)){
            handle_seen(player->position.x +  (1 + speed_spell_bonus),player->position.y ,player,level);
            player->position.x += (1 + speed_spell_bonus); 
        }
        break;
        case KEY_HOME:
        if(valid_tile(player->position.x - (1 + speed_spell_bonus),player->position.y - (1 + speed_spell_bonus))){
            player->position.x -= (1 + speed_spell_bonus);player->position.y -= (1 + speed_spell_bonus);
            handle_seen(player->position.x -  (1 + speed_spell_bonus),player->position.y -  (1 + speed_spell_bonus),player,level);
        } 
        break;
        case KEY_PPAGE:
        if(valid_tile(player->position.x - (1 + speed_spell_bonus),player->position.y + (1 + speed_spell_bonus))){
            player->position.x -= (1 + speed_spell_bonus); player->position.y += (1 + speed_spell_bonus);
            handle_seen(player->position.x -  (1 + speed_spell_bonus),player->position.y +  (1 + speed_spell_bonus),player,level);
        }
        break;
        case KEY_NPAGE:
        if(valid_tile(player->position.x + (1 + speed_spell_bonus),player->position.y + (1 + speed_spell_bonus))){
            player->position.x += (1 + speed_spell_bonus); player->position.y += (1 + speed_spell_bonus);
            handle_seen(player->position.x +  (1 + speed_spell_bonus),player->position.y +  (1 + speed_spell_bonus),player,level);
        }
        break;
        case KEY_END:
        if(valid_tile(player->position.x + (1 + speed_spell_bonus),player->position.y - (1 + speed_spell_bonus))){
            player->position.x += (1 + speed_spell_bonus); player->position.y -= (1 + speed_spell_bonus);
            handle_seen(player->position.x +  (1 + speed_spell_bonus),player->position.y -  (1 + speed_spell_bonus),player,level);
        }
        break;
    }
}




void throw_weapon(Level *level,Player *player){

    clear_message();
    mvprintw(1,1,"Which direction do you want to throw the dart?");
    int which_way = getch();
    int dagger_longest_way = 5;
    int magic_wand_longest_way = 10;
    Point start_point = player->position;
    if(player->room != -1){

    
    switch(player->thrown_weapon){
        case NORMAL_ARROW_INDEX:
            if(player->number_of_each_weapon[NORMAL_ARROW_INDEX] > 0){
                switch(which_way){
                case KEY_LEFT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'R';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    start_point.y--;
                }
                break;
                case KEY_RIGHT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'R';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    start_point.y++;
                }
                break;     
                case KEY_UP:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'R';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    start_point.x--;
                }
                break;
                case KEY_DOWN:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'R';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    start_point.x++;
                }
                break;
                case KEY_HOME:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'R';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    start_point.x--; start_point.y--;
                }
                break;
                case KEY_PPAGE:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'R';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                     start_point.x--; start_point.y++;
                }
                break;
                case KEY_NPAGE:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'R';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                     start_point.x++; start_point.y++;
                }
                break;
                case KEY_END:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'R';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[NORMAL_ARROW_INDEX]--;
                        break;
                    }
                     start_point.x++; start_point.y--;
                }
                break;
            }
            } 
            else {
                clear_message();
                throw_dart = true;
                mvprintw(1,1,"You don't have normal arrow to throw!");
            }
            break;
            case DAGGER_INDEX:
            if(player->number_of_each_weapon[DAGGER_INDEX] > 0){
                switch(which_way){
                case KEY_LEFT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || dagger_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'D';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    dagger_longest_way --;
                    start_point.y--;
                }
                break;
                case KEY_RIGHT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || dagger_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'D';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    start_point.y++; dagger_longest_way --;
                }
                break;     
                case KEY_UP:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || dagger_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'D';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    start_point.x--; dagger_longest_way --;
                }
                break;
                case KEY_DOWN:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || dagger_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'D';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    start_point.x++; dagger_longest_way --;
                }
                break;
                case KEY_HOME:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || dagger_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'D';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    start_point.x--; start_point.y--; dagger_longest_way --;
                }
                break;
                case KEY_PPAGE:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || dagger_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'D';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                     start_point.x--; start_point.y++; dagger_longest_way --;
                }
                break;
                case KEY_NPAGE:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || dagger_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'D';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                     start_point.x++; start_point.y++; dagger_longest_way --;
                }
                break;
                case KEY_END:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || dagger_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'D';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[DAGGER_INDEX]--;
                        break;
                    }
                     start_point.x++; start_point.y--; dagger_longest_way --;
                }
                break;
            }
            } 
            else {
                clear_message();
                throw_dart = true;
                mvprintw(1,1,"You don't have dagger to throw!");
            }
            break;     
            case MAGIC_WAND_INDEX:
            if(player->number_of_each_weapon[MAGIC_WAND_INDEX] > 0){
                switch(which_way){
                case KEY_LEFT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || magic_wand_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.y--;     magic_wand_longest_way--;
                }
                break;
                case KEY_RIGHT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || magic_wand_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.y++; magic_wand_longest_way--;
                }
                
                break;     
                case KEY_UP:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || magic_wand_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.x--;  magic_wand_longest_way--;
                }
                break;
                case KEY_DOWN:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || magic_wand_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                
                    start_point.x++; magic_wand_longest_way--;
                }
                break;
                case KEY_HOME:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || magic_wand_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.x--; start_point.y--; magic_wand_longest_way--;
                }
                break;
                case KEY_PPAGE:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || magic_wand_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                     start_point.x--; start_point.y++; magic_wand_longest_way--;
                }
                break;
                case KEY_NPAGE:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || magic_wand_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                     start_point.x++; start_point.y++; magic_wand_longest_way--;
                }
                break;
                case KEY_END:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O' || magic_wand_longest_way == 0){
                        level[level_map].rooms[player->room].total_places ;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level[level_map].rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                     start_point.x++; start_point.y--; magic_wand_longest_way--;
                }
                break;
            }
            } 
            else {
                clear_message();
                throw_dart = true;
                mvprintw(1,1,"You don't have magic wand to throw!");
            }
            break; 
    }
    } else {
        clear_message();
        mvprintw(1,1,"You have to be in a room to throw a dart.");
    }
    
}


bool handle_movement(Point position,Level *level,Player *player){
    int value = mvinch(position.x,position.y) & A_CHARTEXT;
    switch (value)
    {
    case '.':
        for(int i = 0; i < level[level_map].rooms[player->room].total_trap;i++){
            if(level[level_map].rooms[player->room].traps[i].position.x == position.x && level[level_map].rooms[player->room].traps[i].position.y == position.y){
                clear();
                player->health -= 1;
                level[level_map].rooms[player->room].traps[i].position.x = -1;
                level[level_map].rooms[player->room].traps[i].position.y = -1;
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
        update_player_room(player,&(level[level_map]));
        clear();
        printf_level(&level[level_map],player);
        refresh();
        return true;
    case '<':
        level_map--;
        player->position = find_point_by_char(level[level_map],'>');
        update_player_room(player,&(level[level_map]));
        clear();
        printf_level(&level[level_map],player);
        refresh();
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
            if(level[level_map].rooms[player->room].places[item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position)].is_it_thrown == true){
                  mvprintw(1,1,"You collect a Dagger Arrows!");  
                  player->number_of_each_weapon[DAGGER_INDEX]++;
                  player->number_of_player_weapon += 1;
            } else {
                mvprintw(1,1,"You collect 3 Daggers!");
                player->number_of_each_weapon[DAGGER_INDEX] += 3;
                player->number_of_player_weapon += 3;
            }
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
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
            if(level[level_map].rooms[player->room].places[item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position)].is_it_thrown == true){
                  mvprintw(1,1,"You collect a Normal Arrows!");  
                  player->number_of_each_weapon[NORMAL_ARROW_INDEX]++;
                  player->number_of_player_weapon += 1;
            } else {
                mvprintw(1,1,"You collect 5 Normal Arrows!");
                player->number_of_each_weapon[NORMAL_ARROW_INDEX] += 5;
                player->number_of_player_weapon+= 5;
            }
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
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

bool handle_command(char command,Player*player,Level *level){
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
        case 'a':
            throw_weapon(level,player);          
        return true;
        case 'm':
            is_hidden = !is_hidden;
            clear();
            printf_level(&level[level_map],player); 
            refresh();
        return true;
        case 'f':
            move_fast(&level[level_map],player);
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
                player -> hunger += 80;
                if(player->hunger > START_HUNGER){
                    player-> hunger = START_HUNGER;
                }
                if(player->health > START_HEALTH){player->health = START_HEALTH;}
                player->number_of_each_food[NORMAL_FOOD_INDEX]--;
                mvprintw(1,1,"You Consumed a normal food!");
            } else {
                mvprintw(1,1,"You don't have normal food to consume!");
            }
        return;
        case PURE_FOOD_INDEX:
            if(player->number_of_each_food[PURE_FOOD_INDEX] >= 1){
                player->health += 15;
                player -> hunger += 150;
                damage_spell_life += 15;
                if(player->hunger > START_HUNGER){
                    player-> hunger = START_HUNGER;
                }
                if(player->health > START_HEALTH){player->health = START_HEALTH;}
                player->number_of_each_food[PURE_FOOD_INDEX]--;
                mvprintw(1,1,"You Consumed a pure food!");
            } else {
                mvprintw(1,1,"You don't have pure food to consume!");
            }
        return;
        case MAGIC_FOOD_INDEX:
            if(player->number_of_each_food[MAGIC_FOOD_INDEX] >= 1){
                player->health += 15;
                player -> hunger += 150;
                speed_spell_life += 15;
                if(player->hunger > START_HUNGER){
                    player-> hunger = START_HUNGER;
                }
                if(player->health > START_HEALTH){player->health = MAGIC_FOOD_INDEX;}
                player->number_of_each_food[MAGIC_FOOD_INDEX]--;
                mvprintw(1,1,"You Consumed a magic food!");
            } else {
                mvprintw(1,1,"You don't have magic food to consume!");
            }
        return;
   
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

void use_weapon(int weapon_index,Player *player){
    clear_message();
    switch(weapon_index){
        case MACE_INDEX:
        if(player->number_of_each_weapon[MACE_INDEX] > 0){
            player->current_weapon.symbol = weapons[MACE_INDEX];
            player->strength = START_STRENGHTH; player->strength += weapons_damage[MACE_INDEX];
            player->number_of_each_weapon[MACE_INDEX] -= 1;
            mvprintw(1,1,"Now, you use a Mace!");
        } else {
            mvprintw(1,1,"You don't have a Mace to use!");
        }
        return;
        case SWORD_INDEX:
        if(player->number_of_each_weapon[SWORD_INDEX] > 0){
            player->current_weapon.symbol = weapons[SWORD_INDEX];
            player->strength = START_STRENGHTH; player->strength += weapons_damage[SWORD_INDEX];
            player->number_of_each_weapon[SWORD_INDEX] -= 1;
            mvprintw(1,1,"Now, you use a Sword!");
        } else {
            mvprintw(1,1,"You don't have a Sword to use!");
        }
        return;
        case DAGGER_INDEX:
            player->thrown_weapon = DAGGER_INDEX;
            mvprintw(1,1,"Now, you throw dagger!");
        return;
        case NORMAL_ARROW_INDEX:
            player->thrown_weapon = NORMAL_ARROW_INDEX;
            mvprintw(1,1,"Now, you throw arrow!");
        return;
        case MAGIC_WAND_INDEX:
            player->thrown_weapon = MAGIC_WAND_INDEX;
            mvprintw(1,1,"Now, you throw magic wand!");
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
    
    is_treasure_room = false;
    is_game_stop = true;
    getch();
    clear();
    return;
}

bool handle_player_death(const Player *player){
    if(player->health <= 0){
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

        mvprintw(lines/2 - 9,cols/2 - 15," Losing today doesn't define");
        mvprintw(lines/2 - 8,cols/2 - 15,"    your journey tomorrow   ");
        mvprintw(lines/2 - 6,cols/2 - 19," ->Total time playing: %d",player->total_player_moves);
        mvprintw(lines/2 - 3,cols/2 - 19," ->Total gold collected: %d",player->gold);
        mvprintw(lines/2 - 0,cols/2 - 19," ->Total score: %d",player->gold * (game_diff + 2));
        mvprintw(lines/2 + 5,cols/2 - 15,"   Press any key to exit...");

        update_score_file(0,0,0);
        is_game_ended = true;
        delete_savegane(player_username);
        
        is_game_stop = true;
        getch();
        clear();
        return true;
    }
    return false;
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

void handle_rotten_food(Player* player){
    if(rotten_food_cycle <= 0){

        if(player->number_of_each_food[NORMAL_FOOD_INDEX] >= 1){
            player->number_of_each_food[NORMAL_FOOD_INDEX] -= 1;
            player->number_of_each_food[ROTTEN_FOOD_INDEX] += 1;
        } 
        if(player->number_of_each_food[PURE_FOOD_INDEX] >= 1){
            player->number_of_each_food[PURE_FOOD_INDEX] -= 1;
            player->number_of_each_food[NORMAL_FOOD_INDEX] += 1;
        } 
        if(player->number_of_each_food[MAGIC_FOOD_INDEX] >= 1){
            player->number_of_each_food[MAGIC_FOOD_INDEX] -= 1;
            player->number_of_each_food[NORMAL_FOOD_INDEX] += 1;
        } 
        rotten_food_cycle = ROTTEN_CYCLE;
    } else {
        rotten_food_cycle--;
    }
}

void move_fast(Level *level, Player* player){
    clear_message();
    mvprintw(1,1,"Which direction do you want to teleporte?");
    Point start_point = player->position;
    int which_way = getch();
    switch(which_way){
                case KEY_LEFT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == 'O' || tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E' || tile == ' '){
                        break;
                    }
                    start_point.y--;
                }
                player->position = start_point;
                break;
                case KEY_RIGHT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == 'O' || tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E' || tile == ' '){
                        break;
                    }
                    start_point.y++;
                }
                player->position = start_point;
                break;     
                case KEY_UP:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == 'O' || tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E' || tile == ' '){
                        break;
                    }
                    start_point.x--;
                }
                player->position = start_point;
                break;
                case KEY_DOWN:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == 'O' || tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E' || tile == ' '){
                        break;
                    }
                    start_point.x++;
                }
                player->position = start_point;
                break;
                case KEY_HOME:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == 'O' || tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E' || tile == ' '){
                        break;
                    }
                    start_point.x--; start_point.y--;
                }
                player->position = start_point;
                break;
                case KEY_PPAGE:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == 'O' || tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E' || tile == ' '){
                        break;
                    }
                     start_point.x--; start_point.y++;
                }
                player->position = start_point;
                break;
                case KEY_NPAGE:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == 'O' || tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E' || tile == ' '){
                        break;
                    }
                     start_point.x++; start_point.y++;
                }
                player->position = start_point;
                break;
                case KEY_END:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == 'O' || tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E' || tile == ' '){
                        break;
                    }
                    start_point.x++; start_point.y--;
                }
                player->position = start_point;
                break;
    }
}



#endif 