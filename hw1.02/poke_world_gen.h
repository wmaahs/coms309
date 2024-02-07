# ifndef POKE_WORLD_GEN_H
# define POKE_WORLD_GEN_H

#include "pokemon_map_gen.h"

#define WORLD_HEIGHT 401
#define WORLD_WIDTH 401

typedef struct world {
    map_t *world[WORLD_HEIGHT][WORLD_WIDTH];
    point_t coordinates;
} world_t;


void nav_pokeverse(world_t pokeverse);

void draw_curr_map(map_t current_map);

int check_north(world_t pokeverse);
int *check_adj_maps(world_t pokeverse);


#endif