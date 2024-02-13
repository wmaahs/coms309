//
// Created by Will Maahs on 1/27/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "pokemon_map_gen.h"

/**
 * MAIN FUNCTION
 * @param argc
 * @param argv
 * @return
 */
map_t pokemon_map_gen(map_t *map){

    //initialize the random number generator
    srand(time(NULL));

    //put mountains around the edge
    gen_map_boarder(map);
    gen_short_grass(map);
    gen_mountains(map);
    gen_forest(map);
    gen_tall_grass(map);
    draw_roads(map);
    gen_buildings(map);
    extra_things(map);

    
    return *map;
    //print the map to the terminal
    // generate_the_map(map);
}

/**
 *
 * @param map the 2d array containg the chars for the map
 */
void gen_map_boarder(map_t *map) {

    int i, j;

    for(i = 0; i < MAP_HEIGHT; i++) {
        for(j = 0; j < MAP_WIDTH; j ++) {

            //top boarder
            if(i == 0) {
                map->map[j][i] = '%';
            }
            //bottom boarder
            else if(i == MAP_HEIGHT -1) {
                map->map[j][i] = '%';
            }
            else if(j == 0) {
                map->map[j][i] = '%';
            }
            else if(j == MAP_WIDTH -1) {
                map->map[j][i] = '%';
            }
        }
    }
}

/**
 * Fill the entire map with short grass, inside of the boarder
 *
 * @param map the 2d array containg the chars for the map
 */
void gen_short_grass(map_t *map) {
    int i, j;

    for(i = 0; i < MAP_HEIGHT; i++){
        for(j = 0; j < MAP_WIDTH; j++) {
            if((j > 0 && j < MAP_WIDTH -1) && (i > 0 && i < MAP_HEIGHT -1)) {
                map->map[j][i] = '.';
            }
        }
    }
}

/**
 * Generates a random number of mountains (1-4)
 * initialize the mountains, each with a random width (1-20)
 * each with random height (1-9)
 *
 * @param map the 2d array containg the chars for the map
 */
void gen_mountains(map_t *map) {

    //define a random number of mountains (1-5)
    int num_mountains = rand() %5 + 1;
    int i;
    struct terrain mountains[num_mountains];

    //define random widths, heights, and locations for the tall grass
    for(i = 0; i < num_mountains; i++) {
        mountains[i].origin.x = rand() % MAP_WIDTH + 1;
        mountains[i].origin.y = rand() % MAP_HEIGHT + 1;
        mountains[i].width = rand() % MAX_MOUNTAIN_WIDTH + MIN_MOUNTAIN_WIDTH;
        mountains[i].height = rand() % MAX_MOUNTAIN_HEIGHT + MIN_MOUNTAIN_HEIGHT;
    }
    int k, y, z;
    // draw each mountain on the map
    for(k = 0; k < num_mountains; k++) {
        for(y = 0; y < mountains[k].height; y++) {
            for(z = 0; z < mountains[k].width; z++) {
                //boundary check
                if(z + mountains[k].origin.x >= MAP_WIDTH -1|| y + mountains[k].origin.y >= MAP_HEIGHT -1) {
                    continue;
                }
                //if something is there, don't replace it
                if(map->map[z + mountains[k].origin.x][y + mountains[k].origin.y] != '.') {
                    continue;
                }
                map->map[z + mountains[k].origin.x][y + mountains[k].origin.y] = '%';
            }
        }
    }

}

/**
 * 
 * This is the function that actually prints the map the stdout.
 * 
 * @param map the 2d array containg the chars for the map
 */
void generate_the_map(map_t *map) {

    int i, j;
    for(i = 0; i < MAP_HEIGHT; i++) {
        for(j = 0; j < MAP_WIDTH; j++) {
            printf("%c", map->map[j][i]);
            //return every 21 characters to make a box.
            if(j == MAP_WIDTH -1) {
                printf("\n");
            }
        }
    }
}

/**
 *
 *
 * @param map the 2d array containg the chars for the map
 */
void gen_forest(map_t *map) {

    //define a random number of forests (1-6)
    int num_forests = rand() %6 +1;
    int i;
    struct terrain forests[num_forests];
    //define random widths, heights, and locations for the tall grass
    for(i = 0; i < num_forests; i++) {
        forests[i].origin.x = rand() % MAP_WIDTH + 1;
        forests[i].origin.y = rand() % MAP_HEIGHT + 1;
        forests[i].width = rand() % MAX_FOREST_WIDTH + MIN_FOREST_WIDTH;
        forests[i].height = rand() % MAX_FOREST_HEIGHT + MIN_FOREST_HEIGHT;
    }
    int k, y, z;
    // draw each mountain on the map
    for(k = 0; k < num_forests; k++) {
        for(y = 0; y < forests[k].height; y++) {
            for(z = 0; z < forests[k].width; z++) {
                if(z + forests[k].origin.x >= MAP_WIDTH-1 || y + forests[k].origin.y >= MAP_HEIGHT-1) {
                    continue;
                }
                map->map[z + forests[k].origin.x][y + forests[k].origin.y] = '^';
            }
        }
    }
}

/**
 *
 *
 * @param map the 2d array containg the chars for the map
 */
void gen_tall_grass(map_t *map) {

    //define a random number of fields (1-6)
    int num_fields = rand() %6 + 1;
    int i;
    struct terrain fields[num_fields];

    //define random widths, heights, and locations for the tall grass
    for(i = 0; i < num_fields; i++) {
        fields[i].origin.x = rand() % MAP_WIDTH + 1;
        fields[i].origin.y = rand() % MAP_HEIGHT + 1;
        fields[i].width = rand() % MAX_FIELD_WIDTH + MIN_FIELD_WIDTH;
        fields[i].height = rand() % MAX_FIELD_HEIGHT + MIN_FIELD_HEIGHT;
    }
    int k, y, z;
    // draw the grass on the map
    for(k = 0; k < num_fields; k++) {
        for(y = 0; y < fields[k].height; y++) {
            for(z = 0; z < fields[k].width; z++) {
                if(z + fields[k].origin.x >= MAP_WIDTH -1 || y + fields[k].origin.y >= MAP_HEIGHT -1) {
                    continue;
                }
                map->map[z + fields[k].origin.x][y + fields[k].origin.y] = ':';
            }
        }
    }
}

/**
 * Draw the roads on the map. If we are drawing a map whose
 * gate locations are predetermined, then use those locations.
 * Otherwise uses a random location for the gate.
 * 
 * @param map the 2d array containg the chars for the map
 */
void draw_roads(map_t *map) {

    int north_gate, south_gate, east_gate, west_gate;

    //SELECT NORTH GATE
    if(map->n != -1) {
        north_gate = map->n;
    }
    else{
        north_gate = rand() % (MAP_WIDTH -2) + 3;
        //move the gates closer to the center of the map
        //improved appearance
        if(north_gate < 2 || north_gate > MAP_WIDTH -3) {
            north_gate = 29;
        }
    }
    //SELECT SOUTH GATE
    if(map->s != -1) {
        south_gate = map->s;
    }
    else {
        south_gate = rand() % (MAP_WIDTH -2) + 3;
        //move the gates closer to the center of the map
        //improved appearance
        if(south_gate < 2 || south_gate > MAP_WIDTH -3) {
            south_gate = 34;
        }
    }
    //SELECT EAST GATE
    if(map->e != -1) {
        east_gate = map->e;
    }
    else {
        east_gate = rand() % (MAP_HEIGHT -1) + 2;
        //move the gates closer to the center of the map
        //improved appearance
        if(east_gate < 2 || east_gate > MAP_HEIGHT -3) {
            east_gate = 9;
        }
    }
    //SELECT WEST GATE
    if(map->w != -1) {
        west_gate = map->w;
    }
    else {
        west_gate = rand() % (MAP_HEIGHT -1) +2;
        //move the gates closer to the center of the map
        //improved appearance
        if(west_gate < 2 || west_gate > MAP_HEIGHT -3) {
            west_gate = 6;
        }
    }
    
    //set the gates for map_t
    map->e = east_gate;
    map->w = west_gate;
    map->n = north_gate;
    map->s = south_gate;

    //draw the gates
    map->map[0][west_gate] = '#';
    map->map[MAP_WIDTH -1][east_gate] = '#';
    map->map[north_gate][0] = '#';
    map->map[south_gate][MAP_HEIGHT -1] = '#';

    int y_intersect = rand() % (MAP_WIDTH -2) + 4;
    int x_intersect = rand() % (MAP_HEIGHT -2) + 4;

    //move the intersects closer to the middle
    if(x_intersect < 2) {
        x_intersect += 5;
    }
    if(x_intersect > MAP_WIDTH -2){
        x_intersect -= 5;
    }
    if(y_intersect < 2) {
        y_intersect += 5;
    }
    if(y_intersect > MAP_HEIGHT -2){
        y_intersect -= 5;
    }

    int i;
    //draw the road from west -> middle
    for(i = 0; i < y_intersect; i++){
        map->map[i][west_gate] = '#';
    }

    //draw the road from east -> middle
    for(i = y_intersect; i < MAP_WIDTH -1; i++) {
        map->map[i][east_gate] = '#';
    }

    //draw the road from north -> middle
    for(i = 0; i < x_intersect; i++) {
        map->map[north_gate][i] = '#';
    }

    //draw the road from south -> middle
    for(i = x_intersect; i < MAP_HEIGHT -1; i++) {
        map->map[south_gate][i] = '#';
    }

    // connect the east-west roads in the middle
    if(east_gate < west_gate) {
        //east gate is lower than west gate
        for(i = east_gate; i <= west_gate; i++){
            map->map[y_intersect][i] = '#';
        }
    }
    else {
        //west gate is lower than east gate
        for(i = west_gate; i <= east_gate; i++) {

            map->map[y_intersect][i] = '#';
        }
    }
    //connect the roads in the middle
    if(south_gate < north_gate) {
        //south gate is left of north gate
        for(i = south_gate; i <= north_gate; i++){
            map->map[i][x_intersect] = '#';
        }
    }
    else {
        //north gate is left of south gate
        for(i = north_gate; i <= south_gate; i++) {

            map->map[i][x_intersect] = '#';
        }
    }

}

/**
 *
 * @param map the 2d array containg the chars for the map
 */
void gen_buildings(map_t *map) {

    int poke_flag = 0;  //flag to exit loops
    int mart_flag = 0;  //flag to exit loops
    int i, j;

    //logic to skip placing the buildings
    int manhatten;
    double freq;
    double mult;
    manhatten = abs(map->loc.x - 200) + abs(map->loc.y - 200);
    if(manhatten < 200) {
        mult = manhatten * -45;
        mult = mult / 200;
        freq = (mult + 50) / 100;
    }
    else {
        freq = .05;
    }
    int freq_int;
    freq_int = 1/freq;

    //pick a random number between 1 and freq
    int rand_skip = rand() % freq_int + 1;

    //if the random number doesnt match then skip
    //if 0,0 always do the buildings
    if((map->loc.x == 200) && (map->loc.y == 200)) {
        rand_skip = 1;
    }
    else if(rand_skip != 1) {
        return;
    }
    //generate the pokecenter next to a road with 10 < x < 70
    //find the road, put it below or to the right of the road.
    // 4 < y < 10, so that it is closer to the center
    for(i = 0; i < MAP_HEIGHT; i++) {
        for(j = 0; j < MAP_WIDTH; j++) {
            if((j > 10) && (j < 70) && (map->map[j][i] == '#') && (i > 4) && (i < 10)) {
                // if the road is moving east-west, then go below
                if ((map->map[j + 1][i] == '#') || (map->map[j - 1][i] == '#')) {
                    map->map[j][i - 1] = 'P';
                    map->map[j][i - 2] = 'P';
                    map->map[j + 1][i - 1] = 'P';
                    map->map[j + 1][i - 2] = 'P';
                    poke_flag = 1;
                    break;
                }
                    //if the road is moving north-south, then go to the right
                else {
                    map->map[j + 1][i] = 'P';
                    map->map[j + 2][i] = 'P';
                    map->map[j + 1][i + 1] = 'P';
                    map->map[j + 2][i + 1] = 'P';
                    poke_flag = 1;
                    break;
                }
            }
        }
        if(poke_flag) {
            break;
        }
    }
    //generate the pokemart next to road with 15 < x < 65
    //find the road, put it below or to the right of the road.
    // 11 < y < 19
    for(i = 0; i < MAP_HEIGHT; i++) {
        for(j = 0; j < MAP_WIDTH; j++) {
            if((j > 15) && (j < 65) && (map->map[j][i] == '#') && (i > 11) && (i < 19)){
                // if the road is moving east-west, then go below
                if((map->map[j+1][i] == '#') || (map->map[j-1][i] == '#')) {
                    map->map[j][i-1] = 'M';
                    map->map[j][i-2] = 'M';
                    map->map[j+1][i-1] = 'M';
                    map->map[j+1][i-2] = 'M';
                    mart_flag = 1;  //flag to break out of loops once drawn
                    break;
                }
                //if the road is moving north-south, then go to the right
                else {
                    map->map[j+1][i] = 'M';
                    map->map[j+2][i] = 'M';
                    map->map[j+1][i+1] = 'M';
                    map->map[j+2][i+1] = 'M';
                    mart_flag = 1;  //flag to break out of loops once drawn
                    break;
                }

            }
        }
        if(mart_flag) {
            break;
        }
    }
}

/**
 * Function to throw a random number (10-30) of rocks and trees in at the end
 * every other obejct placed is a rock or a tree.
 * @param map the 2d array containg the chars for the map
 */
void extra_things(map_t *map) {

    //define random number of things
    int num_things = rand() % 20 + 10;

    int i;
    for(i =0; i <= num_things; i++) {
        int rand_x = rand() %77 + 1;
        int rand_y = rand() %18 + 1;
        if(map->map[rand_x][rand_y] != 'P' && map->map[rand_x][rand_y] != 'M' && map->map[rand_x][rand_y] != '#') {

            if(i%2) {

                map->map[rand_x][rand_y] = '^';
            }
            else{
                map->map[rand_x][rand_y] = '%';
            }
        }

    }

}
