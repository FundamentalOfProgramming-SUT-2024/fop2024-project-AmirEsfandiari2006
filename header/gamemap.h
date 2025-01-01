#ifndef GENERATEMAP_H
#define GENERATEMAP_H

#include "include.h"
#include "functions.h"

int random_number(int,int);


Room init_room(Point start,int width,int height,int num_of_door){
    Room room;
    room.start.x = start.x;
    room.start.y = start.y;
    room.width = width;
    room.height = height;
    return room;
}



void select_numbers(int *num, int *count, int start, int end) {
    int range_size = end - start + 1;
    int chosen[range_size];
    for (int i = 0; i < range_size; i++) {
        chosen[i] = 0;
    }

    int selected_count = 0;

    while (selected_count < 2) {
        int index = rand() % range_size + start;
        if (!chosen[index - start]) { 
            chosen[index - start] = 1;
            num[selected_count] = index;
            selected_count++;
        }
    }

    if ((rand() % 100) < 50 && selected_count < (end - start + 1)) {
        for (int i = 0; i < range_size; i++) {
            if (!chosen[i]) {
                chosen[i] = 1;
                num[selected_count] = i + start; 
                selected_count++;
                break;
            }
        }
    }

    if ((rand() % 100) < 10 && selected_count < (end - start + 1)) {
        for (int i = 0; i < range_size; i++) {
            if (!chosen[i]) {
                chosen[i] = 1;
                num[selected_count] = i + start;
                selected_count++;
                break;
            }
        }
    }

    *count = selected_count;
}

void print_room(Room room){

    int start_x = room.start.x;
    int start_y = room.start.y;

    int copy_start_x = room.start.x;
    int copy_start_y = room.start.y;

    

    for(int t = 0; t < room.height + 2; t++){
        mvprintw(start_x,start_y,"-");
        start_y++;
    }
    start_y = copy_start_y;
    start_x++;

    for(int i = 0; i < room.width; i++){
        mvprintw(start_x,start_y,"|");
        start_y++;
        for(int j = 0; j < room.height; j++){
            mvprintw(start_x,start_y,".");
            start_y++;
        } 
        mvprintw(start_x,start_y,"|");
        start_x++;
        start_y = copy_start_y;
    }
    for(int t = 0; t < room.height + 2; t++){
        mvprintw(start_x,start_y,"-");
        start_y++;
    }
    for(int i = 0; i < MAX_DOOR; i++){
        if(room.is_door[i] == true)
        mvprintw(room.doors[i].x,room.doors[i].y,"+");
    }
    
}

void set_door(Room* room, int dir){
    switch(dir){
        case Right: 
            room->is_door[Right] = true;
            room->doors[Right].x = room->start.x + random_number(1, room->width);
            room->doors[Right].y = room->start.y + room->height + 1; 
            return;
        case Up: 
            room->is_door[Up] = true;
            room->doors[Up].x = room->start.x;
            room->doors[Up].y = room->start.y + random_number(1, room->height) ; 
            return;
        case Left: 
            room->is_door[Left] = true;
            room->doors[Left].x = room->start.x + random_number(1, room->width);
            room->doors[Left].y = room->start.y;
            return;
        case Down:
            room->is_door[Down] = true; 
            room->doors[Down].x = room->start.x + room->width + 1;
            room->doors[Down].y = room->start.y + random_number(1, room->height) ; 
            return;     
    }
}

void generate_random_room(Level* level){
    
    
    if(random_number(0,1)){
        for(int i = 0 ; i < 4; i++){
            level->is_there_room[i] = 1;
            level->rooms[i].start.x = random_number(4,10);
            level->rooms[i].start.y = random_number(10,13) + i * (cols / 4);
            level->rooms[i].width = random_number(4,10);
            level->rooms[i].height = random_number(4,15);
            if(i == 0){
                set_door(&level->rooms[i],Right);
            }
            if(i == 1 || i == 2){
                set_door(&level->rooms[i],Right);
                set_door(&level->rooms[i],Left);
            }
            if(i == 3){
                set_door(&level->rooms[i],Left);
            }
        }
        int count = 0;
        int add_room[4];
        select_numbers(add_room,&count,4,7);
        for(int i = 0; i < count; i++){
            level->is_there_room[add_room[i]] = 1;
            level->rooms[add_room[i]].start.x = random_number(4,10) + lines/2;
            level->rooms[add_room[i]].start.y = random_number(10,13) + (add_room[i] - 4) * (cols / 4);
            level->rooms[add_room[i]].width = random_number(4,10);
            level->rooms[add_room[i]].height = random_number(4,15);
            if(add_room[i] == 4){
                int state = 1;
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 5){
                        state = 0;
                        set_door(&level->rooms[add_room[i]],Right);
                    }
                }
                if(random_number(0,1) || state){
                    set_door(&level->rooms[add_room[i]],Up);
                    set_door(&level->rooms[add_room[i] - 4],Down);
                }
                
            }
            int room5_room6 = 0;
            if(add_room[i] == 5 ){
            
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 4){
                        set_door(&level->rooms[add_room[i]],Left);
                    }
                }
                set_door(&level->rooms[add_room[i]],Up);
                set_door(&level->rooms[add_room[i] - 4],Down);
                for(int j = 0;j < count; j++){
                if(add_room[j] == 6){
                    if(random_number(0,1)){
                    room5_room6 = 1;
                    set_door(&level->rooms[add_room[i]],Right);
                    }
                    }
                }
            }
            
            if(add_room[i] == 6 ){
                if(room5_room6){
                    set_door(&level->rooms[add_room[i]],Left);
                }
                
                if(room5_room6){
                   if(random_number(0,1)){
                        set_door(&level->rooms[add_room[i]],Up);
                        set_door(&level->rooms[add_room[i] - 4],Down);
                    } 
                }else{
                    set_door(&level->rooms[add_room[i]],Up);
                    set_door(&level->rooms[add_room[i] - 4],Down);
                }
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 7){
                        set_door(&level->rooms[add_room[i]],Right);
                    }
                }

            }
            if(add_room[i] == 7){
                int state = 1;
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 6){
                        state = 0;
                        set_door(&level->rooms[add_room[i]],Left);
                    }
                }
                if(random_number(0,1) || state){
                    set_door(&level->rooms[add_room[i]],Up);
                    set_door(&level->rooms[add_room[i] - 4],Down);
                }
                
            }
        }
        
    } else {
        for(int i = 4; i < MAX_ROOM; i++){
            level->is_there_room[i] = 1;
            level->rooms[i].start.x = random_number(4,10) + lines/2;
            level->rooms[i].start.y = random_number(10,13) + (i - 4) * (cols / 4);
            level->rooms[i].width = random_number(4,10);
            level->rooms[i].height = random_number(4,15);
            if(i == 4){
                set_door(&level->rooms[i],Right);
            }
            if(i == 5 || i == 6){
                set_door(&level->rooms[i],Right);
                set_door(&level->rooms[i],Left);
            }
            if(i == 7){
                set_door(&level->rooms[i],Left);
            }
        }
        int count = 0;
        int add_room[4];
        select_numbers(add_room,&count,0,3);
        for(int i = 0; i < count; i++){
            level->is_there_room[add_room[i]] = 1;
            level->rooms[add_room[i]].start.x = random_number(4,10) ;
            level->rooms[add_room[i]].start.y = random_number(10,13) + (add_room[i]) * (cols / 4);
            level->rooms[add_room[i]].width = random_number(4,10);
            level->rooms[add_room[i]].height = random_number(4,15);
            if(add_room[i] == 0){
                int state = 1;
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 1){
                        state = 0;
                        set_door(&level->rooms[add_room[i]],Right);
                    }
                }
                if(random_number(0,1) || state){
                    set_door(&level->rooms[add_room[i]],Down);
                    set_door(&level->rooms[add_room[i] +4 ],Up);
                }
                
            }
            int room2_room3 = 0;
            if(add_room[i] == 1){
            
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 0){
                        set_door(&level->rooms[add_room[i]],Left);
                    }
                }
                set_door(&level->rooms[add_room[i]],Down);
                set_door(&level->rooms[add_room[i] +4 ],Up);
                for(int j = 0;j < count; j++){
                if(add_room[j] == 2){
                    if(random_number(0,1)){
                    room2_room3 = 1;
                    set_door(&level->rooms[add_room[i]],Right);
                    }
                    }
                }
            }
            
            if(add_room[i] == 2 ){
                if(room2_room3){
                    set_door(&level->rooms[add_room[i]],Left);
                }
                if(room2_room3){
                   if(random_number(0,1)){
                        set_door(&level->rooms[add_room[i]],Down);
                        set_door(&level->rooms[add_room[i] +4 ],Up);
                    } 
                }else{
                    set_door(&level->rooms[add_room[i]],Down);
                    set_door(&level->rooms[add_room[i] +4 ],Up);
                }
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 3){
                        set_door(&level->rooms[add_room[i]],Right);
                    }
                }

            }
            if(add_room[i] == 3){
                int state = 1;
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 2){
                        state = 0;
                        set_door(&level->rooms[add_room[i]],Left);
                    }
                }
                if(random_number(0,1) || state){
                    set_door(&level->rooms[add_room[i]],Down);
                    set_door(&level->rooms[add_room[i] +4 ],Up);
                }
                
            }
        
        }
    }
}


void printf_level(Level *level){
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true); 
    for(int i = 0 ;i < MAX_ROOM; i++){
        if(level->is_there_room[i] == 1){
                print_room(level->rooms[i]);
        }
    }
    getch();
    refresh();
    endwin();
}


#endif