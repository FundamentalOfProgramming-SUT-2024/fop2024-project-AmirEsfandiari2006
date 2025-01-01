#ifndef FUNCTION_H
#define FUNCTION_H

#include "include.h"

void draw_borders(){
    for(int i = 0; i < COLS; i++){
        mvprintw(0,i,"#");
        mvprintw(LINES - 1,i,"#"); 
    }
    for(int i = 0; i < LINES; i++){
        mvprintw(i,0,"|");
        mvprintw(i,COLS - 1,"|"); 
    }
    if(is_logged_in){
        mvprintw(LINES - 2,1,"Your Logged in!");
    } else {
        mvprintw(LINES - 2,1,"Your not Logged in!");
    }
}

int play_the_game_menu(){
    const char* menu_item[] = {"New Game","Load Game"};
    const int number_menu_item = 2;
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
        if(command == 'y'){
            break;
        }
        if (command == KEY_UP)
            hover = (hover == 0) ? number_menu_item - 1: hover - 1;
        else if (command == KEY_DOWN)
            hover = (hover == number_menu_item - 1) ? 0 : hover + 1;
        else if (command == ENTER){
            clear();
            endwin();
            is_game_plying = true;
            return hover;
        }
            
        clear();
    }
    refresh();
    endwin();
}


int get_command_main_menu(){
    const char* menu_item[] = {"Play","Sign up","Leader Board","Setting","Exit"};
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
    const int start = COLS/2 - 40;
    int first_time = 0;
    while(true){
        int state = 1;
        mvprintw(top + 0,start , "%s","Enter your Username: ");
        mvprintw(top + 2,start , "%s","Enter your Password: ");
        mvprintw(top + 4,start , "%s","Enter your Email address: ");
        mvprintw(top + 9,start , "%s","*Your password must have at least 7 characters, consist of one upper,lower and number.");
        if(first_time){
            mvprintw(top - 2,start , "%s","Do you want to back to main menu? [y/n] ");
            char ch[MAX_INPUT_SIZE];
            mvscanw(top - 2,COLS/2 , "%s", ch);;
            if(!strcmp("y",ch)){ 
                break;
            } else {
                mvprintw(top - 2,start , "%s","                                           ");
            }
        }
        mvscanw(top + 0,COLS/2 , "%s", username);
        mvscanw(top + 2,COLS/2 , "%s", password);
        mvscanw(top + 4,COLS/2 , "%s", email);
        first_time = 1;

        if(!valid_username(username)){
            attron(COLOR_PAIR(1));
            mvprintw(top + 11,start ,"This username is already defined." );
            state = 0;
            attroff(COLOR_PAIR(1)); 
        } else {
            mvprintw(top + 11,start,"                                  ");
        }
        if(!valid_password(password)){
            attron(COLOR_PAIR(1));
            mvprintw(top + 13,start ,"Your Password doesn't meet the requirments." );
            state = 0;
            attroff(COLOR_PAIR(1));
        } else {
            mvprintw(top + 13,start ,"                                           " );
        }
        if(!valid_email(email)){
            attron(COLOR_PAIR(1));
            mvprintw(top + 15,start ,"Invalid Email." );
            state = 0;
            attroff(COLOR_PAIR(1));
        } else{
            mvprintw(top + 15,start ,"              " );
        }
        if(state == false){
            for (int i = top; i < LINES/2; i++) {
                move(i, start);
                for (int j = start ; j < COLS; j++) {
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

void log_in_user(){
    char username[MAX_INPUT_SIZE];
    char password[MAX_INPUT_SIZE];

    initscr();
    echo();
    curs_set(1);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    draw_borders();
    const int top = LINES/2 - 5;
    const int start = COLS/2 - 40;

    if(is_logged_in){
        play_the_game_menu();
    } else {
        int first_time = 0;
        while(true){
            int state = true;
            mvprintw(top + 0,start , "%s","Enter your Username: ");
            mvprintw(top + 2,start , "%s","Enter your Password: ");

            if(first_time){
                mvprintw(top - 2,start , "%s","Do you want to back to main menu? [y/n] ");
                char ch[MAX_INPUT_SIZE];
                mvscanw(top - 2,COLS/2 , "%s", ch);;
                if(!strcmp("y",ch)){ 
                    break;
                } else {
                    mvprintw(top - 2,start , "%s","                                           ");
                }
            }
            mvscanw(top + 0,COLS/2 , "%s", username);
            mvscanw(top + 2,COLS/2 , "%s", password);
            first_time = 1;
            if(valid_username(username)){
                state = false;
                attron(COLOR_PAIR(1));
                mvprintw(top + 11,start ,"Invalid username.");
                attroff(COLOR_PAIR(1));
            }else{
                mvprintw(top + 11,start ,"                 " );
                if(!check_password_for_username(username,password)){
                state = false;
                attron(COLOR_PAIR(1));
                mvprintw(top + 13,start ,"Password is incorrect.");
                attroff(COLOR_PAIR(1));
                } else {
                    mvprintw(top + 13,start ,"                      ");
                }
            }

            //make the input ready for next try or logged in and continue
            if(state == false){
                for (int i = top; i < LINES/2; i++) {
                    move(i, start);
                    for (int j = start ; j < COLS; j++) {
                    addch(' '); 
                    }
                }
                continue;
            } else {

                is_logged_in = true;
                //strcpy(player.username,username);
                //It should be continued by other values but for now. It would be enough.
                clear();
                break;
            }
        }
    }

    clear();
    
    refresh();
    endwin();
}

int random_number(int min, int max) {

    return rand() % (max - min + 1) + min;
}

//incomplete function.
void open_items_menu(int menu_number){
    switch(menu_number){
        case LOGIN         : log_in_user() ;      return;
        case SIGNUP        : sign_up_user();      return;
        case LEADERBOSRD   : /* leader_board()*/  return;
        case SETTING       : /* setting()     */  return;
        case EXIT          :                      return;
    }
}


#endif