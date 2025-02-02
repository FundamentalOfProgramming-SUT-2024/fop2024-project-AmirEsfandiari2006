#ifndef GENERATEMAP_H
#define GENERATEMAP_H

#include "include.h"
int random_number(int,int);
int random_room(const Level*);
Point random_position_point(const Room*);
void set_up_colors();
bool handle_unicode(char,Point);
int monster_room(Point point, const Level* level);
char* get_monster_name(Monster monster);


Room init_room(Point start,int width,int height,int num_of_door){
    Room room;
    room.start.x = start.x;
    room.start.y = start.y;
    room.width = width;
    room.height = height;
    room.total_places = 0;
    room.total_trap = 0;
    room.is_seen = false;
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
    mvprintw(room.width + room.start.x + 1 , room.start.y , "|");
    mvprintw(room.width + room.start.x + 1 , room.start.y + room.height  + 1, "|");
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
    for(int i = 0; i < room.total_monsters; i++){
        set_up_colors();
        attron(COLOR_PAIR(3));
        mvprintw(room.monsters[i].position.x,room.monsters[i].position.y,"%c",room.monsters[i].display);
        attroff(COLOR_PAIR(3));
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


Point find_empty_for_door(Point point,int dir){
    
    if(dir == Down){
        point.x ++; return point;
    }
    if(dir == Up){
        point.x --; return point;
    }
    if(dir == Right){
        point.y++; return point;
    }
    if(dir == Left){
        point.y--; return point;
    }
}   



void draw_line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (!is_hidden || is_tile_seen[level_map][x1][y1] == true) {
            mvprintw(x1, y1, "#");  
        }

        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void generate_road(Point start, Point end) {
    Point mid;
    mid.x = end.x;
    mid.y = start.y;

    draw_line(start.x, start.y, mid.x, mid.y);
    draw_line(mid.x, mid.y, end.x, end.y);     
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
            if(!is_hidden || level.rooms[i].is_seen == true || level.rooms[i + 1].is_seen == true){
                generate_corridor(find_empty_for_door(level.rooms[i].doors[Right],Right),find_empty_for_door(level.rooms[i + 1].doors[Left],Left));
                level.is_there_corridor[i] = 1;
            }
            
        }
    }
    for(int i = 4; i < 7; i++){
        if(level.rooms[i].is_door[Right] == 1 && level.rooms[i + 1].is_door[Left] == 1){
            if(!is_hidden || level.rooms[i].is_seen == true || level.rooms[i + 1].is_seen == true){
                generate_corridor(find_empty_for_door(level.rooms[i].doors[Right],Right),find_empty_for_door(level.rooms[i + 1].doors[Left],Left));
                level.is_there_corridor[i] = 1;
            }
            
        }
    }
    for(int i = 0; i < 4; i++){
        if(level.rooms[i].is_door[Down] == 1 && level.rooms[i + 4].is_door[Up] == 1){
            if(!is_hidden || level.rooms[i].is_seen == true || level.rooms[i + 4].is_seen == true){
                generate_corridor(find_empty_for_door(level.rooms[i].doors[Down],Down),find_empty_for_door(level.rooms[i + 4].doors[Up],Up));
                level.is_there_corridor[i] = 1;
            }

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
    for(int i = lines - 20; i <= lines; i++){
        for(int j = 0; j <= 4; j++){
            mvprintw(i,j," ");
        }
    }
}

void print_message(char message[]){
    mvprintw(1,1,"%s",message);
}

char* current_weapon(char symbol){
    switch (symbol)
    {
    case ' ': return "Nothing";
    case 'M': return "Mace";
    case 'S': return "Sword";    
    }
}

void print_status(const Player* player){
    mvprintw(lines - 1,cols/2 - 42,"                                                                                    ");
    mvprintw(lines - 1,cols/2 - 42,"Health: %d",player->health);
    mvprintw(lines - 1,cols/2 - 22,"Gold: %d"  ,player->gold);
    mvprintw(lines - 1,cols/2 - 2,"Strength: %d ",player->strength + damage_spell_bonus);
    mvprintw(lines - 1,cols/2 + 10,"(%s)",current_weapon(player->current_weapon.symbol));
    mvprintw(lines - 1,cols/2 + 26,"Hunger: %d",player->hunger);
}

void printf_level(const Level* level,const Player* player){
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true); 
    if(is_hidden == false){
        for(int i = 0 ;i < MAX_ROOM; i++){
        if(level->is_there_room[i] == 1){
                    print_room(level->rooms[i]);
            }
        }
    } else {
        for(int i = 0 ;i < MAX_ROOM; i++){
            if(level->is_there_room[i] == 1 && level->rooms[i].is_seen == true){
                    print_room(level->rooms[i]);
            }
            for(int j = 0; j < MAX_DOOR; j++){
                for(int z1 = -1; z1 <= 1; z1++){
                    for(int z2 = -1; z2 <= 1; z2++){
                        if((level->rooms[i].is_door[j] == true) && (player->position.x + z1 == level->rooms[i].doors[j].x)  && (player->position.y  + z2 == level->rooms[i].doors[j].y))
                            mvprintw(level->rooms[i].doors[j].x,level->rooms[i].doors[j].y,"+");
                    }
                }
            }   
        }
        print_room(level->rooms[player->room]);
    }
    

    set_up_corridors(*level);

    char game_difficulty[MAX_LENGTH];
    switch(game_diff){
        case -1: strcpy(game_difficulty,"Easy");   break;
        case 0: strcpy(game_difficulty,"Normal"); break;
        case 1: strcpy(game_difficulty,"Hard"); break;

    }
    mvprintw(1,cols - 30,"Game difficulty: %s",game_difficulty);
    mvprintw(lines - 1,cols - 25,"You're at level: %d Room:%d",level_map + 1,player->room + 1);
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
                    int color = 6;
                    char weapon = weapons[random_number(0,weapons_number -1)];
                    level->rooms[item_room].places[level->rooms[item_room].total_places].display = weapon;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].position = item_position;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].color = color;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].is_it_thrown = false;
                    level->rooms[item_room].total_places++;
                }
        }
    }
    }
}

void random_spell(Level *level,int max_number,int max_chance){
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
                    int color = 6;
                    int start,end;
                    char sp = spell[random_number(0,weapons_number -1)];
                    level->rooms[item_room].places[level->rooms[item_room].total_places].display = sp;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].position = item_position;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].color = color;
                    level->rooms[item_room].total_places++;
                }
        }
    }
    }
}


void random_food(Level *level,int max_number,int max_chance){
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
                    int color = 6;
                    int start,end;
                    char food_sym = food[random_number(0,food_number -1)];
                    level->rooms[item_room].places[level->rooms[item_room].total_places].display = food_sym;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].position = item_position;
                    level->rooms[item_room].places[level->rooms[item_room].total_places].color = color;
                    level->rooms[item_room].total_places++;
                }
        }
    }
    }
}



void random_gold(Level *level,int number){
    for(int gold_room = 0; gold_room < MAX_ROOM; gold_room++){
        if(level->is_there_room[gold_room] == true){
            for(int repeat = 0; repeat < random_number(0,number) - game_diff; repeat++){
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

void random_trap(Level *level,int max_number,int max_chance){
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
                        if (level->rooms[item_room].traps[i].position.x == item_position.x &&
                            level->rooms[item_room].traps[i].position.y == item_position.y) {
                            position_found = false; 
                            total_attmpt--;
                            break;
                        }
                    }
                }
                    level->rooms[item_room].traps[level->rooms[item_room].total_trap].position = item_position;
                    level->rooms[item_room].total_trap++;
                }
        }
    }
    }
}

void random_gold_room(Room*room,int number){
            for(int repeat = 0; repeat < number - 10 * game_diff; repeat++){
                Point gold_position;
                bool position_found = false;
                int total_attmpt =  1000;
                while (!position_found && total_attmpt >= 0) {
                    gold_position = random_position_point(room);
                    position_found = true; 
                    for (int i = 0; i < room->total_places; i++) {
                        if (room->places[i].position.x == gold_position.x &&
                            room->places[i].position.y == gold_position.y) {
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
                    room->places[room->total_places].display = 'G';
                    room->places[room->total_places].position = gold_position;
                    room->places[room->total_places].color = color;
                    room->places[room->total_places].amout = random_number(start,end);
                    room->total_places++;
                }
}

void random_monster(Level *level,int max_number,int max_chance){
    for(int item_room = 0; item_room < MAX_ROOM; item_room++){
        if(random_number(0,max_chance)){
        if(level->is_there_room[item_room] == true){
            for(int repeat = 0; repeat < random_number(0,max_number) + game_diff + 1; repeat++){
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
                    for (int i = 0; i < level->rooms[item_room].total_monsters; i++) {
                        if (level->rooms[item_room].monsters[i].position.x == item_position.x &&
                            level->rooms[item_room].monsters[i].position.y == item_position.y) {
                            position_found = false; 
                            total_attmpt--;
                            break;
                        }
                    }
                }
                    int which = random_number(0,MAX_MONSTER -1);
                    level->rooms[item_room].monsters[level->rooms[item_room].total_monsters].display = monster_name[which];
                    level->rooms[item_room].monsters[level->rooms[item_room].total_monsters].position = item_position;
                    level->rooms[item_room].monsters[level->rooms[item_room].total_monsters].damage = monster_damge[which];
                    level->rooms[item_room].monsters[level->rooms[item_room].total_monsters].health = monster_health[which];
                    level->rooms[item_room].monsters[level->rooms[item_room].total_monsters].moves = monster_moves[which];
                    level->rooms[item_room].monsters[level->rooms[item_room].total_monsters].room = monster_room(item_position,&(level[level_map]));
                    level->rooms[item_room].total_monsters++;
                }
        }
    }
    }
}



void init_treasure_room(Room *room){
    Point treasure_room_pos = {lines/3 - 5,cols/3 - 5};
    room->start = treasure_room_pos;
    room->width = 20;
    room->height = 60;
    room->num_of_door = 0;
    random_gold_room(room, 20);
}

void print_treasure_room(Level * level,Player *player){
    set_up_colors();
    attron(COLOR_PAIR(7));
    print_room(level->treasure_room);
    attroff(COLOR_PAIR(7));
    
    char game_difficulty[MAX_LENGTH];
    switch(game_diff){
        case -1: strcpy(game_difficulty,"Easy");   break;
        case 0: strcpy(game_difficulty,"Normal"); break;
        case 1: strcpy(game_difficulty,"Hard"); break;

    }
    mvprintw(1,cols - 30,"Game difficulty: %s",game_difficulty);
    mvprintw(lines - 1,cols - 25,"You're at Treasure room.");
    print_status(player);
    clear_baord();
}

void init_level(Level level[]){

    init_treasure_room(&(level->treasure_room));

    for(int i = 0; i < MAX_LEVEL - 3;i++){
        generate_random_room(&level[i]);
    }
        for(int i = 0; i < MAX_LEVEL - 3; i++){
            for(int j = 0; j < random_number(2,4); j++){
                random_place(&level[i],'O',3);
            }
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        random_gold(&level[i],3);
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        
        if(i !=  3){
            random_place(&level[i],'>', 2);
        } else {
            random_place(&level[i],'^',2);
        }
        if(i != 0){
            random_place(&level[i],'<', 2);
        }
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        random_weapon(&level[i],1,3);
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        random_spell(&level[i],1,3);
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        random_food(&level[i],1,3);
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        random_trap(&level[i],1,3);
    }
    for(int i = 0; i < MAX_LEVEL - 3; i++){
        random_monster(&level[i],1,3);
    }


}

void attack_player(Monster* monster, Player* player){
    player->health -= monster->damage;
    attron(COLOR_PAIR(3));
    mvprintw(2,1,"The Monster: %s, gives you %d damage!",get_monster_name(*monster),monster->damage);
    attroff(COLOR_PAIR(3));
}


int is_walkable(int x, int y) {
    return ((mvinch(x, y) & A_CHARTEXT ) != 'O') && ((mvinch(x, y) & A_CHARTEXT )!= '@') ;
}

void moveMonster(Monster* monster_man, Player* player_man) {
    Point* player = &player_man->position;
    Point* monster = &monster_man->position;

    int dx = player->x - monster->x;
    int dy = player->y - monster->y;
    if (abs(dx) <= 1 && abs(dy) <= 1) {
        attack_player(monster_man,player_man);
        return;
    }

    if(monster_man->moves <= 0){
        return;
    }


    int new_x = monster->x;
    int new_y = monster->y;

    if (dx > 0 && is_walkable(monster->x + 1, monster->y)) new_x++;
    else if (dx < 0 && is_walkable(monster->x - 1, monster->y)) new_x--;

    if (dy > 0 && is_walkable(new_x, monster->y + 1)) new_y++;
    else if (dy < 0 && is_walkable(new_x, monster->y - 1)) new_y--;

    if (is_walkable(new_x, new_y)) {
        monster->x = new_x;
        monster->y = new_y;
    }
    monster_man->moves -= 1;
}

void handle_monsters_movement(Level*level,Player*player){
    
    for(int i = 0; i < level[level_map].rooms[player->room].total_monsters; i++){
        if(player->room != -1 && level[level_map].rooms[player->room].monsters[i].health > 0){
            moveMonster(&(level[level_map].rooms[player->room].monsters[i]),player);
        }
    }

}



#endif