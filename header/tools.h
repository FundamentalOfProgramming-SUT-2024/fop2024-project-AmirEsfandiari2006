#ifndef TOOL_H
#define TOOL_H

#include "include.h"


void set_up_display(){
    initscr();
    cols = COLS;
    lines = LINES;
    endwin();
    refresh;
}


void set_up_colors(){
    init_pair(1, player_color, COLOR_BLACK);
    init_pair(2,COLOR_MAGENTA,COLOR_WHITE);
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
    }



int random_number(int min, int max) {
    if (min > max) {
        fprintf(stderr, "Error: min cannot be greater than max.\n");
        exit(EXIT_FAILURE);
    }
    return (random() % (max - min + 1)) + min;
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
                return level.rooms[i].places[j].position;
            }
        }
    }
}

int item_of_room_index_in_room(const Room* room,Point point){
    for(int i = 0; i < room->total_places; i++){
        if(room->places[i].position.x == point.x && room->places[i].position.y == point.y){
            return i;
        }
    }
    
    return -1;
}

int update_player_room(Player *player, const Level* level) {
    for (int i = 0; i < MAX_ROOM; i++) {
        if (level->is_there_room[i] == 1) {
            if (player->position.x > level->rooms[i].start.x &&
                player->position.x <= level->rooms[i].start.x + level->rooms[i].width &&
                player->position.y > level->rooms[i].start.y &&
                player->position.y <= level->rooms[i].start.y + level->rooms[i].height) {
                
                player->room = i; 
                return i;         
            }
        }
    }
    player->room = -1;
    return -1;
}
void remove_place(Room *room, int index) {
    if (index < 0 || index >= room->total_places) {
        printf("Invalid index to remove.\n");
        return; 
    }

    for (int i = index; i < room->total_places - 1; i++) {
        room->places[i] = room->places[i + 1];
    }

    room->total_places--;
    room->places[room->total_places] = (Place){0, 0, 0, 0};
}

bool handle_unicode(char symbol,Point point){

    switch(symbol){
        case 'M':
            mvprintw(point.x, point.y, "%ls", L"⚒");
            return true;
        case 'D':
            mvprintw(point.x, point.y, "%ls", L"🗡");
            return true;
        case 'W':
            mvprintw(point.x, point.y, "%ls", L"🪄");
            return true;
        case 'R':
            mvprintw(point.x, point.y, "%ls", L"➹");
            return true;
        case 'S':
            mvprintw(point.x, point.y, "%ls", L"⚔");
            return true;
    }
    return false;
}



#endif