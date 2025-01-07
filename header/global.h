#ifndef GLOBAL_H
#define GLOBAL_H

#include "include.h"



bool  is_logged_in = false;
bool  is_game_playing = false;
bool  can_get_item = true;


int level_map = 0;
int game_diff = 0;
int player_color = 2;

int cols;
int lines;

char weapons[MAX_ITEM] = {'M','D','W','R','S'};
const int  weapons_number = 5;

char spell[MAX_ITEM] = {'H','Z','C'};
const int spell_number = 3;

char food[MAX_ITEM] = {'F'};
const int food_number = 1;

#endif