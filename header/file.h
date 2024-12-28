#include <sys/stat.h>
#include <string.h>

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
    FILE *file = fopen(folder_path, "w");
    fprintf(file, "%s\n", username);  // Write username followed by a newline
    fprintf(file, "%s\n", password);  // Write password followed by a newline
    fprintf(file, "%s\n", email);
    fclose(file);
}