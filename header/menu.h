#ifndef FUNCTION_H
#define FUNCTION_H

#include "include.h"
#include "global.h"
#include "datatype.h"

void use_food(int,Player*);
void clear_message();

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
            is_game_playing = true;
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
                strcpy(player_username,username);
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

void setting(){
    const char* line1[] = {"Easy","Normal","Hard",};
    const char* line2[] = {"Green","Blue","Cyan"};

    const int number_lien1_item = 3;
    const int number_lien2_item = 3;
    
    initscr();
    noecho();
    keypad(stdscr, true); 
    curs_set(0);
    int which_line = 1;
    int hover = 0;
    while(true){
        
        draw_borders();
        mvprintw(LINES/2,COLS/2 - 30,"Game Difficulty:");
        mvprintw(LINES/2 +2,COLS/2 - 30,"Player Color: ");
        for(int i = 0; i < number_lien2_item;i++){
            mvprintw(LINES/2,COLS/2 -number_lien1_item + 10 * i ,"%s\n",line1[i]);
        }
        for(int i = 0; i < number_lien1_item;i++){
            mvprintw(LINES/2 +2,COLS/2 -number_lien2_item + 10 * i ,"%s\n",line2[i]);
        }
        if(which_line == 1){
        for(int i = 0; i < number_lien1_item; i++){
            if(i == hover)
                attron(A_REVERSE);
            mvprintw(LINES/2,COLS/2 -number_lien1_item + 10 * i ,"%s\n",line1[i]);
            if(i == hover)
                attroff(A_REVERSE);
            }
            int command = getch();
            if (command == KEY_LEFT)
                hover = (hover == 0) ? number_lien1_item - 1: hover - 1;
            else if (command == KEY_RIGHT)
                hover = (hover == number_lien1_item - 1) ? 0 : hover + 1;
            else if (command == ENTER){
                which_line = 2;
                game_diff = hover - 1;
                hover = 0;
            } else if(command == ESCAPE){
                clear();
                break;;
            }
        } 
        if(which_line == 2){
            for(int i = 0; i < number_lien2_item; i++){
                if(i == hover)
                    attron(A_REVERSE);
                mvprintw(LINES/2 +2,COLS/2 -number_lien2_item + 10 * i ,"%s\n",line2[i]);
                if(i == hover)
                    attroff(A_REVERSE);
            }
            int command = getch();
            if (command == KEY_LEFT)
                hover = (hover == 0) ? number_lien2_item - 1: hover - 1;
            else if (command == KEY_RIGHT)
                hover = (hover == number_lien2_item - 1) ? 0 : hover + 1;
            else if (command == ENTER){
                switch (hover)
                {
                case 0: player_color = COLOR_GREEN; break;
                case 1: player_color = COLOR_BLUE; break;
                case 2: player_color = COLOR_CYAN; break;
                }
                clear();
                break;
            } else if(command == ESCAPE){
                which_line = 1;

            }    

        clear();
        }
    }
    
    refresh();
    endwin();
}

//incomplete function.
void open_items_menu(int menu_number){
    switch(menu_number){
        case LOGIN         : log_in_user() ;      return;
        case SIGNUP        : sign_up_user();      return;
        case LEADERBOSRD   : /* leader_board()*/  return;
        case SETTING       : setting();           return;
        case EXIT          :                      return;
    }
}


void open_and_handle_inventory(Player* player){


    const char* main_line_name[] = {"Weapons","Spells","Foods","Other Items"};
    const char* weapons_name[] = {"Mace","Dagger","Magic Wand","Normal Arrow","Sword"};
    const char* spells_name[] = {"Damage","Health","Speed"};
    const char* foods_name[] = {"Normal Food","Pure Food","Magic Food","Rotten Food"};

    const int number_of_main_line_name = 3;

    initscr();
    noecho();
    keypad(stdscr, true); 
    curs_set(0);
    int which_line = 1;
    int hover = 0;

    bool first_time = 1;
    int command;
    while(true){
        if(first_time){
            clear();
            first_time = 0;
        }
        mvprintw(1,cols/2 - 10,"Player Inventory");

        for(int i = 4; i < lines; i++){
            mvprintw(i, 1 * cols / 4 - 2,"|");
            mvprintw(i, 2 * cols / 4 - 2, "|");
            mvprintw(i, 3 * cols / 4 - 2, "|");
        }
        for(int i = 0; i < number_of_main_line_name; i++){
            if(i == hover){attron(A_REVERSE);}
            mvprintw(4 , i * (cols / 4) + 20, "%s\n",main_line_name[i]);
            if(i == hover){attroff(A_REVERSE);}
        }

        for(int i = 0; i < MAX_WEAPON_N;i++){
            mvprintw(8 + 2 * i, 2,"%d.Number of %s: %d",i+1,weapons_name[i],player->number_of_each_weapon[i]);
        }
        for(int i = 0; i < MAX_SPELL_N;i++){
            mvprintw(8 + 2 * i, 48,"%d.Number of %s Spell: %d",i+1,spells_name[i],player->number_of_each_spell[i]);
        }
        for(int i = 0; i < MAX_FOOD_N;i++){
            mvprintw(8 + 2 * i, 90,"%d.Number of %s: %d",i+1,foods_name[i],player->number_of_each_food[i]);
        }

        command = getch();
        clear_message();

        if (command == KEY_LEFT)
            hover = (hover == 0) ? number_of_main_line_name - 1: hover - 1;
        else if (command == KEY_RIGHT)
            hover = (hover == number_of_main_line_name - 1) ? 0 : hover + 1;
        else if (command == ENTER){
            mvprintw(1,1,"Which Item do want to use? (Enter the number)");
            command = getch();
            if(hover == 0){
                //use_weapon(command - 1 - '0',player);
            } else if(hover == 1){
                //use_spell(command - 1 - '0',player);
            } else if(hover == 2){
                use_food(command - 1 - '0',player);
            }

        } else if(command == ESCAPE){
            clear();
            break;;
        } else if(command == 'i'){
            clear();
            return;
        } 
    }
    refresh();
    endwin();
}

#endif