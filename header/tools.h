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
    init_pair(6,COLOR_WHITE,COLOR_BLACK);
    init_pair(7,186,COLOR_BLACK);
    init_pair(8,51,COLOR_BLACK);
    init_pair(9,94,COLOR_BLACK);
    }



int random_number(int min, int max) {
    if (min > max) {
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

void sort_leaderboard(PlayerScore leaderboard[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (leaderboard[i].score < leaderboard[j].score) {
                PlayerScore temp = leaderboard[i];
                leaderboard[i] = leaderboard[j];
                leaderboard[j] = temp;
            }
        }
    }
}

void reset_player(Player *player){
    memset(player, 0, sizeof(*player));
}

void reset_level(Level level[]){
    for(int i = 0; i < MAX_LEVEL; i++){
        memset(&level[i], 0, sizeof(level[i]));
    } 
}

int monster_room(Point point, const Level* level) {
    for (int i = 0; i < MAX_ROOM; i++) {
        if (level->is_there_room[i] == 1) {
            if (point.x > level->rooms[i].start.x &&
                point.x <= level->rooms[i].start.x + level->rooms[i].width &&
                point.y > level->rooms[i].start.y &&
                point.y <= level->rooms[i].start.y + level->rooms[i].height) { 
                return i;         
            }
        }
    }
}

char* get_monster_name(Monster monster){
    switch(monster.display){
        case 'E': return "Deamon";
        case 'B': return "Fire Breathing";
        case 'I': return "Giant";
        case 'N': return "Snake";
        case 'U': return "Undeed";
    }
}

Monster* get_monster_by_point(Point point,Level* level,Player* player){
    for(int i = 0; i < level[level_map].rooms[player->room].total_monsters; i++){
        if(point.x == level[level_map].rooms[player->room].monsters[i].position.x && 
           point.y == level[level_map].rooms[player->room].monsters[i].position.y) {
                return &level[level_map].rooms[player->room].monsters[i];
           }
    }
    return NULL;
}

void handle_time(){
    if(damage_spell_life > 0){
            damage_spell_life--;
    }
    if(health_spell_life > 0){
            health_spell_life--;
    }
    if(speed_spell_life > 0){
            speed_spell_life--;
    }
}

void *play_music(void *arg) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
        return NULL;
    }
    
    Mix_Music *music = Mix_LoadMUS(music_paths[selected_music]); 
    if (!music) {
        printf("Error loading music: %s\n", Mix_GetError());
        return NULL;
    }

    while (is_game_playing && is_exit != true) { 
        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(music, -1); 
        }
        SDL_Delay(500);
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    return NULL;
}

void *play_music_mainmenu(void *arg) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
        return NULL;
    }

    Mix_Music *music = Mix_LoadMUS(main_menu_music); 
    if (!music) {
        printf("Error loading music: %s\n", Mix_GetError());
        return NULL;
    }

    while (!is_game_playing && is_exit != true) { 
        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(music, -1); 
        }
        SDL_Delay(500);
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    return NULL;
}





#endif


