#ifndef BATTLE_H
#define BATTLE_H

#include "pokemon.h"

void battle_fight(Pokemon *enemy, Pokemon *pc_pokemon);
void enemy_free_attack(Pokemon *enemy, Pokemon *pc_pokemon);
Pokemon select_pokemon();

#endif
