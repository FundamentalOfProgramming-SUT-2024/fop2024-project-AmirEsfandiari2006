#ifndef DATATYPE_H
#define DATATYPE_H

#define MAX_LENGTH 1000

typedef struct 
{
    char username[MAX_LENGTH];
    int playing_time;
    int total_gold;
    int number_of_end;

} PlayerInfo;


typedef struct 
{
    int x;
    int y;

} Point;


typedef struct
{
    Point start;
    int width;
    int height;
    int num_of_door;
    bool is_door[MAX_DOOR];
    Point doors[MAX_DOOR];
    
} Room;

typedef struct 
{
    int level;

    int is_there_room[MAX_ROOM];
    Room rooms[MAX_ROOM];

    int is_there_corridor[MAX_ROOM];
    

} Level;

typedef struct 
{
    Point position;
    int room;
    int gold;
    int health;
    int strength;
  
} Player;


#endif