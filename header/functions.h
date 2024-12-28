#include <ncurses.h>
#include "constants.h"

void draw_borders(){
    for(int i = 0; i < COLS; i++){
        mvprintw(0,i,"#");
        mvprintw(LINES - 1,i,"#"); 
    }
    for(int i = 0; i < LINES; i++){
        mvprintw(i,0,"|");
        mvprintw(i,COLS - 1,"|"); 
    }
}

int get_command_main_menu(){
    const char* menu_item[] = {"Log in","Sign up","History","Setting","Exit"};
    const int number_menu_item = 5;
    initscr();
    noecho();
    keypad(stdscr, true); 
    curs_set(0);
    int hover = 0;
    while(true){
        draw_borders();
        for(int i = 0; i < number_menu_item; i++){
        if(i == hover)
            attron(A_REVERSE);
        mvprintw(LINES/2+i - number_menu_item,COLS/2 -number_menu_item,"%s\n",menu_item[i]);
        if(i == hover)
            attroff(A_REVERSE);
        }
        int command = getch();
        if (command == KEY_UP)
            hover = (hover == 0) ? number_menu_item - 1: hover - 1;
        else if (command == KEY_DOWN)
            hover = (hover == number_menu_item - 1) ? 0 : hover + 1;
        else if (command == ENTER){
            endwin();
            return hover;
        }
            
        clear();
    }
    refresh();
    endwin();
}