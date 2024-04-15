
#include "bag.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>


Bag::Bag()
{
    int i;
    pokeball_t curr_pokeball;
    revive_t curr_revive;
    potion_t curr_potion;

    //starter balls
    for(i = 0; i < DEFAULT_POKEBALLS; i++) {
        curr_pokeball.type = common;
        curr_pokeball.boost = 1;
        pokeballs.push_back(curr_pokeball);
    }
    //starter revives
    for(i = 0; i < DEFAULT_REVIVES; i++){
        curr_revive.type = common;
        curr_revive.heal = 10;
        revives.push_back(curr_revive);
    }
    //starter potions
    for(i = 0; i < DEFAULT_POTIONS; i++){
        curr_potion.type = common;
        curr_potion.heal = 10;
        potions.push_back(curr_potion);
    }
}