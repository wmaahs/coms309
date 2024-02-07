#include "poke_world_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Main Function for poke_world_gen.c
 * 
 * @param argc
 * @param argv
*/
int main(int argc, char **argv){

    world_t the_pokeverse;

    map_t first_map = pokemon_map_gen();    //should remove paramters for pokemon_map_gen
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
    the_pokeverse.coordinates.x = 200;
    the_pokeverse.coordinates.y = 200;
    *the_pokeverse.world[first_map.loc.x][first_map.loc.y] = first_map;
    nav_pokeverse(the_pokeverse);
    //free the map after execution... just for now
    free(the_pokeverse.world[first_map.loc.x][first_map.loc.y]);

    /** UNCOMMENT WHEN DONE */
    // //exiting program, free the world
    // for(i = 0; i < WORLD_HEIGHT; i++) {
    //     for(j = 0; j < WORLD_WIDTH; j++){

    //         //check if it has been malloced
    //         if(the_pokeverse.world[j][i] != NULL) {
    //             free(the_pokeverse.world[j][i]);
    //         }
    //     }
    // }
}

void nav_pokeverse(world_t pokeverse) {

    map_t *current_map = pokeverse.world[pokeverse.coordinates.x][pokeverse.coordinates.y];
    draw_curr_map(*current_map);

    //display location
    printf("\n");
    printf("L[%d][%d]   \n", abs(pokeverse.coordinates.x) - 200, abs(pokeverse.coordinates.y) -200);

    char *input_buff;
    input_buff = (char *) (malloc(sizeof(char) * 25));  //buffer for user input

    //wait for user input
    while(1) {
        scanf("%s", input_buff);
        break;
    }

    //if user inputs 'n'
    if(strcmp(input_buff, "n") == 0) {

        //if the map is already there
        if(check_north(pokeverse) >= 0) {
            pokeverse.coordinates.y++;
            nav_pokeverse(pokeverse);
        }
        //else generate new map with s gate in same spot as n gate
        else{
                //malloc the new map
                pokeverse.world[pokeverse.coordinates.x][pokeverse.coordinates.y +1] = (void *)(malloc(sizeof(map_t)));
                pokeverse.coordinates.y++;
                
                //check adj of new map
                //if
        }

    }

    //if user inputs 's'
    if(strcmp(input_buff, "s") == 0) {

        printf("User typed 's'\n");
    }

    //if user inputs 'w'
    if(strcmp(input_buff, "w") == 0) {

        printf("User typed 'w'\n");
    }

    //if user inputs 's'
    if(strcmp(input_buff, "e") == 0) {

        printf("User typed 'e'\n");
    }

    //if user wants to quit
    if(strcmp(input_buff, "q") == 0) {

        return;

    }

    free(input_buff);

}

/**
 * 
 * @param current_map the map that player is currently on
*/
void draw_curr_map(map_t current_map) {

    generate_the_map(current_map);

}

/**
 * Function to check the map the north.
 * If there already exists a map to the north, returns location of southern gate.
 * If there is no map to the north, returns -1.
 * 
 * @param pokeverse the pokemon universe
 * @returns -1, on no map to the north
 * @returns northern_map.s, when there is already a map there.
*/
int check_north(world_t pokeverse) {

    map_t northern_map;
    northern_map = *pokeverse.world[pokeverse.coordinates.x][pokeverse.coordinates.y + 1];
    //if the map is already there
    if(&northern_map != NULL) {
        //return the location of the southern gate
        return northern_map.s;
    }
    // otherwise return -1
    else {
        return -1;
    }
}

// int *check_adj_maps(world_t pokeverse) {

//     map_t *current_map;
//     current_map = pokeverse.world[pokeverse.coordinates.x][pokeverse.coordinates.y];

//     int n, s, e, w;
//     if(pokeverse->world[pokeverse.coordinates.x][pokeverse.coordinates.y + 1]. == NULL) {
//         //north has not been made yet
//         n = -1;
//     }

// }

