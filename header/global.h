#ifndef GLOBAL_H
#define GLOBAL_H

#include "include.h"

char player_username[MAX_NAME];

bool  is_logged_in = false;
bool  is_game_playing = false;
bool  is_game_ended = false;
bool  is_there_savegame = false;
bool  is_treasure_room = false;
bool  is_game_stop = true;
bool  can_get_item = true;



bool have_game_played = false;

bool  player_new_game = false;
bool  player_load_game = false;

int level_map = 0;
int game_diff = 0;
int player_color = 2;

int cols;
int lines;

const int  weapons_number = 5;
char weapons[MAX_ITEM] = {'M','D','W','R','S'};
int weapons_damage[MAX_ITEM] = {5,12,15,5,10};


char spell[MAX_ITEM] = {'H','Z','C'};
const int spell_number = 3;

char food[MAX_ITEM] = {'F'};
const int food_number = 1;

const char monster_name[MAX_MONSTER] = {'E','B','I','N','U'};
const char monster_damge[MAX_MONSTER] = {1,2,3,4,5};
const char monster_health[MAX_MONSTER] = {5,10,15,20,30};


int damage_spell_life = 0;
int health_spell_life = 0;
int speed_spell_life = 0;

int health_spell_bonus= 1;
int damage_spell_bonus = 0;
int speed_spell_bonus = 0;


bool throw_dart = false;

#endif