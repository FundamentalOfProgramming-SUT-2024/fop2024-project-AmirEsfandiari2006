#ifndef GENERATEMAP_H
#define GENERATEMAP_H

#include "include.h"
int random_number(int,int);
int random_room(const Level*);
Point random_position_point(const Room*);
void set_up_colors();
bool handle_unicode(char,Point);


Room init_room(Point start,int width,int height,int num_of_door){
    Room room;
    room.start.x = start.x;
    room.start.y = start.y;
    room.width = width;
    room.height = height;
    room.total_places = 0;
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
        mvprintw(start_x,start_y,"_");
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
        mvprintw(start_x,start_y,"_");
        start_y++;
    }
    for(int i = 0; i < MAX_DOOR; i++){
        if(room.is_door[i] == true)
        mvprintw(room.doors[i].x,room.doors[i].y,"+");
    }
    for(int i = 0; i < room.total_places; i++){
        set_up_colors();
        attron(COLOR_PAIR(room.places[i].color));
        if(!handle_unicode(room.places[i].display,room.places[i].position)){
            mvprintw(room.places[i].position.x,room.places[i].position.y,"%c",room.places[i].display);
        }
        attroff(COLOR_PAIR(room.places[i].color));
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
        int room5_room6 = 0;
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
            if(add_room[i] == 5){
            
                for(int j = 0;j < count; j++){
                    if(add_room[j] == 4){
                        set_door(&level->rooms[add_room[i]],Left);
                    }
                }
                set_door(&level->rooms[add_room[i] - 4],Down);
                set_door(&level->rooms[add_room[i]],Up);

            }
            
            if(add_room[i] == 6 ){
                if(room5_room6 == 1){
                    set_door(&level->rooms[add_room[i]],Left);
                }
                if(room5_room6 == 1){
                   if(random_number(0,1)){
                        set_door(&level->rooms[add_room[i] - 4],Down);
                        set_door(&level->rooms[add_room[i]],Up);
                    } 
                }else{
                    set_door(&level->rooms[add_room[i] - 4],Down);
                    set_door(&level->rooms[add_room[i]],Up);
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
            level->rooms[i].start.x = random_number(4,8) + lines/2;
            level->rooms[i].start.y = random_number(5,8) + (i - 4) * (cols / 4);
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
                    int room2_room3 = 0;
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
                    set_door(&level->rooms[add_room[i] + 4],Up);
                }
                
            }
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


Point find_empty_for_door(Point point){
    if(mvinch(point.x + 1,point.y) == ' '){
        point.x ++; return point;
    }
    if(mvinch(point.x - 1,point.y) == ' '){
        point.x --; return point;
    }
    if(mvinch(point.x,point.y + 1) == ' '){
        point.y++; return point;
    }
    if(mvinch(point.x,point.y - 1) == ' '){
        point.y--; return point;
    }
}   



void generate_road(Point start, Point end) {
    Point mid;


    mid.x = end.x;
    mid.y = start.y;

    int dx = abs(mid.x - start.x);
    int dy = abs(mid.y - start.y);
    int sx = (start.x < mid.x) ? 1 : -1;
    int sy = (start.y < mid.y) ? 1 : -1;

    int x = start.x, y = start.y;
    while (true) {
        mvprintw(x, y, "#");
        if (x == mid.x && y == mid.y) break;

        if (dx >= dy) {
            x += sx;
        } else {
            y += sy;
        }
    }

    dx = abs(end.x - mid.x);
    dy = abs(end.y - mid.y);
    sx = (mid.x < end.x) ? 1 : -1;
    sy = (mid.y < end.y) ? 1 : -1;

    x = mid.x, y = mid.y;
    while (true) {
        mvprintw(x, y, "#"); 
        if (x == end.x && y == end.y) break;

        if (dx >= dy) {
            x += sx;
        } else {
            y += sy;
        }
    }
}


void generate_corridor(Point start, Point end) {

    Point mid1;

    mid1.x = (start.x + end.x) / 2;
    mid1.y = (start.y + end.y) / 2;
    

    generate_road(start, mid1); 
    generate_road(mid1, end);
}

void set_up_corridors(Level level){
    for(int i = 0; i < 3; i++){
        if(level.rooms[i].is_door[Right] == 1 && level.rooms[i + 1].is_door[Left] == 1){
            generate_corridor(find_empty_for_door(level.rooms[i].doors[Right]),find_empty_for_door(level.rooms[i + 1].doors[Left]));
            level.is_there_corridor[i] = 1;
        }
    }
    for(int i = 4; i < 7; i++){
        if(level.rooms[i].is_door[Right] == 1 && level.rooms[i + 1].is_door[Left] == 1){
            generate_corridor(find_empty_for_door(level.rooms[i].doors[Right]),find_empty_for_door(level.rooms[i + 1].doors[Left]));
            level.is_there_corridor[i] = 1;
        }
    }
    for(int i = 0; i < 4; i++){
        if(level.rooms[i].is_door[Down] == 1 && level.rooms[i + 4].is_door[Up] == 1){
            generate_corridor(find_empty_for_door(level.rooms[i].doors[Down]),find_empty_for_door(level.rooms[i + 4].doors[Up]));
            level.is_there_corridor[i + 7] = 1;
        }
    }
}


void clear_baord(){
    for(int i = 0; i < cols; i++){
        mvprintw(0,i,"%c",' ');
    }
    for(int i = 1; i < cols; i++){
        mvprintw(0,i,"%c",' ');
    }
    for(int i = lines - 10; i <= lines; i++){
        for(int j = 0; j <= 4; j++){
            mvprintw(i,j," ");
        }
    }
}

void print_message(char message[]){
    mvprintw(1,1,"%s",message);
}


void print_status(const Player* player){
    mvprintw(lines - 1,cols/2 - 38,"Health: %d",player->health);
    mvprintw(lines - 1,cols/2 - 18,"Gold: %d"  ,player->gold);
    mvprintw(lines - 1,cols/2 + 2,"Strength: %d",player->strength);
    mvprintw(lines - 1,cols/2 + 22,"Armor: %d",player->armor);
}

void printf_level(const Level* level,const Player* player){
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true); 
    for(int i = 0 ;i < MAX_ROOM; i++){
        if(level->is_there_room[i] == 1){
                print_room(level->rooms[i]);
        }
    }
    set_up_corridors(*level);

    char game_difficulty[MAX_LENGTH];
    switch(game_diff){
        case 0: strcpy(game_difficulty,"Easy");   break;
        case 1: strcpy(game_difficulty,"Normal"); break;
        case 2: strcpy(game_difficulty,"Hard"); break;

    }
    mvprintw(1,cols - 30,"Game difficulty: %s",game_difficulty);
    mvprintw(lines - 1,cols - 25,"You're at level: %d",level_map + 1);
    print_status(player);
    clear_baord();
}

void random_place(Level* level, char ch,int color) {
    int stair_room = random_room(level);
    Point stair_position;
    bool position_found = false;
    int total_attmpt =  1000;
    while (!position_found && total_attmpt >= 0) {
        stair_room = random_room(level);
        stair_position = random_position_point(&level->rooms[stair_room]);
        position_found = true; 
        for (int i = 0; i < level->rooms[stair_room].total_places; i++) {
            if (level->rooms[stair_room].places[i].position.x == stair_position.x &&
                level->rooms[stair_room].places[i].position.y == stair_position.y) {
                position_found = false; 
                total_attmpt--;
                break;
            }
        }
    }
    level->rooms[stair_room].places[level->rooms[stair_room].total_places].display = ch;
    level->rooms[stair_room].places[level->rooms[stair_room].total_places].position = stair_position;
    level->rooms[stair_room].places[level->rooms[stair_room].total_places].color = color;
    level->rooms[stair_room].total_places++;
}

void random_weapon(Level *level,int max_number,int max_chance){
    for(int item_room = 0; item_room < MAX_ROOM; item_room++){
        if(random_number(0,max_chance)){
        if(level->is_there_room[item_room] == true){
            for(int repeat = 0; repeat < random_number(0,max_number) - game_diff; repeat++){
                Point item_position;
                bool position_found = false;
                int total_attmpt =  1000;
                while (!position_found && total_attmpt >= 0) {
                    item_position = random_position_point(&level->rooms[item_room]);
                    position_found = true; 
                    for (int i = 0; i < level->rooms[item_room].total_places; i++) {
                        if (level->rooms[item_room].places[i].position.x == item_position.x &&
                            level->rooms[item_room].places[i].position.y == item_position.y) {
                            position_found = false; 
                            total_attmpt--;
                            break;
                        }
                    }
                }
                    int color = 1;
                    int start,end;
                    char weapon = weapons[random_number(0,weapons_number -1)];
                    level->rooms[item_room].places[level->rooms[item_room].total_places].display = weapon;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].position = item_position;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].color = color;
                    level->rooms[item_room].total_places++;
                }
        }
    }
    }
}

void random_gold(Level *level){
    for(int gold_room = 0; gold_room < MAX_ROOM; gold_room++){
        if(level->is_there_room[gold_room] == true){
            for(int repeat = 0; repeat < random_number(0,3) - game_diff; repeat++){
                Point gold_position;
                bool position_found = false;
                int total_attmpt =  1000;
                while (!position_found && total_attmpt >= 0) {
                    gold_position = random_position_point(&level->rooms[gold_room]);
                    position_found = true; 
                    for (int i = 0; i < level->rooms[gold_room].total_places; i++) {
                        if (level->rooms[gold_room].places[i].position.x == gold_position.x &&
                            level->rooms[gold_room].places[i].position.y == gold_position.y) {
                            position_found = false; 
                            total_attmpt--;
                            break;
                        }
                    }
                }
                    int color;
                    int start,end;
                    if(!random_number(0,9)){
                        color = 5; start = 30; end = 60;
                    } else {
                        color = 4; start = 10; end = 30;
                    }
                    level->rooms[gold_room].places[level->rooms[gold_room].total_places].display = 'G';
                    level->rooms[gold_room].places[level->rooms[gold_room].total_places].position = gold_position;
                    level->rooms[gold_room].places[level->rooms[gold_room].total_places].color = color;
                    level->rooms[gold_room].places[level->rooms[gold_room].total_places].amout = random_number(start,end);
                    level->rooms[gold_room].total_places++;
                }
        }
    }
}


void init_level(Level level[]){

    for(int i = 0; i < MAX_LEVEL - 3;i++){
        generate_random_room(&level[i]);
    }
        for(int i = 0; i < MAX_LEVEL - 3; i++){
            for(int j = 0; j < random_number(2,4); j++){
                random_place(&level[i],'O',3);
            }
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        random_gold(&level[i]);
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        
        if(i !=  3){
            random_place(&level[i],'>', 2);
        }
        if(i != 0){
            random_place(&level[i],'<', 2);
        }
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        random_weapon(&level[i],1,3/*1*/);
    }
        
}

#endif