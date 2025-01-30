#ifndef FILE_H
#define FILE_H

#include "include.h"
#include "datatype.h"

extern char  player_username[MAX_NAME];


void create_folder(const char username[]) {
    char folder_path[256]; 
    strcpy(folder_path, "data/");
    strcat(folder_path, username);
    mkdir(folder_path, 0755);
}

void add_username(char *username){
    FILE *file = fopen("usernames.txt", "a");
    fprintf(file, "%s\n", username);
    fclose(file);
}

void add_info(char username[],char password[],char email[]){
    char folder_path[256];
    strcpy(folder_path, "data/");
    strcat(folder_path,username);
    strcat(folder_path,"/");
    strcat(folder_path,"info.txt");

    char info_file[256];
    strcpy(info_file, "data/");
    strcat(info_file,username);
    strcat(info_file,"/");
    strcat(info_file,"score.txt");
    
    FILE *file = fopen(folder_path, "w");
        fprintf(file, "%s\n", username);
        fprintf(file, "%s\n", password);  
        fprintf(file, "%s\n", email);
    fclose(file);
    FILE *file2 = fopen(info_file, "w");
        fprintf(file2, "%d\n", 0);
        fprintf(file2, "%d\n", 0);  
        fprintf(file2, "%d\n", 0);
        fprintf(file2, "%d\n", 0);
    fclose(file2);
}


void update_score_file(int score_add, int time_add, int gold_add) {

    char score_file[256];
    strcpy(score_file,"data/");
    strcat(score_file,player_username);
    strcat(score_file,"/score.txt");

    FILE *file = fopen(score_file, "r+");

    int score = 0, time_played = 0, gold = 0; int number_of_win = 0;

    fscanf(file, "%d %d %d %d", &score, &time_played, &gold,&number_of_win);

    score += score_add;
    time_played += time_add;
    gold += gold_add;
    number_of_win++;

    rewind(file);
    
    fprintf(file, "%d\n%d\n%d\n%d\n", score, time_played, gold,number_of_win);
    
    fflush(file);
    fclose(file);
}

int read_usernames(const char *filename, char usernames[MAX_PLAYER][MAX_LEN]) {
    FILE *file = fopen(filename, "r");

    int count = 0;
    while (count < MAX_PLAYER && fscanf(file, "%s", usernames[count]) == 1) {
        count++;
    }
    
    fclose(file);
    return count;
}

int get_user_data(const char *username, PlayerScore *playerscore) {
    char filepath[MAX_LEN];
    snprintf(filepath, sizeof(filepath), "data/%s/score.txt", username);

    FILE *file = fopen(filepath, "r");
    if (fscanf(file, "%d %d %d %d", &playerscore->score, &playerscore->total_time, &playerscore->total_gold, &playerscore->num_plays) != 4) {
        fclose(file);
        return 0; 
    }

    strcpy(playerscore->username, username);
    fclose(file);
    return 1;
}

bool is_savefile_exits(const char *username) {
    char file_path[256];
    strcpy(file_path,"data/");
    strcat(file_path,username);
    strcat(file_path,"/save.txt");
    FILE *file = fopen(file_path, "r");
    if (file) {
        fclose(file);
        return true; 
    }
    return false; 
}

void delete_savegane(char username[]){
    char save_path_file[256] = "data/";
    strcat(save_path_file,username);
    strcat(save_path_file,"/save.txt");
    remove(save_path_file);
}
void save_game(Player *player, Level level[MAX_LEVEL    ]) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "data/%s/save.txt", player_username);

    FILE *file = fopen(filepath, "wb");
    if (!file) {
        perror("Error opening file for saving");
        return;
    }

    // Save Player struct
    fwrite(player->username, sizeof(player->username), 1, file);
    fwrite(&player->total_player_moves, sizeof(int), 1, file);
    fwrite(&player->position, sizeof(Point), 1, file);
    fwrite(&player->room, sizeof(int), 1, file);
    fwrite(&player->gold, sizeof(int), 1, file);
    fwrite(&player->health, sizeof(int), 1, file);
    fwrite(&player->strength, sizeof(int), 1, file);
    fwrite(&player->armor, sizeof(int), 1, file);

    // Save arrays and other dynamic fields in Player struct
    fwrite(player->player_weapon, sizeof(Weapon), MAX_ITEM, file);
    fwrite(player->plaeyr_spell, sizeof(Spell), MAX_ITEM, file);
    fwrite(player->player_food, sizeof(Food), MAX_FOOD, file);
    fwrite(&player->number_of_player_food, sizeof(int), 1, file);
    fwrite(&player->number_of_player_spell, sizeof(int), 1, file);
    fwrite(&player->number_of_player_weapon, sizeof(int), 1, file);
    fwrite(player->number_of_each_food, sizeof(int), MAX_FOOD, file);
    fwrite(player->number_of_each_spell, sizeof(int), MAX_ITEM, file);
    fwrite(player->number_of_each_weapon, sizeof(int), MAX_ITEM, file);

    // Save Level data
    for (int i = 0; i < MAX_LEVEL   ; i++) {
        fwrite(&level[i].level, sizeof(int), 1, file);
        fwrite(level[i].is_there_room, sizeof(int), MAX_ROOM, file);

        // Save Room data for each level
        for (int j = 0; j < MAX_ROOM; j++) {
            fwrite(&level[i].rooms[j], sizeof(Room), 1, file);
        }

        // Save treasure room
        fwrite(&level[i].treasure_room, sizeof(Room), 1, file);
    }

    fclose(file);
    printf("Game saved successfully to %s\n", filepath);
}

int load_game(Player *player, Level level[MAX_LEVEL ]) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "data/%s/save.txt", player_username);

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("Error opening save file");
        return 0;  // Failed to load
    }

    // Load Player data
    fread(player->username, sizeof(player->username), 1, file);
    fread(&player->total_player_moves, sizeof(int), 1, file);
    fread(&player->position, sizeof(Point), 1, file);
    fread(&player->room, sizeof(int), 1, file);
    fread(&player->gold, sizeof(int), 1, file);
    fread(&player->health, sizeof(int), 1, file);
    fread(&player->strength, sizeof(int), 1, file);
    fread(&player->armor, sizeof(int), 1, file);

    // Load arrays and other dynamic fields in Player struct
    fread(player->player_weapon, sizeof(Weapon), MAX_ITEM, file);
    fread(player->plaeyr_spell, sizeof(Spell), MAX_ITEM, file);
    fread(player->player_food, sizeof(Food), MAX_FOOD, file);
    fread(&player->number_of_player_food, sizeof(int), 1, file);
    fread(&player->number_of_player_spell, sizeof(int), 1, file);
    fread(&player->number_of_player_weapon, sizeof(int), 1, file);
    fread(player->number_of_each_food, sizeof(int), MAX_FOOD, file);
    fread(player->number_of_each_spell, sizeof(int), MAX_ITEM, file);
    fread(player->number_of_each_weapon, sizeof(int), MAX_ITEM, file);

    // Load Level data
    for (int i = 0; i < MAX_LEVEL   ; i++) {
        fread(&level[i].level, sizeof(int), 1, file);
        fread(level[i].is_there_room, sizeof(int), MAX_ROOM, file);

        // Load Room data for each level
        for (int j = 0; j < MAX_ROOM; j++) {
            fread(&level[i].rooms[j], sizeof(Room), 1, file);
        }

        // Load treasure room
        fread(&level[i].treasure_room, sizeof(Room), 1, file);
    }

    fclose(file);
    printf("Game loaded successfully from %s\n", filepath);
    return 1;  // Success
}


#endif