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
    int amout;
    char display;
    Point position;
    int color;

} Place;

typedef struct
{
    Point start;
    int width;
    int height;
    int num_of_door;
    int total_places;
    bool is_door[MAX_DOOR];
    Point doors[MAX_DOOR];
    Place places[MAX_PLACE];    
} Room;

typedef struct 
{
    int level;

    int is_there_room[MAX_ROOM];
    Room rooms[MAX_ROOM];

    int is_there_corridor[MAX_ROOM];

    

} Level;


typedef struct {
    char name[MAX_NAME];
    char symbol;
} Weapon;

typedef struct {
    char name[MAX_NAME];
    char symbol;
} Spell;

typedef struct 
{
    Point position;
    int room;
    int gold;
    int health;
    int strength;
    int armor;
    Weapon player_weapon[MAX_ITEM];
    Spell plaeyr_spell [MAX_ITEM];
    int number_of_player_spell;
    int number_of_player_weapon;
  
} Player;



#endif