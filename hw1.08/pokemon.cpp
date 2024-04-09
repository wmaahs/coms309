#include "pokemon.h"
#include "poke327.h"


int manhattan_distance(pair_t xy1, pair_t xy2)
{
    return (abs(xy1[dim_x] - xy2[dim_x]) + abs(xy1[dim_y] - xy2[dim_y]));
}


void Pokemon::get_pokemon_level()
{
    int man_dist;
    pair_t origin;
    origin[dim_x] = 0;
    origin[dim_y] = 0;

    int min_level;
    int max_level;
    //find the manhattan distance from the origin
    man_dist = manhattan_distance(world.cur_idx, origin);

    if(man_dist <= 200) {
        min_level = 1;
        max_level = man_dist / 2;
    }
    else
    {
        min_level = (man_dist - 200) / 2;
        max_level = 100;
    }

    level = rand() % max_level + min_level;
}
void Pokemon::get_pokemon_moveset()
{
    /* @todo: figure out level first, make a class for the pokemon that we are gonna battle*/
    int i;
    int move_count = 0;
    pokemon_move_db init_move_set[20];
    move_db level_move_set[20];

    //get initial list of moves (first 20 matches)
    for(i = 0; i < NUM_POKE_MOVES && move_count < 20; i++) {
        if((poke_id == pokemon_moves[i].pokemon_id) && (pokemon_moves[i].pokemon_move_method_id == 1)) {
            init_move_set[move_count] = pokemon_moves[i];
            move_count++;
        }
    }

    // check that pokemon is high enough lvl
    for(i = 0; i < move_count; i++) {
        if(init_move_set[i].level > level){
            level_move_set[i].id = init_move_set[i].move_id;
        }
    }
    // get two moves from standard distribution of those

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

    Pokemon new_poke;
    int new_poke_id;
    terrain_type_t cur_terrain;
    cur_terrain = world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]];

    //if in tall grass, 10% chance that pokemon spawns
    if(cur_terrain == ter_grass) {
        if(rand() % 10 == 1) {
            //if one spawns, then grab random number 0-1091
            new_poke_id = rand() % 1092;
            new_poke.name = pokemon[new_poke_id].identifier;
            new_poke.poke_id = pokemon[new_poke_id].species_id;
            new_poke.get_pokemon_level();
            new_poke.get_pokemon_moveset();

            return;
        }
    }

    return;
}