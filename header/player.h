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
    player->armor = START_ARMOR;
    player->health = START_HEALTH;
    player->room = random_room(level);
    player->position.x = random_position(player,level).x;
    player->position.y = random_position(player,level).y;
    strcpy(player->player_weapon[0].name, "Mace");
    player->number_of_player_weapon = 1;

}


void print_player(const Player* player){
    start_color();
    set_up_colors();
    attron(COLOR_PAIR(1));
    mvprintw(player->position.x,player->position.y,PLAYER_SYM);
    attroff(COLOR_PAIR(1));
}

bool valid_tile(int x,int y){
    char tile = mvinch(x,y);
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
    return true;
    case '_':
    case 'O':
    case ' ':
    case '|':
        return false;
    }
}


void move_player(int command,Player* player){
    switch(command){
        case KEY_LEFT:
        if(valid_tile(player->position.x,player->position.y - 1))
            player->position.y--;
        break;
        case KEY_RIGHT:
        if(valid_tile(player->position.x,player->position.y + 1))
            player->position.y++;
        break;
        case KEY_UP:
        if(valid_tile(player->position.x - 1,player->position.y))
            player->position.x--;
        break;
        case KEY_DOWN:
        if(valid_tile(player->position.x + 1,player->position.y))
            player->position.x++;
        break;
        case KEY_HOME:
        if(valid_tile(player->position.x - 1,player->position.y - 1)){
            player->position.x--;player->position.y--;
        } 
        break;
        case KEY_PPAGE:
        if(valid_tile(player->position.x - 1,player->position.y + 1)){
                        player->position.x--; player->position.y++;
        }
        break;
        case KEY_NPAGE:
        if(valid_tile(player->position.x + 1,player->position.y + 1)){
            player->position.x++; player->position.y++;
        }
        break;
        case KEY_END:
        if(valid_tile(player->position.x + 1,player->position.y - 1)){
            player->position.x++; player->position.y--;
        }
        break;
    }
}


bool handle_movement(char ch,Level *level,Player *player){
    switch (ch)
    {
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
            player->gold += (&level[level_map])->rooms[player->room].places[item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position)].amout;
            mvprintw(1,1,"You collect %d gold!",(&level[level_map])->rooms[player->room].places[item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position)].amout);
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            return true;
        }
    case 'M':
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Mace");
            player->player_weapon[player->number_of_player_weapon].symbol = 'M';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a mace!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            return true;
        }
    case 'D':
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Dagger");
            player->player_weapon[player->number_of_player_weapon].symbol = 'D';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a Dagger!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            return true;
        }
    case 'W':
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Magin Wand");
            player->player_weapon[player->number_of_player_weapon].symbol = 'W';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a Magic wand!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            return true;
        }
    case 'R':
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Normal Arrow");
            player->player_weapon[player->number_of_player_weapon].symbol = 'R';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a Magic Normal Arrow!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            return true;
        }
    case 'S':
        if(can_get_item){
            clear();
            strcpy(player->player_weapon[player->number_of_player_weapon].name, "Sword");
            player->player_weapon[player->number_of_player_weapon].symbol = 'S';
            player->number_of_player_weapon++;
            mvprintw(1,1,"You collect a Magic Sword!");
            remove_place(&level[level_map].rooms[player->room],item_of_room_index_in_room(&level[level_map].rooms[player->room],player->position));
            return true;
        }       
    }

    return false;
}

bool handle_command(char command){
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
    }
    return false;
}


#endif 