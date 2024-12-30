#ifndef DATATYPE_H
#define DATATYPE_H

#define MAX_LENGTH 1000

typedef struct 
{
    char username[MAX_LENGTH];
    int playing_time;
    int total_gold;
    int number_of_end;

} Player;

#endif