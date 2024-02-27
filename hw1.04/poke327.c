#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include "heap.h"

#define malloc(size) ({          \
  void *_tmp;                    \
  assert((_tmp = malloc(size))); \
  _tmp;                          \
})

typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef int16_t pair_t[num_dims];

typedef struct path {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
} path_t;

typedef struct trainer_path {

  heap_node_t *hn;
  pair_t from;
  pair_t to;
  int32_t cost;

} trainer_path_t;



#define MAP_X              80
#define MAP_Y              21
#define MIN_TREES          10
#define MIN_BOULDERS       10
#define TREE_PROB          95
#define BOULDER_PROB       95
#define WORLD_SIZE         401

#define mappair(pair) (m->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (m->map[y][x])
#define heightpair(pair) (m->height[pair[dim_y]][pair[dim_x]])
#define heightxy(x, y) (m->height[y][x])

#define hiker_distance_pair(pair) (world->hiker_distance[pair[dim_y]][pair[dim_x]])
#define hiker_distance_xy(x, y) (world->hiker_distance[y][x])

#define rival_distance_pair(pair) (world->rival_distance[pair[dim_y]][pair[dim_x]])
#define rival_distance_xy(x, y) (world->rival_distance[y][x])

typedef enum __attribute__ ((__packed__)) terrain_type {
  ter_debug,
  ter_boulder,
  ter_tree,
  ter_path,
  ter_mart,
  ter_center,
  ter_grass,
  ter_clearing,
  ter_mountain,
  ter_forest,
  ter_water,
  ter_gate,
  ter_pc
  // ter_hiker,
  // ter_rival,
  // ter_sentry,
  // ter_wanderer
} terrain_type_t;

typedef enum trainer_type {
  trainer_pc,
  trainer_hiker,
  trainer_rival,
  trainer_sentry,
  trainer_wanderer,
  num_trainer_types
} trainer_t;


/*TODO fix the distance_pair and xy
so that they work in the dijkstras algorithm
*/
typedef struct map {
  terrain_type_t map[MAP_Y][MAP_X];
  uint8_t height[MAP_Y][MAP_X];
  int8_t n, s, e, w;
} map_t;

typedef struct queue_node {
  int x, y;
  struct queue_node *next;
} queue_node_t;

typedef struct player_character {

  pair_t coordinates;
  int next_turn;
  int seq_num;

} pc_t;

typedef struct character {

  trainer_t title;
  pair_t position;
  int next_turn;
  int seq_num;

} character_t;


typedef struct world {
  map_t *world[WORLD_SIZE][WORLD_SIZE];
  pair_t cur_idx;
  map_t *cur_map;
  int hiker_distance[MAP_Y][MAP_X];
  int rival_distance[MAP_Y][MAP_X];
  character_t *character_map[MAP_Y][MAP_X];
  pc_t player_character;
  int seq_num;
} world_t;




/* Even unallocated, a WORLD_SIZE x WORLD_SIZE array of pointers is a very *
 * large thing to put on the stack.  To avoid that, world is a global.     */
world_t world;

static pc_t place_pc(map_t *map)
{
  pc_t pc;
  int i, j;
  for (i = 0; i < MAP_Y; i++)
  {
    for (j = 0; j < MAP_X; j++)
    {
      // temporarily set a range for the pc to be in. Must be road.
      if ((map->map[i][j] == ter_path) && (i > 5) && (i < 19) && (j > 5) && (j < 75))
      {
        map->map[i][j] = ter_pc;
        pc.coordinates[dim_x] = j;
        pc.coordinates[dim_y] = i;
        return pc;
      }
    }
  }

  // this should never happen
  pc.coordinates[dim_x] = -1;
  pc.coordinates[dim_y] = -1;
  return pc;
}

//This will also need to add characters to the queue and to the character map
// if I pass the characters by address even if they are created in the loop,
//will the addresses still point to the same data?
int spawn_trainers(world_t *world, int num_trainers) {

  int x;
  //  map_t *map = world->cur_map;
  if(num_trainers > 2) {
    
    for(x = 0; x < num_trainers; x++) {

      //make sure you add at least one rival
      if (x == 0) {
        character_t first_rival;
        first_rival.seq_num = world->seq_num;
        world->seq_num++;
        first_rival.title = trainer_rival;
        first_rival.position[dim_x] = rand() % (MAP_X - 1) + 1;
        first_rival.position[dim_x] = rand() % (MAP_Y - 1) + 1;

	if(first_rival.position[dim_x] > 0) {
	  printf("first_rival success\n");
	}
      }
      //and at least one hiker
      if(x == 1) {
        character_t first_hiker;
        first_hiker.seq_num = world->seq_num;
        world->seq_num++;
        first_hiker.title = trainer_hiker;
        first_hiker.position[dim_x] = rand() % (MAP_X - 1) + 1;
        first_hiker.position[dim_y] = rand() % (MAP_Y - 1) + 1;
	world->character_map[first_hiker.position[dim_x]][first_hiker.position[dim_y]] = &first_hiker;
      }
      //after that pick randomly
      if(x > 1) {
        //pick a random character that isn't the PC
        int character = rand() % (num_trainer_types-1) + 1;
        character_t cur_character;
        cur_character.seq_num = world->seq_num;
        world->seq_num++;
        cur_character.title = character;
        if(cur_character.title == trainer_hiker) {
          printf("curr_char type: hiker\n");
        }
        else if(cur_character.title == trainer_rival) {
          printf("curr_char type: rival\n");
        }
        else if(cur_character.title == trainer_sentry) {
          printf("curr_char type: sentry\n");
        }
        else if(cur_character.title == trainer_wanderer) {
          printf("curr_char type: wander\n");
        }
        else {
          printf("curr_char type: SOMETHING NOT RIGHT!!\n");
        }
      }
    }
    return num_trainers;
  }
  else if (num_trainers == 1) {
    character_t solo_rival;
    solo_rival.seq_num = 0;
    solo_rival.title = trainer_rival;
    if(solo_rival.title == trainer_rival) {
      printf("solo rival success\n");
    }
    return 1;
  }
  else {
    return 0;
  }
  
  return num_trainers;
}

static int32_t trainer_path_cmp(const void *key, const void *with) {
  return ((trainer_path_t *) key)->cost - ((trainer_path_t *) with)->cost;
}

/*
TODO
After that we can have one function to call them both in a loop from every location

fix the way we adjust the cost. Should maybe call cost distance.
in main if we really wanted to we could put the pc in ever spot and 
run shortest path from ever possible combo.
*/
static void dijkstra_rival_path(world_t * world, pair_t to)
{

  //pair_t to can be thought of as the PC's location
  //pair_t from can be thought of as hiker's location

  // create data local structures
  static trainer_path_t rival_path[MAP_Y][MAP_X], *rp; // might want to consider change from static
  heap_t heap;
  int x, y;
  map_t * map = world->cur_map;

  //create map of cost for each terrain_t
  for (y = 0; y < MAP_Y; y++)
  {
    for (x = 0; x < MAP_X; x++)
    {
      switch (map->map[y][x])
      {
      case ter_grass:
        rival_distance_xy(x, y) = 15;
        break;
      case ter_clearing:
        rival_distance_xy(x, y) = 10;
        break;
      case ter_path:
        rival_distance_xy(x, y) = 10;
        break;
      case ter_center:
        rival_distance_xy(x, y) = 50;
        break;
      case ter_mart:
        rival_distance_xy(x, y) = 50;
        break;
      case ter_pc:
        rival_distance_xy(x, y) = 10;
        break;
      default:
        rival_distance_xy(x, y) = INT_MAX;
        break;
      }
    }
  }
    
  // set all the nodes cost as infinity
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      rival_path[y][x].hn = NULL;
      rival_path[y][x].from[dim_y] = y;
      rival_path[y][x].from[dim_x] = x;
      rival_path[y][x].to[dim_y] = to[dim_y];
      rival_path[y][x].to[dim_x] = to[dim_x];
      rival_path[y][x].cost = INT_MAX;
    }
  }

  //Assign distance 0 of pc to 
  rival_path[to[dim_y]][to[dim_x]].cost = 0;

  //Step 1. Create a set of unvisited nodes
  heap_init(&heap, trainer_path_cmp, NULL);
  
  for (y = 1; y < MAP_Y -1; y++) {
    for (x = 1; x < MAP_X -1; x++) {
      //ignore if uncreachable
      if(rival_distance_xy(x, y) != INT_MAX){
        rival_path[y][x].hn = heap_insert(&heap, &rival_path[y][x]);
      }
    }
  }

  //Step 3. For the current node, consider all of its unvisited neighbors
  while ((rp = heap_remove_min(&heap)))
  {
    rp->hn = NULL;


    /*
    NEIGHBOR BELOW
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((rival_path[rp->from[dim_y] - 1][rp->from[dim_x]].hn) &&
        ((rival_path[rp->from[dim_y] - 1][rp->from[dim_x]].cost) > (rp->cost + rival_distance_pair(rp->from))))
    { 
      rival_path[rp->from[dim_y] - 1][rp->from[dim_x]].cost = rp->cost + rival_distance_pair(rp->from);
      heap_decrease_key_no_replace(&heap, rival_path[rp->from[dim_y] - 1][rp->from[dim_x]].hn);
    }
    /* 
    NEIGHBOR LEFT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((rival_path[rp->from[dim_y]][rp->from[dim_x] - 1].hn) && 
        (rival_path[rp->from[dim_y]][rp->from[dim_x] - 1].cost > (rp->cost + rival_distance_pair(rp->from))))
    {
      
      rival_path[rp->from[dim_y]][rp->from[dim_x] - 1].cost = (rp->cost + rival_distance_pair(rp->from));
      heap_decrease_key_no_replace(&heap, rival_path[rp->from[dim_y]][rp->from[dim_x] - 1].hn);
    }
    /*
    NEIGHBOR RIGHT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((rival_path[rp->from[dim_y]][rp->from[dim_x] + 1].hn) &&
        (rival_path[rp->from[dim_y]][rp->from[dim_x] + 1].cost > (rp->cost + rival_distance_pair(rp->from))))
    {
      rival_path[rp->from[dim_y]][rp->from[dim_x] + 1].cost = (rp->cost + rival_distance_pair(rp->from));
      heap_decrease_key_no_replace(&heap, rival_path[rp->from[dim_y]][rp->from[dim_x] + 1].hn);
    }
    /*
    NEIGHBOR ABOVE
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.

    delete the edge penalty
    */
    if ((rival_path[rp->from[dim_y] + 1][rp->from[dim_x]].hn) &&
        (rival_path[rp->from[dim_y] + 1][rp->from[dim_x]].cost > (rp->cost + rival_distance_pair(rp->from))))
    {
      rival_path[rp->from[dim_y] + 1][rp->from[dim_x]].cost = (rp->cost + rival_distance_pair(rp->from));
      heap_decrease_key_no_replace(&heap, rival_path[rp->from[dim_y] + 1][rp->from[dim_x]].hn);
    }

        /*
    NEIGHBOR DOWN-RIGHT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((rival_path[rp->from[dim_y] - 1][rp->from[dim_x] + 1].hn) &&
        ((rival_path[rp->from[dim_y] - 1][rp->from[dim_x] + 1].cost) > (rp->cost + rival_distance_pair(rp->from))))
    {
      rival_path[rp->from[dim_y] - 1][rp->from[dim_x] + 1].cost = rp->cost + rival_distance_pair(rp->from);
      heap_decrease_key_no_replace(&heap, rival_path[rp->from[dim_y] - 1][rp->from[dim_x] + 1].hn);
    }

        /*
    NEIGHBOR DOWN-LEFT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((rival_path[rp->from[dim_y] - 1][rp->from[dim_x] - 1].hn) &&
        ((rival_path[rp->from[dim_y] - 1][rp->from[dim_x] - 1].cost) > (rp->cost + rival_distance_pair(rp->from))))
    {
      rival_path[rp->from[dim_y] - 1][rp->from[dim_x] - 1].cost = rp->cost + rival_distance_pair(rp->from);
      heap_decrease_key_no_replace(&heap, rival_path[rp->from[dim_y] - 1][rp->from[dim_x] - 1].hn);
    }

        /*
    NEIGHBOR UP-RIGHT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((rival_path[rp->from[dim_y] + 1][rp->from[dim_x ] + 1].hn) &&
        ((rival_path[rp->from[dim_y] + 1][rp->from[dim_x] + 1].cost) > (rp->cost + rival_distance_pair(rp->from))))
    {
      rival_path[rp->from[dim_y] + 1][rp->from[dim_x] + 1].cost = rp->cost + rival_distance_pair(rp->from);
      heap_decrease_key_no_replace(&heap, rival_path[rp->from[dim_y] + 1][rp->from[dim_x] + 1].hn);
    }

        /*
    NEIGHBOR UP-LEFT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((rival_path[rp->from[dim_y] + 1][rp->from[dim_x] - 1].hn) &&
        ((rival_path[rp->from[dim_y] + 1][rp->from[dim_x] - 1].cost) > (rp->cost + rival_distance_pair(rp->from))))
    {
      rival_path[rp->from[dim_y] + 1][rp->from[dim_x] - 1].cost = rp->cost + rival_distance_pair(rp->from);
      heap_decrease_key_no_replace(&heap, rival_path[rp->from[dim_y] + 1][rp->from[dim_x] -1].hn);
    }
  }

  //write the cost for each node in the distance map 
  for(y = 0; y < MAP_Y; y++) {
    for(x = 0; x < MAP_X; x++) {

      // if node is unreachable, print a space
      if (rival_path[y][x].cost == INT_MAX)
      {
        //print new lines for prettiness
        if (x != MAP_X - 1)
        {
          printf("   ");
        }
        else
        {
          printf("   \n");
        }
      }
      // also if node is unreachable, print a space
      else if (rival_path[y][x].cost < 0)
      {
        if (x != MAP_X - 1)
        {
          printf("   ");
        }
        else
        {
          printf("   \n");
        }
      }
      // the ones we care about
      else
      {
        if (x != MAP_X - 1)
        {
          printf("%02d ", rival_path[y][x].cost % 100);
        }
        else
        {
          printf("%02d \n", rival_path[y][x].cost % 100);
        }
      }
    }
  }
  return;
}
/*
* Hikers Dijkstra
*/
static void dijkstra_hiker_path(world_t *world, pair_t to)
{

  //pair_t "to" can be thought of as the PC's location
  //pair_t "from" can be thought of as hiker's location

  //set of all the nodes
  static trainer_path_t hiker_path[MAP_Y][MAP_X], *hp;    //might want to consider change from static
  heap_t heap;
  int x, y;
  map_t *map = world->cur_map;

  //create map of cost for each terrain_t
  for (y = 0; y < MAP_Y; y++)
  {
    for (x = 0; x < MAP_X; x++)
    {
      switch (map->map[y][x])
      {
      case ter_grass:
        hiker_distance_xy(x, y) = 15;
        break;
      case ter_clearing:
        hiker_distance_xy(x, y) = 10;
        break;
      case ter_path:
        hiker_distance_xy(x, y) = 10;
        break;
      case ter_center:
        hiker_distance_xy(x, y) = 50;
        break;
      case ter_mart:
        hiker_distance_xy(x, y) = 50;
        break;
      case ter_forest:
        hiker_distance_xy(x, y) = 15;
        break;
      case ter_mountain:
        hiker_distance_xy(x, y) = 15;
        break;
      case ter_pc:
        rival_distance_xy(x, y) = 10;
        break;
      default:
        hiker_distance_xy(x, y) = INT_MAX;
        break;
      }
    }
  }
  //label each node
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      hiker_path[y][x].hn = NULL;
      hiker_path[y][x].from[dim_y] = y;
      hiker_path[y][x].from[dim_x] = x;
      hiker_path[y][x].to[dim_y] = to[dim_y];
      hiker_path[y][x].to[dim_x] = to[dim_x];
      hiker_path[y][x].cost = INT_MAX;

    }
  }


  //Assing the PC's node to have a cost of 0
  hiker_path[to[dim_y]][to[dim_x]].cost = 0;

  //Step 1. Create a set of unvisited nodes
  heap_init(&heap, trainer_path_cmp, NULL);
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if(hiker_distance_xy(x, y) != INT_MAX) {
        hiker_path[y][x].hn = heap_insert(&heap, &hiker_path[y][x]);
      }
    }
  }

  //Step 3. For the current node, consider all of its unvisited neighbors
  while ((hp = heap_remove_min(&heap)))
  {
    hp->hn = NULL;

    /*
    * NEIGHBOR BELOW
    * if the cost of traveling through current node is 
    * less than the previous cost and neighbor is unvisited and exits in the heap
    * then decrease the heap.
    */
    if ((hiker_path[hp->from[dim_y] - 1][hp->from[dim_x]].hn) &&
        ((hiker_path[hp->from[dim_y] - 1][hp->from[dim_x]].cost) > (hp->cost + hiker_distance_pair(hp->from))))
    {
      hiker_path[hp->from[dim_y] - 1][hp->from[dim_x]].cost = hp->cost + hiker_distance_pair(hp->from);
      heap_decrease_key_no_replace(&heap, hiker_path[hp->from[dim_y] - 1][hp->from[dim_x]].hn);
    }
    /*
    NEIGHBOR LEFT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((hiker_path[hp->from[dim_y]][hp->from[dim_x] - 1].hn) && 
        (hiker_path[hp->from[dim_y]][hp->from[dim_x] - 1].cost > (hp->cost + hiker_distance_pair(hp->from))))
    {
      
      hiker_path[hp->from[dim_y]][hp->from[dim_x] - 1].cost = (hp->cost + hiker_distance_pair(hp->from));
      heap_decrease_key_no_replace(&heap, hiker_path[hp->from[dim_y]][hp->from[dim_x] - 1].hn);
    }
    /*
    NEIGHBOR RIGHT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((hiker_path[hp->from[dim_y]][hp->from[dim_x] + 1].hn) &&
        (hiker_path[hp->from[dim_y]][hp->from[dim_x] + 1].cost > (hp->cost + hiker_distance_pair(hp->from))))
    {
      hiker_path[hp->from[dim_y]][hp->from[dim_x] + 1].cost = (hp->cost + hiker_distance_pair(hp->from));
      heap_decrease_key_no_replace(&heap, hiker_path[hp->from[dim_y]][hp->from[dim_x] + 1].hn);
    }
    /*
    NEIGHBOR ABOVE
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.

    delete the edge penalty
    */
    if ((hiker_path[hp->from[dim_y] + 1][hp->from[dim_x]].hn) &&
        (hiker_path[hp->from[dim_y] + 1][hp->from[dim_x]].cost > (hp->cost + hiker_distance_pair(hp->from))))
    {
      hiker_path[hp->from[dim_y] + 1][hp->from[dim_x]].cost = (hp->cost + hiker_distance_pair(hp->from));
      heap_decrease_key_no_replace(&heap, hiker_path[hp->from[dim_y] + 1][hp->from[dim_x]].hn);
    }

        /*
    NEIGHBOR DOWN-RIGHT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((hiker_path[hp->from[dim_y] - 1][hp->from[dim_x] + 1].hn) &&
        ((hiker_path[hp->from[dim_y] - 1][hp->from[dim_x] + 1].cost) > (hp->cost + hiker_distance_pair(hp->from))))
    {
      hiker_path[hp->from[dim_y] - 1][hp->from[dim_x] + 1].cost = hp->cost + hiker_distance_pair(hp->from);
      heap_decrease_key_no_replace(&heap, hiker_path[hp->from[dim_y] - 1][hp->from[dim_x] + 1].hn);
    }

        /*
    NEIGHBOR DOWN-LEFT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((hiker_path[hp->from[dim_y] - 1][hp->from[dim_x] - 1].hn) &&
        ((hiker_path[hp->from[dim_y] - 1][hp->from[dim_x] - 1].cost) > (hp->cost + hiker_distance_pair(hp->from))))
    {
      hiker_path[hp->from[dim_y] - 1][hp->from[dim_x] - 1].cost = hp->cost + hiker_distance_pair(hp->from);
      heap_decrease_key_no_replace(&heap, hiker_path[hp->from[dim_y] - 1][hp->from[dim_x] - 1].hn);
    }

        /*
    NEIGHBOR UP-RIGHT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((hiker_path[hp->from[dim_y] + 1][hp->from[dim_x ] + 1].hn) &&
        ((hiker_path[hp->from[dim_y] + 1][hp->from[dim_x] + 1].cost) > (hp->cost + hiker_distance_pair(hp->from))))
    {
      hiker_path[hp->from[dim_y] + 1][hp->from[dim_x] + 1].cost = hp->cost + hiker_distance_pair(hp->from);
      heap_decrease_key_no_replace(&heap, hiker_path[hp->from[dim_y] + 1][hp->from[dim_x] + 1].hn);
    }

        /*
    NEIGHBOR UP-LEFT
    if the old cost is greater and neighbor is unvisited
    than the new cost after visiting through neighbor,
    replace the cost
    then decrease the heap.
    */
    if ((hiker_path[hp->from[dim_y] + 1][hp->from[dim_x] - 1].hn) &&
        ((hiker_path[hp->from[dim_y] + 1][hp->from[dim_x] - 1].cost) > (hp->cost + hiker_distance_pair(hp->from))))
    {
      hiker_path[hp->from[dim_y] + 1][hp->from[dim_x] - 1].cost = hp->cost + hiker_distance_pair(hp->from);
      heap_decrease_key_no_replace(&heap, hiker_path[hp->from[dim_y] + 1][hp->from[dim_x] -1].hn);
    }
  }

  //write the cost for each node in the distance map 
  for(y = 0; y < MAP_Y; y++) {
    for(x = 0; x < MAP_X; x++) {

      // if node is unreachable, print a space
      if (hiker_path[y][x].cost == INT_MAX)
      {
        //print new lines for prettiness
        if (x != MAP_X - 1)
        {
          printf("   ");
        }
        else
        {
          printf("   \n");
        }
      }
      // also if node is unreachable, print a space
      else if (hiker_path[y][x].cost < 0)
      {
        if (x != MAP_X - 1)
        {
          printf("   ");
        }
        else
        {
          printf("   \n");
        }
      }
      // the real ones
      else
      {
        if (x != MAP_X - 1)
        {
          printf("%02d ", hiker_path[y][x].cost % 100);
        }
        else
        {
          printf("%02d \n", hiker_path[y][x].cost % 100);
        }
      }
    }
  }

  return;
}

//given comparator.
static int32_t path_cmp(const void *key, const void *with) {
  return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

static int32_t edge_penalty(int8_t x, int8_t y)
{
  return (x == 1 || y == 1 || x == MAP_X - 2 || y == MAP_Y - 2) ? 2 : 1;
}

static void dijkstra_path(map_t *m, pair_t from, pair_t to)
{
  static path_t path[MAP_Y][MAP_X], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) {
    for (y = 0; y < MAP_Y; y++) {
      for (x = 0; x < MAP_X; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[from[dim_y]][from[dim_x]].cost = 0;

  heap_init(&h, path_cmp, NULL);

  for (y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      path[y][x].hn = heap_insert(&h, &path[y][x]);
    }
  }

  while ((p = heap_remove_min(&h))) {
    p->hn = NULL;

    if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x]) {
      for (x = to[dim_x], y = to[dim_y];
           (x != from[dim_x]) || (y != from[dim_y]);
           p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y]) {
        // Don't overwrite the gate
        if (x != to[dim_x] || y != to[dim_y]) {
          mapxy(x, y) = ter_path;
          heightxy(x, y) = 0;
        }
      }
      heap_delete(&h);
      return;
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x], p->pos[dim_y] - 1))))
    {
      path[p->pos[dim_y] - 1][p->pos[dim_x]].cost =
          ((p->cost + heightpair(p->pos)) *
           edge_penalty(p->pos[dim_x], p->pos[dim_y] - 1));
      path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]]
                                               .hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x] - 1, p->pos[dim_y])))) {
      path[p->pos[dim_y]][p->pos[dim_x] - 1].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x] - 1, p->pos[dim_y]));
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x] + 1, p->pos[dim_y])))) {
      path[p->pos[dim_y]][p->pos[dim_x] + 1].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x] + 1, p->pos[dim_y]));
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x], p->pos[dim_y] + 1)))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x], p->pos[dim_y] + 1));
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x]    ].hn);
    }
  }
}

static int build_paths(map_t *m)
{
  pair_t from, to;

  /*  printf("%d %d %d %d\n", m->n, m->s, m->e, m->w);*/

  if (m->e != -1 && m->w != -1) {
    from[dim_x] = 1;
    to[dim_x] = MAP_X - 2;
    from[dim_y] = m->w;
    to[dim_y] = m->e;

    dijkstra_path(m, from, to);
  }

  if (m->n != -1 && m->s != -1) {
    from[dim_y] = 1;
    to[dim_y] = MAP_Y - 2;
    from[dim_x] = m->n;
    to[dim_x] = m->s;

    dijkstra_path(m, from, to);
  }

  if (m->e == -1) {
    if (m->s == -1) {
      from[dim_x] = 1;
      from[dim_y] = m->w;
      to[dim_x] = m->n;
      to[dim_y] = 1;
    } else {
      from[dim_x] = 1;
      from[dim_y] = m->w;
      to[dim_x] = m->s;
      to[dim_y] = MAP_Y - 2;
    }

    
    dijkstra_path(m, from, to);
  }

  if (m->w == -1) {
    if (m->s == -1) {
      from[dim_x] = MAP_X - 2;
      from[dim_y] = m->e;
      to[dim_x] = m->n;
      to[dim_y] = 1;
    } else {
      from[dim_x] = MAP_X - 2;
      from[dim_y] = m->e;
      to[dim_x] = m->s;
      to[dim_y] = MAP_Y - 2;
    }

    dijkstra_path(m, from, to);
  }

  if (m->n == -1) {
    if (m->e == -1) {
      from[dim_x] = 1;
      from[dim_y] = m->w;
      to[dim_x] = m->s;
      to[dim_y] = MAP_Y - 2;
    } else {
      from[dim_x] = MAP_X - 2;
      from[dim_y] = m->e;
      to[dim_x] = m->s;
      to[dim_y] = MAP_Y - 2;
    }

    dijkstra_path(m, from, to);
  }

  if (m->s == -1) {
    if (m->e == -1) {
      from[dim_x] = 1;
      from[dim_y] = m->w;
      to[dim_x] = m->n;
      to[dim_y] = 1;
    } else {
      from[dim_x] = MAP_X - 2;
      from[dim_y] = m->e;
      to[dim_x] = m->n;
      to[dim_y] = 1;
    }

    dijkstra_path(m, from, to);
  }

  return 0;
}

static int gaussian[5][5] = {
  {  1,  4,  7,  4,  1 },
  {  4, 16, 26, 16,  4 },
  {  7, 26, 41, 26,  7 },
  {  4, 16, 26, 16,  4 },
  {  1,  4,  7,  4,  1 }
};

static int smooth_height(map_t *m)
{
  int32_t i, x, y;
  int32_t s, t, p, q;
  queue_node_t *head, *tail, *tmp;
  FILE *out;
  uint8_t height[MAP_Y][MAP_X];

  memset(&height, 0, sizeof (height));

  /* Seed with some values */
  for (i = 1; i < 255; i += 20) {
    do {
      x = rand() % MAP_X;
      y = rand() % MAP_Y;
    } while (height[y][x]);
    height[y][x] = i;
    if (i == 1) {
      head = tail = malloc(sizeof (*tail));
    } else {
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
    }
    tail->next = NULL;
    tail->x = x;
    tail->y = y;
  }

  /*
  out = fopen("seeded.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&height, sizeof (height), 1, out);
  fclose(out);
  */
  
  /* Diffuse the vaules to fill the space */
  while (head) {
    x = head->x;
    y = head->y;
    i = height[y][x];

    if (x - 1 >= 0 && y - 1 >= 0 && !height[y - 1][x - 1]) {
      height[y - 1][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y - 1;
    }
    if (x - 1 >= 0 && !height[y][x - 1]) {
      height[y][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y;
    }
    if (x - 1 >= 0 && y + 1 < MAP_Y && !height[y + 1][x - 1]) {
      height[y + 1][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y + 1;
    }
    if (y - 1 >= 0 && !height[y - 1][x]) {
      height[y - 1][x] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x;
      tail->y = y - 1;
    }
    if (y + 1 < MAP_Y && !height[y + 1][x]) {
      height[y + 1][x] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x;
      tail->y = y + 1;
    }
    if (x + 1 < MAP_X && y - 1 >= 0 && !height[y - 1][x + 1]) {
      height[y - 1][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y - 1;
    }
    if (x + 1 < MAP_X && !height[y][x + 1]) {
      height[y][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y;
    }
    if (x + 1 < MAP_X && y + 1 < MAP_Y && !height[y + 1][x + 1]) {
      height[y + 1][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y + 1;
    }

    tmp = head;
    head = head->next;
    free(tmp);
  }

  /* And smooth it a bit with a gaussian convolution */
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      for (s = t = p = 0; p < 5; p++) {
        for (q = 0; q < 5; q++) {
          if (y + (p - 2) >= 0 && y + (p - 2) < MAP_Y &&
              x + (q - 2) >= 0 && x + (q - 2) < MAP_X) {
            s += gaussian[p][q];
            t += height[y + (p - 2)][x + (q - 2)] * gaussian[p][q];
          }
        }
      }
      m->height[y][x] = t / s;
    }
  }
  /* Let's do it again, until it's smooth like Kenny G. */
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      for (s = t = p = 0; p < 5; p++) {
        for (q = 0; q < 5; q++) {
          if (y + (p - 2) >= 0 && y + (p - 2) < MAP_Y &&
              x + (q - 2) >= 0 && x + (q - 2) < MAP_X) {
            s += gaussian[p][q];
            t += height[y + (p - 2)][x + (q - 2)] * gaussian[p][q];
          }
        }
      }
      m->height[y][x] = t / s;
    }
  }

  
  out = fopen("diffused.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&height, sizeof (height), 1, out);
  fclose(out);

  out = fopen("smoothed.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->height, sizeof (m->height), 1, out);
  fclose(out);
  

  return 0;
}

static void find_building_location(map_t *m, pair_t p)
{
  do {
    p[dim_x] = rand() % (MAP_X - 3) + 1;
    p[dim_y] = rand() % (MAP_Y - 3) + 1;

    if ((((mapxy(p[dim_x] - 1, p[dim_y]    ) == ter_path)     &&
          (mapxy(p[dim_x] - 1, p[dim_y] + 1) == ter_path))    ||
         ((mapxy(p[dim_x] + 2, p[dim_y]    ) == ter_path)     &&
          (mapxy(p[dim_x] + 2, p[dim_y] + 1) == ter_path))    ||
         ((mapxy(p[dim_x]    , p[dim_y] - 1) == ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] - 1) == ter_path))    ||
         ((mapxy(p[dim_x]    , p[dim_y] + 2) == ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 2) == ter_path)))   &&
        (((mapxy(p[dim_x]    , p[dim_y]    ) != ter_mart)     &&
          (mapxy(p[dim_x]    , p[dim_y]    ) != ter_center)   &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_mart)     &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_center)   &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_mart)     &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_center)   &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_mart)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_center))) &&
        (((mapxy(p[dim_x]    , p[dim_y]    ) != ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_path)     &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_path)))) {
          break;
    }
  } while (1);
}

static int place_pokemart(map_t *m)
{
  pair_t p;

  find_building_location(m, p);

  mapxy(p[dim_x]    , p[dim_y]    ) = ter_mart;
  mapxy(p[dim_x] + 1, p[dim_y]    ) = ter_mart;
  mapxy(p[dim_x]    , p[dim_y] + 1) = ter_mart;
  mapxy(p[dim_x] + 1, p[dim_y] + 1) = ter_mart;

  return 0;
}

static int place_center(map_t *m)
{  pair_t p;

  find_building_location(m, p);

  mapxy(p[dim_x]    , p[dim_y]    ) = ter_center;
  mapxy(p[dim_x] + 1, p[dim_y]    ) = ter_center;
  mapxy(p[dim_x]    , p[dim_y] + 1) = ter_center;
  mapxy(p[dim_x] + 1, p[dim_y] + 1) = ter_center;

  return 0;
}

/* Chooses tree or boulder for border cell.  Choice is biased by dominance *
 * of neighboring cells.                                                   */
static terrain_type_t border_type(map_t *m, int32_t x, int32_t y)
{
  int32_t p, q;
  int32_t r, t;
  int32_t miny, minx, maxy, maxx;
  
  r = t = 0;
  
  miny = y - 1 >= 0 ? y - 1 : 0;
  maxy = y + 1 <= MAP_Y ? y + 1: MAP_Y;
  minx = x - 1 >= 0 ? x - 1 : 0;
  maxx = x + 1 <= MAP_X ? x + 1: MAP_X;

  for (q = miny; q < maxy; q++) {
    for (p = minx; p < maxx; p++) {
      if (q != y || p != x) {
        if (m->map[q][p] == ter_mountain ||
            m->map[q][p] == ter_boulder) {
          r++;
        } else if (m->map[q][p] == ter_forest ||
                   m->map[q][p] == ter_tree) {
          t++;
        }
      }
    }
  }
  
  if (t == r) {
    return rand() & 1 ? ter_boulder : ter_tree;
  } else if (t > r) {
    if (rand() % 10) {
      return ter_tree;
    } else {
      return ter_boulder;
    }
  } else {
    if (rand() % 10) {
      return ter_boulder;
    } else {
      return ter_tree;
    }
  }
}

static int map_terrain(map_t *m, int8_t n, int8_t s, int8_t e, int8_t w)
{
  int32_t i, x, y;
  queue_node_t *head, *tail, *tmp;
  //  FILE *out;
  int num_grass, num_clearing, num_mountain, num_forest, num_water, num_total;
  terrain_type_t type;
  int added_current = 0;
  
  num_grass = rand() % 4 + 2;
  num_clearing = rand() % 4 + 2;
  num_mountain = rand() % 2 + 1;
  num_forest = rand() % 2 + 1;
  num_water = rand() % 2 + 1;
  num_total = num_grass + num_clearing + num_mountain + num_forest + num_water;

  memset(&m->map, 0, sizeof (m->map));

  /* Seed with some values */
  for (i = 0; i < num_total; i++) {
    do {
      x = rand() % MAP_X;
      y = rand() % MAP_Y;
    } while (m->map[y][x]);
    if (i == 0) {
      type = ter_grass;
    } else if (i == num_grass) {
      type = ter_clearing;
    } else if (i == num_grass + num_clearing) {
      type = ter_mountain;
    } else if (i == num_grass + num_clearing + num_mountain) {
      type = ter_forest;
    } else if (i == num_grass + num_clearing + num_mountain + num_forest) {
      type = ter_water;
    }
    m->map[y][x] = type;
    if (i == 0) {
      head = tail = malloc(sizeof (*tail));
    } else {
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
    }
    tail->next = NULL;
    tail->x = x;
    tail->y = y;
  }

  /*
  out = fopen("seeded.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->map, sizeof (m->map), 1, out);
  fclose(out);
  */

  /* Diffuse the vaules to fill the space */
  while (head) {
    x = head->x;
    y = head->y;
    i = m->map[y][x];
    
    if (x - 1 >= 0 && !m->map[y][x - 1]) {
      if ((rand() % 100) < 80) {
        m->map[y][x - 1] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x - 1;
        tail->y = y;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (y - 1 >= 0 && !m->map[y - 1][x]) {
      if ((rand() % 100) < 20) {
        m->map[y - 1][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y - 1;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (y + 1 < MAP_Y && !m->map[y + 1][x]) {
      if ((rand() % 100) < 20) {
        m->map[y + 1][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y + 1;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (x + 1 < MAP_X && !m->map[y][x + 1]) {
      if ((rand() % 100) < 80) {
        m->map[y][x + 1] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x + 1;
        tail->y = y;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    added_current = 0;
    tmp = head;
    head = head->next;
    free(tmp);
  }

  /*
  out = fopen("diffused.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->map, sizeof (m->map), 1, out);
  fclose(out);
  */
  
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if (y == 0 || y == MAP_Y - 1 ||
          x == 0 || x == MAP_X - 1) {
        mapxy(x, y) = border_type(m, x, y);
      }
    }
  }

  m->n = n;
  m->s = s;
  m->e = e;
  m->w = w;

  if (n != -1) {
    mapxy(n,         0        ) = ter_gate;
    mapxy(n,         1        ) = ter_gate;
  }
  if (s != -1) {
    mapxy(s,         MAP_Y - 1) = ter_gate;
    mapxy(s,         MAP_Y - 2) = ter_gate;
  }
  if (w != -1) {
    mapxy(0,         w        ) = ter_gate;
    mapxy(1,         w        ) = ter_gate;
  }
  if (e != -1) {
    mapxy(MAP_X - 1, e        ) = ter_gate;
    mapxy(MAP_X - 2, e        ) = ter_gate;
  }

  return 0;
}

static int place_boulders(map_t *m)
{
  int i;
  int x, y;

  for (i = 0; i < MIN_BOULDERS || rand() % 100 < BOULDER_PROB; i++) {
    y = rand() % (MAP_Y - 2) + 1;
    x = rand() % (MAP_X - 2) + 1;
    if (m->map[y][x] != ter_forest &&
        m->map[y][x] != ter_path   &&
        m->map[y][x] != ter_gate) {
      m->map[y][x] = ter_boulder;
    }
  }

  return 0;
}

static int place_trees(map_t *m)
{
  int i;
  int x, y;
  
  for (i = 0; i < MIN_TREES || rand() % 100 < TREE_PROB; i++) {
    y = rand() % (MAP_Y - 2) + 1;
    x = rand() % (MAP_X - 2) + 1;
    if (m->map[y][x] != ter_mountain &&
        m->map[y][x] != ter_path     &&
        m->map[y][x] != ter_water    &&
        m->map[y][x] != ter_gate) {
      m->map[y][x] = ter_tree;
    }
  }

  return 0;
}

// New map expects cur_idx to refer to the index to be generated.  If that
// map has already been generated then the only thing this does is set
// cur_map.
static int new_map()
{
  int d, p;
  int e, w, n, s;

  if (world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x]]) {
    world.cur_map = world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x]];
    return 0;
  }

  world.cur_map                                             =
    world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x]] =
    malloc(sizeof (*world.cur_map));

  smooth_height(world.cur_map);
  
  if (!world.cur_idx[dim_y]) {
    n = -1;
  } else if (world.world[world.cur_idx[dim_y] - 1][world.cur_idx[dim_x]]) {
    n = world.world[world.cur_idx[dim_y] - 1][world.cur_idx[dim_x]]->s;
  } else {
    n = 1 + rand() % (MAP_X - 2);
  }
  if (world.cur_idx[dim_y] == WORLD_SIZE - 1) {
    s = -1;
  } else if (world.world[world.cur_idx[dim_y] + 1][world.cur_idx[dim_x]]) {
    s = world.world[world.cur_idx[dim_y] + 1][world.cur_idx[dim_x]]->n;
  } else  {
    s = 1 + rand() % (MAP_X - 2);
  }
  if (!world.cur_idx[dim_x]) {
    w = -1;
  } else if (world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x] - 1]) {
    w = world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x] - 1]->e;
  } else {
    w = 1 + rand() % (MAP_Y - 2);
  }
  if (world.cur_idx[dim_x] == WORLD_SIZE - 1) {
    e = -1;
  } else if (world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x] + 1]) {
    e = world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x] + 1]->w;
  } else {
    e = 1 + rand() % (MAP_Y - 2);
  }
  
  map_terrain(world.cur_map, n, s, e, w);
     
  place_boulders(world.cur_map);
  place_trees(world.cur_map);
  build_paths(world.cur_map);
  d = (abs(world.cur_idx[dim_x] - (WORLD_SIZE / 2)) +
       abs(world.cur_idx[dim_y] - (WORLD_SIZE / 2)));
  p = d > 200 ? 5 : (50 - ((45 * d) / 200));
  //  printf("d=%d, p=%d\n", d, p);
  if ((rand() % 100) < p || !d) {
    place_pokemart(world.cur_map);
  }
  if ((rand() % 100) < p || !d) {
    place_center(world.cur_map);
  }

  return 0;
}

static void print_map()
{
  int x, y;
  int default_reached = 0;

  printf("\n\n\n\n");

  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      switch (world.cur_map->map[y][x]) {
      case ter_boulder:
      case ter_mountain:
        putchar('%');
        break;
      case ter_tree:
      case ter_forest:
        putchar('^');
        break;
      case ter_path:
      case ter_gate:
        putchar('#');
        break;
      case ter_mart:
        putchar('M');
        break;
      case ter_center:
        putchar('C');
        break;
      case ter_grass:
        putchar(':');
        break;
      case ter_clearing:
        putchar('.');
        break;
      case ter_water:
        putchar('~');
        break;
      case ter_pc:
        putchar('@');
        break;
      default:
        default_reached = 1;
        break;
      }
    }
    putchar('\n');
  }

  if (default_reached) {
    fprintf(stderr, "Default reached in %s\n", __FUNCTION__);
  }
}

// The world is global because of its size, so init_world is parameterless
void init_world()
{
  world.cur_idx[dim_x] = world.cur_idx[dim_y] = WORLD_SIZE / 2;
  //init seq num for player movement
  world.seq_num = 0;
  new_map();
}

void delete_world()
{
  int x, y;

  for (y = 0; y < WORLD_SIZE; y++) {
    for (x = 0; x < WORLD_SIZE; x++) {
      if (world.world[y][x]) {
        free(world.world[y][x]);
        world.world[y][x] = NULL;
      }
    }
  }
}

int main(int argc, char *argv[])
{
  struct timeval tv;
  uint32_t seed;

  if (argc == 2) {
    seed = atoi(argv[1]);
  } else {
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Using seed: %u\n", seed);
  srand(seed);

  init_world();

  pc_t pc;
  pc = place_pc(world.cur_map);

  //error checking (not necissary but why not)
  if((pc.coordinates[dim_y] == -1) || (pc.coordinates[dim_x] == -1)) {

    fprintf(stderr, "Failed to place pc ?\n");
    return -1;
  }
  
  print_map();

  printf("Rival Distance Map: \n");
  dijkstra_rival_path(&world, pc.coordinates);
  printf("Hiker Distance Map: \n");
  dijkstra_hiker_path(&world, pc.coordinates);

  int num_trainers = 10;
  if (spawn_trainers(&world, num_trainers) > 0) {
    //it worked
    printf("you have reached the end of spawing\n");
  }
  else {
    printf("something is very wrong\n");
  }
  
 

  //printf("But how are you going to be the very best if you quit?\n");
  
  return 0;
}
