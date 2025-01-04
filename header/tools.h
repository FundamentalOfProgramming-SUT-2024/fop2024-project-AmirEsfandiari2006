#ifndef TOOL_H
#define TOOL_H

#include "include.h"

int random_number(int min, int max) {

    return rand() % (max - min + 1) + min;
}


#endif