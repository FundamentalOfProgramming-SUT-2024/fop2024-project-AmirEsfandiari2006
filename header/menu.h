#ifndef FUNCTION_H
#define FUNCTION_H

#include "include.h"
#include "global.h"
#include "datatype.h"

void use_food(int,Player*);
void sort_leaderboard(PlayerScore leaderboard[], int count);
int get_user_data(const char *username, PlayerScore *playerscore);
void clear_message();
void set_up_colors();
void use_spell(int spell_index,Player *player);
void use_weapon(int weapon_index,Player *player);
void save_game(Level * level, Player *player);
void print_title();

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

    player_new_game = false;
    player_load_game = false;

    const char* menu_item[] = {"New Game","Continue"};
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
            if(hover == 0){
                player_new_game = true;
                player_load_game = false;
            } else if( hover == 1){
                player_new_game = false;
                player_load_game = true;
                if(is_savefile_exits()== false){
                    if(is_there_savegame == false && have_game_played == false && is_game_stop == false || is_game_ended == true){
                    continue;
                    }
                }

            }
            clear();
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
        
        print_title();
        for(int i = 0; i < number_menu_item; i++){
        if(i == hover)
            attron(A_REVERSE);
        mvprintw(LINES/2+i - number_menu_item,COLS/2 -number_menu_item,"%s\n",menu_item[i]);
        if(i == hover)
            attroff(A_REVERSE);
        }
        draw_borders();
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
                if(is_savefile_exits(username)){
                    is_there_savegame = true;
                }
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
    const char* line3[] = {"On","Off"};
    const char* line4[] = {"Track 1", "Track 2","Track 3"};

    const int number_lien1_item = 3;
    const int number_lien2_item = 3;
    const int number_lien3_item = 2;
    const int number_lien4_item = 3;
    
    initscr();
    noecho();
    keypad(stdscr, true); 
    curs_set(0);
    int which_line = 1;
    int hover = 0;
    while(true){
        
        draw_borders();
        mvprintw(LINES/2 - 4,COLS/2 - 30,"Game Difficulty:");
        mvprintw(LINES/2 - 2,COLS/2 - 30,"Player Color: ");
        mvprintw(LINES/2    ,COLS/2 - 30,"Playing Music: ");
        mvprintw(LINES/2 + 2,COLS/2 - 30,"Which Track: ");
        for(int i = 0; i < number_lien1_item;i++){
            mvprintw(LINES/2 - 4,COLS/2 -number_lien1_item + 10 * i ,"%s\n",line1[i]);
        }
        for(int i = 0; i < number_lien2_item;i++){
            mvprintw(LINES/2 - 2,COLS/2 -number_lien2_item + 10 * i ,"%s\n",line2[i]);
        }
        for(int i = 0; i < number_lien3_item;i++){
            mvprintw(LINES/2    ,COLS/2 -number_lien3_item + 10 * i ,"%s\n",line3[i]);
        }
        for(int i = 0; i < number_lien4_item;i++){
            mvprintw(LINES/2 + 2,COLS/2 -number_lien4_item + 10 * i ,"%s\n",line4[i]);
        }
        if(which_line == 1){
        for(int i = 0; i < number_lien1_item; i++){
            if(i == hover)
                attron(A_REVERSE);
            mvprintw(LINES/2 - 4,COLS/2 -number_lien1_item + 10 * i ,"%s\n",line1[i]);
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
                mvprintw(LINES/2 - 2 ,COLS/2 -number_lien2_item + 10 * i ,"%s\n",line2[i]);
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
                hover = 0;
                which_line = 3;
            } else if(command == ESCAPE){
                which_line = 1;
                hover = 0;

            }
        }
        if(which_line == 3){
            for(int i = 0; i < number_lien3_item; i++){
                if(i == hover)
                    attron(A_REVERSE);
                mvprintw(LINES/2 ,COLS/2 -number_lien3_item + 10 * i ,"%s\n",line3[i]);
                if(i == hover)
                    attroff(A_REVERSE);
            }
            int command = getch();
            if (command == KEY_LEFT)
                hover = (hover == 0) ? number_lien3_item - 1: hover - 1;
            else if (command == KEY_RIGHT)
                hover = (hover == number_lien3_item - 1) ? 0 : hover + 1;
            else if (command == ENTER){
                switch (hover)
                {
                case 0: music_on = 1; break;
                case 1: music_on = 0; break;
                }
                if(music_on){
                    which_line = 4;
                } else {
                    clear();
                    break;
                }
            } else if(command == ESCAPE){
                which_line = 2;
                hover = 0;
            }
        }   
        if(which_line == 4){
            for(int i = 0; i < number_lien4_item; i++){
                if(i == hover)
                    attron(A_REVERSE);
                mvprintw(LINES/2 + 2 ,COLS/2 -number_lien4_item + 10 * i ,"%s\n",line4[i]);
                if(i == hover)
                    attroff(A_REVERSE);
            }
            int command = getch();
            if (command == KEY_LEFT)
                hover = (hover == 0) ? number_lien4_item - 1: hover - 1;
            else if (command == KEY_RIGHT)
                hover = (hover == number_lien4_item - 1) ? 0 : hover + 1;
            else if (command == ENTER){
                switch (hover)
                {
                case 0: selected_music = 0; break;
                case 1: selected_music = 1; break;
                case 2: selected_music = 2; break;
                }
                clear();
                break;
            } else if(command == ESCAPE){
                which_line = 3;

            }   

        
        }
        clear();
    }
    
    refresh();
    endwin();
}


void show_leaderboard(){
    char usernames[MAX_PLAYER][MAX_LEN];
    PlayerScore leaderboard[MAX_PLAYER];
    int user_count = read_usernames("usernames.txt", usernames);
    int valid_users = 0;

    for (int i = 0; i < user_count; i++) {
        if (get_user_data(usernames[i], &leaderboard[valid_users])) {
            valid_users++;
        }
    }
    sort_leaderboard(leaderboard, valid_users);

    start_color();
    while(true){
        clear();
        draw_borders();
        set_up_colors();
        
        attron(COLOR_PAIR(3));
        mvprintw(1,cols/2 -5, "|            |");
        mvprintw(2,cols/2 -5, "|------------|");
        mvprintw(3,cols/2 -5, "|LEADER BOARD|");
        mvprintw(4,cols/2 -5, "+------------+");
        attroff(COLOR_PAIR(3));


        attron(COLOR_PAIR(5));
        mvprintw(6,cols/2 - 43," %-s | %-25s       | %-8s  | %-8s  | %-15s | %-15s","Rank","Player\'s Name","Score","Gold","Spending Time","New Games");
        attroff(COLOR_PAIR(5));



        for(int index = 0; index < user_count; index++){
            if(!strcmp(leaderboard[index].username,player_username)){
                attron(A_BOLD);
            }
            if(index == 0){
                    attron(COLOR_PAIR(7));
                    char temp_name[MAX_LEN];
                    strcpy(temp_name,leaderboard[index].username);
                    strcat(temp_name,", The Legend!");
                    mvprintw(8 + index * 2,cols/2 - 43," %-6ls | %-25s       | %-8d  | %-8d  | %-15d | %-15d",L"🏆",temp_name,leaderboard[index].score,leaderboard[index].total_gold,leaderboard[index].total_time,leaderboard[index].num_plays);
                    attroff(COLOR_PAIR(7));

            } else if(index == 1){
                    attron(COLOR_PAIR(8));
                    char temp_name[MAX_LEN];
                    strcpy(temp_name,leaderboard[index].username);
                    strcat(temp_name,", The Elite!");
                    mvprintw(8 + index * 2,cols/2 - 43," %-6ls | %-25s       | %-8d  | %-8d  | %-15d | %-15d",L"🥈",temp_name,leaderboard[index].score,leaderboard[index].total_gold,leaderboard[index].total_time,leaderboard[index].num_plays);
                    attroff(COLOR_PAIR(8));
            } else if(index == 2){
                    attron(COLOR_PAIR(9));
                    char temp_name[MAX_LEN];
                    strcpy(temp_name,leaderboard[index].username);
                    strcat(temp_name,", The Prodigy!");
                    mvprintw(8 + index * 2,cols/2 - 43," %-6ls | %-25s       | %-8d  | %-8d  | %-15d | %-15d",L"🥉",temp_name,leaderboard[index].score,leaderboard[index].total_gold,leaderboard[index].total_time,leaderboard[index].num_plays);
                    attroff(COLOR_PAIR(9));
            } else {
                mvprintw(8 + index * 2,cols/2 - 43," %-4d | %-25s       | %-8d  | %-8d  | %-15d | %-15d",index+1,leaderboard[index].username,leaderboard[index].score,leaderboard[index].total_gold,leaderboard[index].total_time,leaderboard[index].num_plays);    
            }
            if(!strcmp(leaderboard[index].username,player_username)){
                attroff(A_BOLD);
            }
            
        }
        if(getch() == ESCAPE){
            clear();
            return;
        }   
    }

}



void open_items_menu(int menu_number,Level* level,Player* player){
    switch(menu_number){
        case LOGIN         : log_in_user() ;      return;
        case SIGNUP        : sign_up_user();      return;
        case LEADERBOSRD   : show_leaderboard();  return;
        case SETTING       : setting();           return;
        case EXIT          :
        if(have_game_played == true && is_game_ended == false){
                save_game(level,player);  
        } else {
            exit(0);
        }
        return;
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
                use_weapon(command - 1 - '0',player);
            } else if(hover == 1){
                use_spell(command - 1 - '0',player);
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