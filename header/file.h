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

bool is_savefile_exits() {
    char file_path[256];
    strcpy(file_path,"data/");
    strcat(file_path,player_username);
    strcat(file_path,"/save.data");
    FILE *file = fopen(file_path, "r");
    if (file != NULL) {
        fclose(file);
        return true; 
    }
    return false; 
}

void delete_savegane(char username[]){
    char save_path_file[256] = "data/";
    strcat(save_path_file,username);
    strcat(save_path_file,"/save.data");
    remove(save_path_file);
}



#endif