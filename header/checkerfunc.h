#ifndef CHECKERFUNC_H
#define CHECKERFUNC_H


#include "include.h"

bool valid_username(char username[]) {
    if (strlen(username) == 0) {
        return false; 
    }

    FILE *file = fopen("usernames.txt", "r");

    char line[MAX_INPUT_SIZE];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strcmp(username, line) == 0) {
            fclose(file);
            return false; 
        }
    }

    fclose(file);
    return true;  
}

bool valid_password(char password[]){
    bool upper_case = false;
    bool lower_case = false;
    bool number = false;
    bool length = strlen(password) > 7 ? true : false;
    int index = 0;
    while(password[index] != '\0'){
        if(password[index] >= 'a' && password[index] <= 'z')
            lower_case = true;
        if(password[index] >= 'A' && password[index] <= 'Z')
            upper_case = true;
        if(password[index] >= '0' && password[index] <= '9')
            number = true;
        index++;    
    }
    if(upper_case && lower_case && number && length){
        return true;
    } else{
        return false;
    }
}

bool valid_email(const char email[]) {
    int at_index = -1, dot_index = -1;
    int length = strlen(email);

    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            if (at_index != -1 || i == 0 || i == length - 1) {
                return false;
            }
            at_index = i;
        } else if (email[i] == '.') {
            if (i == 0 || i == length - 1 || (at_index != -1 && at_index == i - 1)) {
                return false;
            }
            dot_index = i;
        } else if (!(email[i] >= 'a' && email[i] <= 'z') && !(email[i] >= 'A' && email[i] <= 'Z') &&
                   !(email[i] >= '0' && email[i] <= '9') && email[i] != '-' && email[i] != '_') {
            return false;
        }
    }

    if (at_index == -1 || dot_index == -1 || dot_index < at_index) {
        return false;
    }

    int ll = at_index; 
    int dl = dot_index - at_index - 1; 
    int tl = length - dot_index - 1;

    if (ll >= 3 && dl >= 1 && tl >= 2) {
        return true;
    }
    return false;
}

bool check_password_for_username(char username[], char password[]){
    char folder_path[256];
    strcpy(folder_path, "data/");
    strcat(folder_path,username);
    strcat(folder_path,"/");
    strcat(folder_path,"info.txt");
    FILE *file = fopen(folder_path, "r");
    char line[MAX_INPUT_SIZE];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strcmp(password, line) == 0) {
            fclose(file);
            return true; 
        }
    }
    fclose(file);
    return false;
}


#endif