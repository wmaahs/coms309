#include "poke_world_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Main Function for poke_world_gen.c.
 * This function initializes the universe by generating
 * the first map. The first map is located at the center
 * of the world (the middle of the array). The world is a 
 * 401x401 array of pointers to maps. The main function
 * then calls the nav_pokeverse function to navigate around
 * the world.
 * @param argc
 * @param argv
*/
int main(int argc, char **argv){

    //Hello Pokeverse
    world_t the_pokeverse;


    // Initially, all pointers to maps in the world are NULL;
    int i, j;
    for(i = 0; i < WORLD_HEIGHT; i++) {
        for(j = 0; j < WORLD_WIDTH; j++){

            the_pokeverse.world[j][i] = NULL;
        }
    }

    map_t first_map;
    first_map.n = - 1;
    first_map.s = - 1;
    first_map.e = - 1;
    first_map.w = - 1;
    first_map = pokemon_map_gen(&first_map);    //should remove paramters for pokemon_map_gen
    first_map.loc.x = 200;
    first_map.loc.y = 200;

    //malloc the first map
    the_pokeverse.world[first_map.loc.x][first_map.loc.y] = (map_t *)(malloc(sizeof(first_map)));
    the_pokeverse.coordinates.x = 200;
    the_pokeverse.coordinates.y = 200;
    *the_pokeverse.world[first_map.loc.x][first_map.loc.y] = first_map;
    nav_pokeverse(&the_pokeverse);
    //free the map after execution... just for now
    free(the_pokeverse.world[first_map.loc.x][first_map.loc.y]);

    /** UNCOMMENT WHEN DONE */
    //exiting program, free the world
    for(i = 0; i < WORLD_HEIGHT; i++) {
        for(j = 0; j < WORLD_WIDTH; j++){

            //check if it has been malloced
            if(the_pokeverse.world[j][i] != NULL) {
                free(the_pokeverse.world[j][i]);
            }
        }
    }
}

/**
 * 
 * 
*/
void nav_pokeverse(world_t *pokeverse) {

    map_t current_map = *pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y];

    draw_curr_map(current_map);

    //display location
    printf("\n");
    printf("L[%d][%d]       ", abs(pokeverse->coordinates.x) - 200, 200 - pokeverse->coordinates.y);

    char input_buff;

    int x_dest, y_dest;


    //struct to hold gate locations
    nsew_t n_s_e_w;

    //wait for user input
    while(1) {
        scanf(" %c", &input_buff);
        break;
    }

    while(input_buff != 'q') {
        switch(input_buff) {

            // GOING NORTH
            case 'n':

            //border check
                if(pokeverse->coordinates.y-1 < 0) {
                    printf("You have reach the edge of the world. You should probably turn around now\n");
                    nav_pokeverse(pokeverse);
                    return;
                }

                //if the map is already there, go to it.
                if(check_north(pokeverse) >= 0) {
                    pokeverse->coordinates.y--;
                    nav_pokeverse(pokeverse);
                }
                //else generate new map with s gate in same spot as current n gate
                else {

                    //malloc the new map
                    pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y -1] = (map_t *)(malloc(sizeof(map_t)));
                    //go to the new map
                    pokeverse->coordinates.y--;

                    //check adj of new map
                    n_s_e_w = get_adj_map_gates(pokeverse);
                    //generate new map with gates in those locations
                    map_t new_map;
                    new_map.n = n_s_e_w.nsew[0];
                    new_map.s = n_s_e_w.nsew[1];
                    new_map.e = n_s_e_w.nsew[2];
                    new_map.w = n_s_e_w.nsew[3];

                    //create the new map
                    new_map = pokemon_map_gen(&new_map);
                    *pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y] = new_map;
                    nav_pokeverse(pokeverse);
                }
                return;
            break;

            // GOING SOUTH
            case 's':
                //border check
                if(pokeverse->coordinates.y+1 >= WORLD_HEIGHT) {
                    printf("You have reach the edge of the world. You should probably turn around now\n");
                    nav_pokeverse(pokeverse);
                    return;
                }

                //if the map is already there, go to it.
                if(check_south(pokeverse) >= 0) {
                    pokeverse->coordinates.y++;
                    nav_pokeverse(pokeverse);
                }
                //else generate new map with n gate in same spot as current s gate
                else {

                    //malloc the new map
                    pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y +1] = (map_t *)(malloc(sizeof(map_t)));
                    //go to the new map
                    pokeverse->coordinates.y++;
                    //check adj of new map
                    n_s_e_w = get_adj_map_gates(pokeverse);
                    //generate new map with gates in those locations
                    map_t new_map;
                    printf("N: %d ", n_s_e_w.nsew[0]);
                    printf("S: %d ", n_s_e_w.nsew[1]);
                    printf("E: %d ", n_s_e_w.nsew[2]);
                    printf("W: %d\n", n_s_e_w.nsew[3]);
                    new_map.n = n_s_e_w.nsew[0];
                    new_map.s = n_s_e_w.nsew[1];
                    new_map.e = n_s_e_w.nsew[2];
                    new_map.w = n_s_e_w.nsew[3];


                    //create the new map
                    new_map = pokemon_map_gen(&new_map);
                    *pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y] = new_map;

                    nav_pokeverse(pokeverse);
                }
                
                return;
            break;

            // GOING EAST
            case 'e':
                //border check
                if(pokeverse->coordinates.x+1 >= WORLD_WIDTH) {
                    printf("You have reach the edge of the world. You should probably turn around now\n");
                    nav_pokeverse(pokeverse);
                    return;
                }

                //if the map is already there, go to it.
                if(check_east(pokeverse) >= 0) {
                    pokeverse->coordinates.x++;
                    nav_pokeverse(pokeverse);
                }
                //else generate new map with n gate in same spot as current s gate
                else {

                    //malloc the new map
                    pokeverse->world[pokeverse->coordinates.x +1][pokeverse->coordinates.y] = (map_t *)(malloc(sizeof(map_t)));
                    //go to the new map
                    pokeverse->coordinates.x++;
                    //check adj maps of new map
                    n_s_e_w = get_adj_map_gates(pokeverse);
                    //generate new map with gates in those locations
                    map_t new_map;
                    new_map.n = n_s_e_w.nsew[0];
                    new_map.s = n_s_e_w.nsew[1];
                    new_map.e = n_s_e_w.nsew[2];
                    new_map.w = n_s_e_w.nsew[3];

                    //create the new map
                    new_map = pokemon_map_gen(&new_map);
                    *pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y] = new_map;


                    nav_pokeverse(pokeverse);
                }
                return;
            break;

            // GOING WEST
            case 'w':
                //border check
                printf("testing");

                if(pokeverse->coordinates.x- 1 < 0) {
                    printf("You have reach the edge of the world. You should probably turn around now\n");

                    nav_pokeverse(pokeverse);
                    return;
                }
                //if the map is already there, go to it.
                if(check_west(pokeverse) >= 0) {
                    pokeverse->coordinates.x--;
                    nav_pokeverse(pokeverse);
                    return;
                }
                //else generate new map with n gate in same spot as current s gate
                else {

                    //malloc the new map
                    pokeverse->world[pokeverse->coordinates.x -1][pokeverse->coordinates.y] = (map_t *)(malloc(sizeof(map_t)));
                    //go to the new map
                    pokeverse->coordinates.x--;
                    //check adj of new map
                    n_s_e_w = get_adj_map_gates(pokeverse);
                    //generate new map with gates in those locations
                    map_t new_map;
                    new_map.n = n_s_e_w.nsew[0];
                    new_map.s = n_s_e_w.nsew[1];
                    new_map.e = n_s_e_w.nsew[2];
                    new_map.w = n_s_e_w.nsew[3];

                    //create the new map
                    new_map = pokemon_map_gen(&new_map);
                    *pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y] = new_map;


                    nav_pokeverse(pokeverse);
                    return;
                }
                return;
            break;

            // FLYING
            case 'f':
                scanf(" %d %d", &x_dest, &y_dest);

                x_dest = 200  + x_dest;
                y_dest = 200 - y_dest;

                //border check
                if((y_dest < WORLD_HEIGHT) && (x_dest < WORLD_WIDTH) && (y_dest >= 0) && (x_dest >= 0)) {


                    //now we actually are gonna start flying
                    //is the map already in memory?
                    if(pokeverse->world[x_dest][y_dest] != NULL) {

                        pokeverse->coordinates.x = x_dest;
                        pokeverse->coordinates.y = y_dest;
                        
                    }
                    //no its not in memory
                    else {

                        //malloc the new map
                        pokeverse->world[x_dest][y_dest] = (map_t *)(malloc(sizeof(map_t)));
                        pokeverse->coordinates.x = x_dest;
                        pokeverse->coordinates.y = y_dest;

                        //check adj maps
                        n_s_e_w = get_adj_map_gates(pokeverse);
                        //generate new map with gates in those locations
                        map_t destination;
                        destination.n = n_s_e_w.nsew[0];
                        destination.s = n_s_e_w.nsew[1];
                        destination.e = n_s_e_w.nsew[2];
                        destination.w = n_s_e_w.nsew[3];
                        //create the new map
                        destination = pokemon_map_gen(&destination);
                        *pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y] = destination;
                    }

                }
                //failed border check
                else{
                    printf("If I let you fly of the edge of the world, you would die!\n");
                }
                nav_pokeverse(pokeverse);
                return;
            break;

            default:
                printf("Invalid command: Valid commands are n, s, e, w, fxy, q\n");
                nav_pokeverse(pokeverse);
            break;
        }
    }
    return;

}

/**
 * 
 * @param current_map the map that player is currently on
*/
void draw_curr_map(map_t current_map) {

    generate_the_map(&current_map);

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
int check_north(world_t *pokeverse) {

    if(pokeverse->coordinates.y -1 < 0) {
        return -2;
    }
    map_t *northern_map;
    northern_map = pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y - 1];
    //if the map is already there
    if(northern_map != NULL) {
        return northern_map->s;
    }
    // otherwise return -1
    else {
        return -1;
    }
}

/**
 * Function to check the map the south.
 * If there already exists a map to the south, returns location of northern gate.
 * If there is no map to the south, returns -1.
 * 
 * @param pokeverse the pokemon universe
 * @returns -1, on no map to the south
 * @returns souther_map.n, when there is already a map there.
*/
int check_south(world_t *pokeverse) {

    map_t *souther_map;
    souther_map = pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y + 1];
    //if the map is already there
    if(souther_map != NULL) {
        //return the location of the southern gate
        return souther_map->n;
    }
    // otherwise return -1
    else {
        return -1;
    }
}

/**
 * Function to check the map the east.
 * If there already exists a map to the east, returns location of western gate.
 * If there is no map to the east, returns -1.
 * 
 * @param pokeverse the pokemon universe
 * @returns -1, on no map to the east
 * @returns eastern_map.s, when there is already a map there.
*/
int check_east(world_t *pokeverse) {

    map_t *eastern_map;
    eastern_map = pokeverse->world[pokeverse->coordinates.x + 1][pokeverse->coordinates.y];
    //if the map is already there
    if(eastern_map != NULL) {
        //return the location of the southern gate
        return eastern_map->w;
    }
    // otherwise return -1
    else {
        return -1;
    }
}

/**
 * Function to check the map the west.
 * If there already exists a map to the west, returns location of eastern gate.
 * If there is no map to the west, returns -1.
 * 
 * @param pokeverse the pokemon universe
 * @returns -1, on no map to the west
 * @returns western_map.s, when there is already a map there.
*/
int check_west(world_t *pokeverse) {

    map_t *western_map;
    western_map = pokeverse->world[pokeverse->coordinates.x - 1][pokeverse->coordinates.y];
    //if the map is already there
    if(western_map != NULL) {
        //return the location of the eastern gate
        return western_map->e;
    }
    // otherwise return -1
    else {
        return -1;
    }
}

/**
 * 
 * Function to get the locations of the gates for
 * all of the adjacent maps. Always returns the locations
 * in the order of North, South, East, then West.
 * If any of the values is -1, that means there is not a map
 * there yet.
 * 
 * @param pokeverse the current state of the pokeverse
*/
nsew_t get_adj_map_gates(world_t *pokeverse) {


    map_t *current_map;
    current_map = pokeverse->world[pokeverse->coordinates.x][pokeverse->coordinates.y];


    //struct to hold the values of n, s, e, w
    nsew_t n_s_e_w;

    // check north
    n_s_e_w.nsew[0] = check_north(pokeverse);
    // check south
    n_s_e_w.nsew[1] = check_south(pokeverse);
    // check east
    n_s_e_w.nsew[2] = check_east(pokeverse);
    // check west
    n_s_e_w.nsew[3] = check_west(pokeverse);

    return n_s_e_w;
}

