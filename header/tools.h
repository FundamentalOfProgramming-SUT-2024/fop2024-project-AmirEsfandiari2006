#ifndef TOOL_H
#define TOOL_H

#include "include.h"

void set_up_colors(){
    init_pair(1, player_color, COLOR_BLACK);
    init_pair(2,COLOR_MAGENTA,COLOR_WHITE);
    init_pair(3,COLOR_RED,COLOR_BLACK);
}



int random_number(int min, int max) {

    return rand() % (max - min + 1) + min;
}

Point random_position_point(const Room* room){
    int x = random_number(room->start.x + 2, room->start.x + room->width - 2);
    int y = random_number(room->start.y + 2, room->start.y + room->height - 2);
    return (Point){x,y};
}

Point find_point_by_char(Level level,char ch){
    for(int i = 0; i < MAX_ROOM; i++){
        for(int j = 0; j < level.rooms[i].total_places ; j++){
            if(level.rooms[i].places[j].display == ch){
                return level.rooms[i].places->position;
            }
        }
    }
}

#endif