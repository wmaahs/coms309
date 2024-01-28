//
// Created by Will Maahs on 1/27/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


#define MAP_HEIGHT 21
#define MAP_WIDTH 80
#define MAX_MOUNTAIN_WIDTH 16
#define MIN_MOUNTAIN_WIDTH 5
#define MAX_MOUNTAIN_HEIGHT 6
#define MIN_MOUNTAIN_HEIGHT 3
#define MAX_FOREST_WIDTH 18
#define MIN_FOREST_WIDTH 5
#define MAX_FOREST_HEIGHT 10
#define MIN_FOREST_HEIGHT 4


void gen_map_boarder(char map[MAP_WIDTH][MAP_HEIGHT]);  // function to generate mountains
void gen_short_grass(char map[MAP_WIDTH][MAP_HEIGHT]);   // function to fill whole map with short grass
void gen_mountains(char map[MAP_WIDTH][MAP_HEIGHT]);    // function to plop in mountains
void generate_the_map(char map[MAP_WIDTH][MAP_HEIGHT]);    // final function to print the map to the terminal

struct point{
    int x;
    int y;
};

struct terrain {
    struct point origin;    //the top left corner of the object
    int width;      //th width of the terrain
    int height;     //the height of the terrain
    char symbol;    //symbol for that terrain
};

struct building {
    int pole[2];
    char symbol;
};

/**
 * MAIN FUNCTION
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]){

    //initialize the random number generator
    srand(time(NULL));
    char map[MAP_WIDTH][MAP_HEIGHT];

    //put mountains around the edge
    gen_map_boarder(map);
    gen_short_grass(map);
    gen_mountains(map);
    //print the map to the terminal
    generate_the_map(map);
}

/**
 *
 * @param map
 */
void gen_map_boarder(char map[MAP_WIDTH][MAP_HEIGHT]) {

    int i, j;

    for(i = 0; i < MAP_HEIGHT; i++) {
        for(j = 0; j < MAP_WIDTH; j ++) {

            //top boarder
            if(i == 0) {
                map[j][i] = '^';
            }
            //bottom boarder
            else if(i == MAP_HEIGHT -1) {
                map[j][i] = '^';
            }
            else if(j == 0) {
                map[j][i] = '^';
            }
            else if(j == MAP_WIDTH -1) {
                map[j][i] = '^';
            }
        }
    }
}

/**
 *
 * @param map
 */
void gen_short_grass(char map[MAP_WIDTH][MAP_HEIGHT]) {
    int i, j;

    for(i = 0; i < MAP_HEIGHT; i++){
        for(j = 0; j < MAP_WIDTH; j++) {
            if((j > 0 && j < MAP_WIDTH -1) && (i > 0 && i < MAP_HEIGHT -1)) {
                map[j][i] = '.';
            }
        }
    }
}

/**
 * Generates a random number of mountains (1-4)
 * initialize the mountains, each with a random width (1-20)
 * each with random height (1-9)
 *
 * @param map
 */
void gen_mountains(char map[MAP_WIDTH][MAP_HEIGHT]) {

    int num_mountains = rand() %4 + 1;
    int i;
    struct terrain mountains[num_mountains];

    for(i = 0; i < num_mountains; i++) {
        mountains[i].symbol = '^';
        mountains[i].origin.x = rand() % MAP_WIDTH + 1;
        mountains[i].origin.y = rand() % MAP_HEIGHT + 1;
        mountains[i].width = rand() % MAX_MOUNTAIN_WIDTH + MIN_MOUNTAIN_WIDTH;
        mountains[i].height = rand() % MAX_MOUNTAIN_HEIGHT + MIN_MOUNTAIN_HEIGHT;
    }

    char mountain0[10][10];
    int y, z;
    for(y = 0; y < 10; y++) {
        for(z = 0; z < 10; z++) {
            mountain0[z][y] = '^';
        }
    }
    strcpy(&map[mountains[0].origin.x][mountains[0].origin.y], *mountain0);
    //for testing
    printf("number of mountains %d\n", num_mountains);
    int j;
    for(j = 0; j < num_mountains; j++) {
        printf("mountain %d: width=%d, height=%d\n", j, mountains[j].width, mountains[j].height);
    }
}

/**
 *
 * @param map
 */
void generate_the_map(char map[MAP_WIDTH][MAP_HEIGHT]) {

    int i, j;
    for(i = 0; i < MAP_HEIGHT; i++) {
        for(j = 0; j < MAP_WIDTH; j++) {
            printf("%c", map[j][i]);
            //return every 21 characters to make a box.
            if(j == MAP_WIDTH -1) {
                printf("\n");
            }
        }
    }
}