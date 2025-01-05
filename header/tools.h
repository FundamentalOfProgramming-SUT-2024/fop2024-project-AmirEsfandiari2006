#ifndef TOOL_H
#define TOOL_H

#include "include.h"

int random_number(int min, int max) {

    return rand() % (max - min + 1) + min;
}

Point random_position_point(const Room* room){
    int x = random_number(room->start.x + 1, room->start.x + room->width - 1);
    int y = random_number(room->start.y + 1, room->start.y + room->height - 1);
    return (Point){x,y};
}



#endif