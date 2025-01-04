#include "header/include.h"

int main() {
    srand(time(0));

    //Game set_up
    Level level1;
    generate_random_room(&level1);

    //Player set_up
    Player player;
    init_player(&player, &level1);

    int command = 0;   
    while (command != EXIT) {
        if (!is_game_playing) {

            while (command != EXIT && !is_game_playing) {
                command = get_command_main_menu();
                open_items_menu(command);

            }
        }

        if (is_game_playing && is_logged_in) {

            while (command != ESCAPE) {
                printf_level(&level1);
                print_player(&player);

                command = getch();
                move_player(command, &player);
                clear();
            }
            is_game_playing = false;
        }
    }

    refresh();
    endwin();

    return 0;
}
