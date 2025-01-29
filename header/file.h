#ifndef FILE_H
#define FILE_H

#include "include.h"

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



#endif