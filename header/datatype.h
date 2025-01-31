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
    bool is_it_thrown;
    int color;

} Place;

typedef struct 
{
    Point position;
    int damage;
} Trap;

typedef struct
{
    Point position;
    char display;
    int room;
    int health;
    int damage;
} Monster;



typedef struct
{
    Point start;
    int width;
    int height;
    int num_of_door;
    bool is_door[MAX_DOOR];
    Point doors[MAX_DOOR];
    Place places[MAX_PLACE];
    Trap traps[MAX_PLACE];
    Monster monsters[MAX_MONSTER];
    int total_places;
    int total_monsters;
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
    int damage;
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
    int hunger;
    Weapon player_weapon[MAX_ITEM];
    Weapon current_weapon;
    int thrown_weapon;
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