#ifndef POKEMON_H
#define POKEMON_H

#include "db_parse.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

class Pokemon {
public:
    std::string name; 
    int poke_id;
    int level;
    int hp;
    int max_hp;
    move_db moves[2];
    Pokemon(){};
    Pokemon(pokemon_db *new_poke)
    {
        name = new_poke->identifier;
        poke_id = new_poke->species_id;
        level = 0;
        hp = 0;
        max_hp = 0;
    }
    void get_pokemon_level();
    void get_pokemon_moveset();
};


void spawn_pokemon();





#endif  /* POKEMON_H */