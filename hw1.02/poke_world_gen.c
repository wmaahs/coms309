#include "poke_world_gen.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct world {
    map_t *world[WORLD_HEIGHT][WORLD_WIDTH];
    point_t coordinates;
} world_t;

int main(int argc, char **argv){

    world_t the_pokeverse;

    map_t first_map = pokemon_map_gen(0, 0);
    first_map.loc.x = 200;
    first_map.loc.y = 200;

    // Initially, all maps in the world are NULL;
    int i, j;
    for(i = 0; i < WORLD_HEIGHT; i++) {
        for(j = 0; j < WORLD_WIDTH; j++){

            the_pokeverse.world[j][i] = NULL;
        }
    }

    //malloc the first map
    the_pokeverse.world[first_map.loc.x][first_map.loc.y] = (void *)(malloc(sizeof(first_map)));
    draw_curr_map(first_map);

    //free the map after execution... just for now
    free(the_pokeverse.world[first_map.loc.x][first_map.loc.y]);
}

void draw_curr_map(map_t current_map) {

    generate_the_map(current_map.map);

}

