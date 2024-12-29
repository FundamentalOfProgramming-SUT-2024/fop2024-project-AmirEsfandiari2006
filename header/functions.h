#ifndef FUNCTION_H
#define FUNCTION_H

#include <ncurses.h>
#include "constants.h"
#include "checkerfunc.h"
#include "file.h"

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
    const char* menu_item[] = {"Play","Sign up","History","Setting","Exit"};
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
            clear();
            endwin();
            return hover;
        }
            
        clear();
    }
    refresh();
    endwin();
}

void sign_up_user(){

    char username[MAX_INPUT_SIZE];
    char password[MAX_INPUT_SIZE];
    char email[MAX_INPUT_SIZE];

    initscr();
    echo();
    curs_set(1);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    draw_borders();
    const int top = LINES/2 - 5;
    while(true){
        int state = 1;
        mvprintw(top + 0,COLS/2 - 40 , "%s","Enter your Username: ");
        mvprintw(top + 2,COLS/2 - 40 , "%s","Enter your Password: ");
        mvprintw(top + 4,COLS/2 - 40 , "%s","Enter your Email address: ");
        mvprintw(top + 9,COLS/2 - 40 , "%s","*Your password must have at least 7 characters, consist of one upper,lower and number.");

        mvscanw(top + 0,COLS/2 , "%s", username);
        mvscanw(top + 2,COLS/2 , "%s", password);
        mvscanw(top + 4,COLS/2 , "%s", email);

        if(!valid_username(username)){
            attron(COLOR_PAIR(1));
            mvprintw(top + 11,COLS/2 - 40 ,"This username is already defined." );
            state = 0;
            attroff(COLOR_PAIR(1)); 
        } else {
            mvprintw(top + 11,COLS/2 - 40,"                                  ");
        }
        if(!valid_password(password)){
            attron(COLOR_PAIR(1));
            mvprintw(top + 13,COLS/2 - 40 ,"Your Password doesn't meet the requirments." );
            state = 0;
            attroff(COLOR_PAIR(1));
        } else {
            mvprintw(top + 13,COLS/2 - 40 ,"                                           " );
        }
        if(!valid_email(email)){
            attron(COLOR_PAIR(1));
            mvprintw(top + 15,COLS/2 - 40 ,"Invalid Email." );
            state = 0;
            attroff(COLOR_PAIR(1));
        } else{
            mvprintw(top + 15,COLS/2 - 40 ,"              " );
        }
        if(state == false){
            for (int i = top; i < LINES/2; i++) {
                move(i, COLS/2 - 40);
                for (int j = COLS/2 - 40 ; j < COLS; j++) {
                addch(' '); 
                }
            }
            continue;
        } else {
            add_username(username);
            create_folder(username);
            add_info(username,password,email);
            break;
        }
    }
    clear();
    refresh();
    endwin();

}


//incomplete function.
void open_items_menu(int menu_number){
    switch(menu_number){
        case 0: return;
        case 1: sign_up_user(); return;
        case 2: return;
        case 3: return;
        case 4: return;
    }
}

#endif