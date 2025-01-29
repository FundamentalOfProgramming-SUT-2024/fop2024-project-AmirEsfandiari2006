#ifndef DATATYPE_H
#define DATATYPE_H

#define MAX_LENGTH 1000


typedef struct {
    char username[MAX_LEN];
    int score;
    int total_time;
    int total_gold;
    int num_plays;
} PlayerScore;


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
    Point position;
    int damage;
} Trap;


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
    Trap traps[MAX_PLACE];
    int total_trap;
} Room;

typedef struct 
{
    int level;

    int is_there_room[MAX_ROOM];
    Room rooms[MAX_ROOM];

    int is_there_corridor[MAX_ROOM];

    Room treasure_room;

} Level;


typedef struct {
    char name[MAX_NAME];
    char symbol;
} Weapon;

typedef struct {
    char name[MAX_NAME];
    char symbol;
} Spell;


typedef struct {
    char name[MAX_NAME];
    char symbol;
} Food;

typedef struct 
{

    char username[MAX_NAME];
    int total_player_moves;

    Point position;
    int room;
    int gold;
    int health;
    int strength;
    int armor;
    Weapon player_weapon[MAX_ITEM];
    Spell plaeyr_spell [MAX_ITEM];
    Food player_food[MAX_FOOD];
    int number_of_player_food;
    int number_of_player_spell;
    int number_of_player_weapon;
    int number_of_each_food[MAX_FOOD];
    int number_of_each_spell[MAX_ITEM];
    int number_of_each_weapon[MAX_ITEM];

} Player;



#endif