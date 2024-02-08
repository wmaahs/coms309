#ifndef POKEMON_MAP_GEN_H
#define POKEMON_MAP_GEN_H


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
#define MAX_FIELD_WIDTH 15
#define MIN_FIELD_WIDTH 8
#define MAX_FIELD_HEIGHT 9
#define MIN_FIELD_HEIGHT 6

typedef struct point{
    int x;
    int y;
} point_t;

typedef struct terrain {
    point_t origin;    //the top left corner of the object
    int width;      //th width of the terrain
    int height;     //the height of the terrain
} terrain_t;

typedef struct building {
    int pole[2];
    char symbol;
} building_t;

typedef struct map {
    char map[MAP_WIDTH][MAP_HEIGHT];
    point_t loc;
    int n, s, e, w;
} map_t;

map_t pokemon_map_gen();    //main funciton   //might have to fix this later (argc, argv)
void gen_map_boarder(map_t *map);  // function to generate mountains
void gen_short_grass(map_t *map);   // function to fill whole map with short grass
void gen_mountains(map_t *map);    // function to plop in mountains
void gen_forest(map_t *map);   // function to plop in forests
void gen_tall_grass(map_t *map);
void draw_roads(map_t *map);   // draw the roads between the gates
void gen_buildings(map_t *map); //function to add the buildings on the roads
void extra_things(map_t *map);   //function to throw in extra tress and rocks
void generate_the_map(map_t *map);    // final function to print the map to the terminal


#endif


