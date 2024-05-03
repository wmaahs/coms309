#ifndef BATTLE_H
#define BATTLE_H

#include "pokemon.h"

void battle_fight(Pokemon *enemy, Pokemon *pc_pokemon);
void enemy_attack(Pokemon *enemy, int enemy_move, Pokemon *pc_pokemon);
void pc_attack(Pokemon *enemy, int selected_move, Pokemon *pc_pokemon);
Pokemon select_pokemon();

#endif
