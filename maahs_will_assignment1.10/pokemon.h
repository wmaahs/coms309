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
    int type_id;
    int base_xp;
    int curr_xp;
    int max_xp;
    move_db learned_moves[2];
    std::vector<simple_move_t> total_moves;
    gender_t poke_gender;

public:
    Pokemon(){};
    Pokemon(pokemon_db new_poke);
    Pokemon(int poke_id, int level, int move_ids[2]);

    /* methods */
    void levelup();
    int add_xp(int xp_earned);

    /* getters */
    int get_id(){return poke_id;}
    int get_base_xp(){return base_xp;}
    int get_curr_xp(){return curr_xp;}
    int get_max_xp(){return max_xp;}
    int get_species_id(){return poke_species_id;}
    int get_level(){return level;}
    int get_hp(){return stats[hp];}
    int get_curr_hp(){return curr_hp;}
    int get_attack(){return stats[attack];}
    int get_defense(){return stats[defense];}
    int get_speed(){return stats[speed];}
    int get_special_attk(){return stats[special_attk];}
    int get_special_def(){return stats[special_def];}
    int get_type(){return type_id;}
    std::string get_name(){return name;}
    move_db get_move(int ind){return learned_moves[ind];}
    std::vector<simple_move_t> get_all_moves(){return total_moves;}
    gender_t get_gender(){return poke_gender;}
    bool is_shiny(){return shiny;}


    void set_curr_hp(int new_hp){
        if(new_hp >= 0){
            if(new_hp <= stats[hp]){
                curr_hp = new_hp;
            }
            else{
                curr_hp = stats[hp];
            }
        }
        else
        {
            curr_hp = 0;
        }
    }
};


void gen_pokemon();
int set_pokemon_level();


#endif  /* POKEMON_H */
