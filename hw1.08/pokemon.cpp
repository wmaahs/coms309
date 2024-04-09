#include "pokemon.h"
#include "poke327.h"


void get_pokemon_moveset(pokemon_db new_poke)
{
    /* @todo: figure out level first, make a class for the pokemon that we are gonna battle*/
    int i;
    int move_count = 0;
    pokemon_move_db init_move_set[20];

    //get initial list of moves (first 20 matches)
    for(i = 0; i < NUM_POKE_MOVES && move_count < 20; i++) {
        if((new_poke.species_id == pokemon_moves[i].pokemon_id) && (pokemon_moves[i].pokemon_move_method_id == 1)) {
            init_move_set[move_count] = pokemon_moves[i];
            move_count++;
        }
    }

    //check that pokemon is high enough lvl
    // for(i = 0; i < move_count; i++) {
    //     if(init_move_set[i].level < new_poke)
    // }
    //get two moves from standard distribution of those

}

/**
 * Function to determin if a pokemon is spawned,
 * called from io.cpp in function io_handle_input
 * after every return from move_pc_dir.
 * 
 * If a pokemon is spawned, then calls get_pokemon_moveset() <-- need to figure out the lvl first
 * 
*/
void spawn_pokemon()
{

    int new_poke_id;
    pokemon_db new_poke;
    terrain_type_t cur_terrain;
    cur_terrain = world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]];

    //if in tall grass, 10% chance that pokemon spawns
    if(cur_terrain == ter_grass) {
        if(rand() % 10 == 0) {
            //if one spawns, then grab random number 0-1091
            new_poke_id = rand() % 1092;
            new_poke = pokemon[new_poke_id];
            get_pokemon_moveset(new_poke);
            return;
        }
    }

    return;
}