#include "pokemon.h"
#include "poke327.h"
#include "io.h"

int manhattan_distance(pair_t xy1, pair_t xy2)
{
    return (abs(xy1[dim_x] - xy2[dim_x]) + abs(xy1[dim_y] - xy2[dim_y]));
}

int set_pokemon_level()
{
    int man_dist;
    pair_t origin;
    origin[dim_x] = 200;
    origin[dim_y] = 200;

    int level;
    int min_level;
    int max_level;
    //find the manhattan distance from the origin
    man_dist = manhattan_distance(world.cur_idx, origin);

    if(man_dist <= 200) {
      if(man_dist == 0){
	return 1;
      }
      min_level = 1;
      max_level = man_dist / 2;
    }
    else
    {
        min_level = (man_dist - 200) / 2;
        max_level = 100;
    }

    level = rand() % (max_level - min_level + 1) + min_level;
    
    return level;
}

Pokemon::Pokemon(pokemon_db new_poke)
{
    name = new_poke.identifier;
    poke_species_id = new_poke.species_id;
    poke_id = new_poke.id;

    level = set_pokemon_level();

    //Find total move set
    int i;
    for(i = 0; i < NUM_POKE_MOVES; i++) {
        if((poke_species_id == pokemon_moves[i].pokemon_id) && (pokemon_moves[i].pokemon_move_method_id == 1)) {
            total_moves.push_back({pokemon_moves[i].level, pokemon_moves[i].move_id});
        }
    }
    //randomly select first move
    int random_move;
    do {
        random_move = rand() % total_moves.size();
        moves[0].id = total_moves[random_move].move_id;
    } while(!(total_moves[random_move].level <= level));

    //randomly select second move that isn't first move
    do {
        random_move = rand() % total_moves.size();
        moves[1].id = total_moves[rand() % total_moves.size()].move_id;
    } while((!(total_moves[random_move].level <= level)) && (moves[0].id != moves[1].id));

    //get basic stats
    for(i = 0; i < NUM_POKE_STATS; i++) {
        if(poke_species_id == pokemon_stats[i].pokemon_id) {
            stats[pokemon_stats[i].stat_id -1] = pokemon_stats[i].base_stat;
        }
    }

    //assign curr hp
    curr_hp = stats[hp];

    //IV's
    for(i = 0; i < 6; i++) {
        iv[i] = rand() % 16;
    }

    //gender
    if(rand() % 2 == 0) {
        poke_gender = female;
    }
    else{
        poke_gender = male;
    }

    //is shiny
    if(rand() % 8192 == 0) {
        shiny = true;
    }
    else{
        shiny = false;
    }
}

/**
 * Function used to level up the stats of a pokemon, given its current level
*/
void Pokemon::levelup(){
    stats[hp] = ((((stats[hp] + iv[hp]) * 2) * level)/100) + level + 10;
    int i;
    for(i = 1; i < 6; i++){
        stats[i] = ((((stats[i] + iv[i]) * 2) * level)/100) + 5;
    }
}

/**
 * Function to determin if a pokemon is spawned,
 * called from io.cpp in function io_handle_input
 * after every return from move_pc_dir.
 * 
 * If a pokemon is spawned, then calls constructor
 * 
*/
void gen_pokemon()
{
    int new_poke_id;
    terrain_type_t cur_terrain;
    cur_terrain = world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]];

    //if in tall grass, 10% chance that pokemon spawns
    if(cur_terrain == ter_grass) {
        if(rand() % 10 == 1) {
            //if one spawns, then grab random number 0-1091
            new_poke_id = rand() % 1092;

            Pokemon new_poke(pokemon[new_poke_id]);
            new_poke.levelup();
            io_battle_wild_pokemon(&new_poke);
            return;
        }
    }

    return;
}
