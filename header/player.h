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
    player->health = MAX_PLAYER_HEALTH;
    player->room = random_room(level);
    player->position.x = random_position(player,level).x;
    player->position.y = random_position(player,level).y;   
}


void print_player(const Player* player){
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
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


void handle_movement(char ch,Level level[],Player *player){
    switch (ch)
    {
    case '>':
        level_map++;
        player->position = find_point_by_char(level[level_map],'<');
        break;
    case '<':
        level_map--;
        player->position = find_point_by_char(level[level_map],'>');
    }
}

#endif 