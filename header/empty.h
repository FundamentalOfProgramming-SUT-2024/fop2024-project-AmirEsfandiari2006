            case MAGIC_WAND_INDEX:
            if(player->number_of_each_weapon[MAGIC_WAND_INDEX] > 0){
                switch(which_way){
                case KEY_LEFT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places += 21;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.y--;
                }
                break;
                case KEY_RIGHT:
                while(true){
                    char tile = mvinch(start_point.x,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places += 21;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.y++;
                }
                break;     
                case KEY_UP:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places += 21;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.x--;
                }
                break;
                case KEY_DOWN:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places += 21;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.x++;
                }
                break;
                case KEY_HOME:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places += 21;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    start_point.x--; start_point.y--;
                }
                break;
                case KEY_PPAGE:
                while(true){
                    char tile = mvinch(start_point.x - 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places += 21;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x - 1,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                     start_point.x--; start_point.y++;
                }
                break;
                case KEY_NPAGE:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y + 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places += 21;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y + 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                     start_point.x++; start_point.y++;
                }
                break;
                case KEY_END:
                while(true){
                    char tile = mvinch(start_point.x + 1,start_point.y - 1);
                    if(tile == '_' || tile == '|' || tile == '+' ||tile == 'O'){
                        level[level_map].rooms[player->room].total_places += 21;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].display = 'W';
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].position = start_point;
                        level[level_map].rooms[player->room].places[level->rooms[player->room].total_places].is_it_thrown = true;
                        level[level_map].rooms[player->room].total_places++;
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                    if(tile == 'B' || tile == 'N' || tile == 'U' || tile == 'I' || tile == 'E'){
                        Point point = {start_point.x + 1,start_point.y - 1};
                        Monster* monster = get_monster_by_point(point,level,player);
                        printf("%d",monster->health);
                        attack_monster_away(monster,player);
                        player->number_of_each_weapon[MAGIC_WAND_INDEX]--;
                        break;
                    }
                     start_point.x++; start_point.y--;
                }
                break;
            }
            } 
            else {
                clear_message();
                throw_dart = true;
                mvprintw(1,1,"You don't have magic wand to throw!");
            }
            break; 