
#include "battle.h"
#include <ncurses.h>


int battle_fight(Pokemon enemy, Pokemon pc_pokemon)
{
    int enemy_move;
    int key;

    //clear battle options
    move(15, 0);
    clrtoeol();
    move(17, 0);
    clrtoeol();
    move(19, 0);
    clrtoeol();
    move(21, 0);
    clrtoeol();

    mvprintw(17, 0, "1. %s", pc_pokemon.get_all_moves().at(0));
    mvprintw(19, 0, "2. %s", pc_pokemon.get_all_moves().at(1));
    refresh();

    key = getch();

    switch(key){
        case '1':
            mvprintw(21, 0, "You have selected move 1");
            break;
        case '2':
            mvprintw(21, 0, "You have selected move 2");
            break;
        default:
            mvprintw(21, 0, "Pick move one or two");
            break;
        
    }

    if(pc_pokemon.get_curr_hp() < 1){
        return 1;
    }
    if(enemy.get_curr_hp() < 1) {
        return 0;
    }

}

