#ifndef POKEMON_H
#define POKEMON_H

#include "db_parse.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

typedef struct simple_move {
    int level;
    int move_id;
} simple_move_t;

typedef enum stat_index {
    hp,
    attack,
    defense,
    speed,
    special_attk,
    special_def
} stat_ind_t;

typedef enum gender {
    male,
    female
} gender_t;

class Pokemon {
public:
    std::string name; 
    int poke_id;
    int poke_species_id;
    int level;
    int stats[6];
    int iv[6];
    bool shiny;
    move_db moves[2];
    std::vector<simple_move_t> total_moves;
    gender_t poke_gender;
    Pokemon(){};
    Pokemon(pokemon_db new_poke);
    ~Pokemon(){};
};


int set_pokemon_level();
void gen_pokemon();





#endif  /* POKEMON_H */