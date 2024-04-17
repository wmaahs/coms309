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
private:
    std::string name; 
    int poke_id;
    int poke_species_id;
    int level;
    int base_stats[6];
    int stats[6];
    int iv[6];
    bool shiny;
    int curr_hp;
    move_db learned_moves[2];
    std::vector<simple_move_t> total_moves;
    gender_t poke_gender;

public:
    Pokemon(){};
    Pokemon(pokemon_db new_poke);

    void levelup();
    int get_id(){return poke_id;}
    int get_species_id(){return poke_species_id;}
    int get_level(){return level;}
    int get_hp(){return stats[hp];}
    int get_curr_hp(){return curr_hp;}
    int get_attack(){return stats[attack];}
    int get_defense(){return stats[defense];}
    int get_speed(){return stats[speed];}
    int get_special_attk(){return stats[special_attk];}
    int get_special_def(){return stats[special_def];}
    std::string get_name(){return name;}
    move_db get_move(int ind){return moves[ind];}
    std::vector<simple_move_t> get_all_moves(){return total_moves;}
    gender_t get_gender(){return poke_gender;}
    bool is_shiny(){return shiny;}
};


int set_pokemon_level();
void gen_pokemon();


#endif  /* POKEMON_H */
