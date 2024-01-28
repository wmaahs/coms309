//
// Created by Will Maahs on 1/27/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


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
void generate_the_map(char map[MAP_WIDTH][MAP_HEIGHT]);    // final function to print the map to the terminal
void gen_mountains(char map[MAP_WIDTH][MAP_HEIGHT]);

struct terrain {
    int x;
    int y;
    char symbol;
};

struct building {
    char symbol;
};

int main(int argc, char *argv[]){

    //initialize the random number generator
    srand(time(NULL));
    char map[MAP_WIDTH][MAP_HEIGHT];

    //put mountains around the edge
    gen_map_boarder(map);
    gen_mountains(map);
    //print the map to the terminal
    generate_the_map(map);
}

/*
 *
 *
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

void gen_mountains(char map[MAP_WIDTH][MAP_HEIGHT]) {

    int num_mountains = rand() %4 + 1;
    int i;
    struct terrain mountains[num_mountains];

    for(i = 0; i < num_mountains; i++) {
        mountains[i].symbol = '^';
        mountains[i].x = rand() % MAX_MOUNTAIN_WIDTH + MIN_MOUNTAIN_WIDTH;
        mountains[i].y = rand() % MAX_MOUNTAIN_HEIGHT + MIN_MOUNTAIN_HEIGHT;
    }

    //for testing
    printf("number of mountains %d\n", num_mountains);
    int j;
    for(j = 0; j < num_mountains; j++) {
        printf("mountain %d: x=%d, y=%d\n", j, mountains[j].x, mountains[j].y);
    }
}
/*
 *
 *
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