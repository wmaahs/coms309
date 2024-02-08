# ifndef POKE_WORLD_GEN_H
# define POKE_WORLD_GEN_H

#include "pokemon_map_gen.h"

#define WORLD_HEIGHT 401
#define WORLD_WIDTH 403

typedef struct n_s_e_w {
    int nsew[4];
} nsew_t;

typedef struct world {
    map_t *world[WORLD_WIDTH][WORLD_WIDTH];
    point_t coordinates;
} world_t;


void nav_pokeverse(world_t *pokeverse);

void draw_curr_map(map_t current_map);

int check_north(world_t *pokeverse);
int check_south(world_t *pokeverse);
int check_east(world_t *pokeverse);
int check_west(world_t *pokeverse);
nsew_t get_adj_map_gates(world_t *pokeverse);


#endif