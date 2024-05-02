#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#include "io.h"
#include "character.h"
#include "pokemon.h"
#include "poke327.h"
#include "battle.h"

#define TRAINER_LIST_FIELD_WIDTH 46

typedef struct io_message {
  /* Will print " --more-- " at end of line when another message follows. *
   * Leave 10 extra spaces for that.                                      */
  char msg[71];
  struct io_message *next;
} io_message_t;

static io_message_t *io_head, *io_tail;

void io_init_terminal(void)
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void io_reset_terminal(void)
{
  endwin();

  while (io_head) {
    io_tail = io_head;
    io_head = io_head->next;
    free(io_tail);
  }
  io_tail = NULL;
}

void io_queue_message(const char *format, ...)
{
  io_message_t *tmp;
  va_list ap;

  if (!(tmp = (io_message_t *) malloc(sizeof (*tmp)))) {
    perror("malloc");
    exit(1);
  }

  tmp->next = NULL;

  va_start(ap, format);

  vsnprintf(tmp->msg, sizeof (tmp->msg), format, ap);

  va_end(ap);

  if (!io_head) {
    io_head = io_tail = tmp;
  } else {
    io_tail->next = tmp;
    io_tail = tmp;
  }
}

static void io_print_message_queue(uint32_t y, uint32_t x)
{
  while (io_head) {
    io_tail = io_head;
    attron(COLOR_PAIR(COLOR_CYAN));
    mvprintw(y, x, "%-80s", io_head->msg);
    attroff(COLOR_PAIR(COLOR_CYAN));
    io_head = io_head->next;
    if (io_head) {
      attron(COLOR_PAIR(COLOR_CYAN));
      mvprintw(y, x + 70, "%10s", " --more-- ");
      attroff(COLOR_PAIR(COLOR_CYAN));
      refresh();
      getch();
    }
    free(io_tail);
  }
  io_tail = NULL;
}

/**************************************************************************
 * Compares trainer distances from the PC according to the rival distance *
 * map.  This gives the approximate distance that the PC must travel to   *
 * get to the trainer (doesn't account for crossing buildings).  This is  *
 * not the distance from the NPC to the PC unless the NPC is a rival.     *
 *                                                                        *
 * Not a bug.                                                             *
 **************************************************************************/
static int compare_trainer_distance(const void *v1, const void *v2)
{
  const character *const *c1 = (const character * const *) v1;
  const character *const *c2 = (const character * const *) v2;

  return (world.rival_dist[(*c1)->pos[dim_y]][(*c1)->pos[dim_x]] -
          world.rival_dist[(*c2)->pos[dim_y]][(*c2)->pos[dim_x]]);
}

static character *io_nearest_visible_trainer()
{
  character **c, *n;
  uint32_t x, y, count;

  c = (character **) malloc(world.cur_map->num_trainers * sizeof (*c));

  /* Get a linear list of trainers */
  for (count = 0, y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      if (world.cur_map->cmap[y][x] && world.cur_map->cmap[y][x] !=
          &world.pc) {
        c[count++] = world.cur_map->cmap[y][x];
      }
    }
  }

  /* Sort it by distance from PC */
  qsort(c, count, sizeof (*c), compare_trainer_distance);

  n = c[0];

  free(c);

  return n;
}

void io_display()
{
  uint32_t y, x;
  character *c;

  clear();
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if (world.cur_map->cmap[y][x]) {
        mvaddch(y + 1, x, world.cur_map->cmap[y][x]->symbol);
      } else {
        switch (world.cur_map->map[y][x]) {
        case ter_boulder:
          attron(COLOR_PAIR(COLOR_MAGENTA));
          mvaddch(y + 1, x, BOULDER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_MAGENTA));
          break;
        case ter_mountain:
          attron(COLOR_PAIR(COLOR_MAGENTA));
          mvaddch(y + 1, x, MOUNTAIN_SYMBOL);
          attroff(COLOR_PAIR(COLOR_MAGENTA));
          break;
        case ter_tree:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, TREE_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_forest:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, FOREST_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_path:
        case ter_bailey:
          attron(COLOR_PAIR(COLOR_YELLOW));
          mvaddch(y + 1, x, PATH_SYMBOL);
          attroff(COLOR_PAIR(COLOR_YELLOW));
          break;
        case ter_gate:
          attron(COLOR_PAIR(COLOR_YELLOW));
          mvaddch(y + 1, x, GATE_SYMBOL);
          attroff(COLOR_PAIR(COLOR_YELLOW));
          break;
        case ter_mart:
          attron(COLOR_PAIR(COLOR_BLUE));
          mvaddch(y + 1, x, POKEMART_SYMBOL);
          attroff(COLOR_PAIR(COLOR_BLUE));
          break;
        case ter_center:
          attron(COLOR_PAIR(COLOR_RED));
          mvaddch(y + 1, x, POKEMON_CENTER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_RED));
          break;
        case ter_grass:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, TALL_GRASS_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_clearing:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, SHORT_GRASS_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_water:
          attron(COLOR_PAIR(COLOR_CYAN));
          mvaddch(y + 1, x, WATER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_CYAN));
          break;
        default:
 /* Use zero as an error symbol, since it stands out somewhat, and it's *
  * not otherwise used.                                                 */
          attron(COLOR_PAIR(COLOR_CYAN));
          mvaddch(y + 1, x, ERROR_SYMBOL);
          attroff(COLOR_PAIR(COLOR_CYAN)); 
       }
      }
    }
  }

  mvprintw(23, 1, "PC position is (%2d,%2d) on map %d%cx%d%c.",
           world.pc.pos[dim_x],
           world.pc.pos[dim_y],
           abs(world.cur_idx[dim_x] - (WORLD_SIZE / 2)),
           world.cur_idx[dim_x] - (WORLD_SIZE / 2) >= 0 ? 'E' : 'W',
           abs(world.cur_idx[dim_y] - (WORLD_SIZE / 2)),
           world.cur_idx[dim_y] - (WORLD_SIZE / 2) <= 0 ? 'N' : 'S');
  mvprintw(22, 1, "%d known %s.", world.cur_map->num_trainers,
           world.cur_map->num_trainers > 1 ? "trainers" : "trainer");
  mvprintw(22, 30, "Nearest visible trainer: ");
  if ((c = io_nearest_visible_trainer())) {
    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(22, 55, "%c at vector %d%cx%d%c.",
             c->symbol,
             abs(c->pos[dim_y] - world.pc.pos[dim_y]),
             ((c->pos[dim_y] - world.pc.pos[dim_y]) <= 0 ?
              'N' : 'S'),
             abs(c->pos[dim_x] - world.pc.pos[dim_x]),
             ((c->pos[dim_x] - world.pc.pos[dim_x]) <= 0 ?
              'W' : 'E'));
    attroff(COLOR_PAIR(COLOR_RED));
  } else {
    attron(COLOR_PAIR(COLOR_BLUE));
    mvprintw(22, 55, "NONE.");
    attroff(COLOR_PAIR(COLOR_BLUE));
  }

  io_print_message_queue(0, 0);

  refresh();
}

uint32_t io_teleport_pc(pair_t dest)
{
  /* Just for fun. And debugging.  Mostly debugging. */

  do {
    dest[dim_x] = rand_range(1, MAP_X - 2);
    dest[dim_y] = rand_range(1, MAP_Y - 2);
  } while (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]                  ||
           move_cost[char_pc][world.cur_map->map[dest[dim_y]]
                                                [dest[dim_x]]] ==
             DIJKSTRA_PATH_MAX                                            ||
           world.rival_dist[dest[dim_y]][dest[dim_x]] < 0);

  return 0;
}

static void io_scroll_trainer_list(char (*s)[TRAINER_LIST_FIELD_WIDTH],
                                   uint32_t count)
{
  uint32_t offset;
  uint32_t i;

  offset = 0;

  while (1) {
    for (i = 0; i < 13; i++) {
      mvprintw(i + 6, 19, " %-40s ", s[i + offset]);
    }
    switch (getch()) {
    case KEY_UP:
      if (offset) {
        offset--;
      }
      break;
    case KEY_DOWN:
      if (offset < (count - 13)) {
        offset++;
      }
      break;
    case 27:
      return;
    }

  }
}

static void io_list_trainers_display(npc **c, uint32_t count)
{
  uint32_t i;
  char (*s)[TRAINER_LIST_FIELD_WIDTH]; /* pointer to array of 40 char */

  s = (char (*)[TRAINER_LIST_FIELD_WIDTH]) malloc(count * sizeof (*s));

  mvprintw(3, 19, " %-40s ", "");
  /* Borrow the first element of our array for this string: */
  snprintf(s[0], TRAINER_LIST_FIELD_WIDTH, "You know of %d trainers:", count);
  mvprintw(4, 19, " %-40s ", *s);
  mvprintw(5, 19, " %-40s ", "");

  for (i = 0; i < count; i++) {
    snprintf(s[i], TRAINER_LIST_FIELD_WIDTH, "%16s %c: %2d %s by %2d %s",
             char_type_name[c[i]->ctype],
             c[i]->symbol,
             abs(c[i]->pos[dim_y] - world.pc.pos[dim_y]),
             ((c[i]->pos[dim_y] - world.pc.pos[dim_y]) <= 0 ?
              "North" : "South"),
             abs(c[i]->pos[dim_x] - world.pc.pos[dim_x]),
             ((c[i]->pos[dim_x] - world.pc.pos[dim_x]) <= 0 ?
              "West" : "East"));
    if (count <= 13) {
      /* Handle the non-scrolling case right here. *
       * Scrolling in another function.            */
      mvprintw(i + 6, 19, " %-40s ", s[i]);
    }
  }

  if (count <= 13) {
    mvprintw(count + 6, 19, " %-40s ", "");
    mvprintw(count + 7, 19, " %-40s ", "Hit escape to continue.");
    while (getch() != 27 /* escape */)
      ;
  } else {
    mvprintw(19, 19, " %-40s ", "");
    mvprintw(20, 19, " %-40s ",
             "Arrows to scroll, escape to continue.");
    io_scroll_trainer_list(s, count);
  }

  free(s);
}

static void io_list_trainers()
{
  npc **c;
  uint32_t x, y, count;

  c = (npc **) malloc(world.cur_map->num_trainers * sizeof (*c));

  /* Get a linear list of trainers */
  for (count = 0, y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      if (world.cur_map->cmap[y][x] && world.cur_map->cmap[y][x] !=
          &world.pc) {
        c[count++] = dynamic_cast <npc *> (world.cur_map->cmap[y][x]);
      }
    }
  }

  /* Sort it by distance from PC */
  qsort(c, count, sizeof (*c), compare_trainer_distance);

  /* Display it */
  io_list_trainers_display(c, count);
  free(c);

  /* And redraw the map */
  io_display();
}

void io_pokemart()
{
  int i;
  pokeball_t curr_pokeball;
  revive_t curr_revive;
  potion_t curr_potion;
  mvprintw(0, 0, "Welcome to the Pokemart.  Could I interest you in some Pokeballs?");
  refresh();
  getch();
  //starter balls
  for(i = 0; i < DEFAULT_POKEBALLS; i++) {
      if(i == DEFAULT_POKEBALLS -1){
          curr_pokeball.type = ultra;
          curr_pokeball.boost = 5;
      }
      else if(i == DEFAULT_POKEBALLS -2){
          curr_pokeball.type = rare;
          curr_pokeball.boost = 3;
      }
      else
      {
          curr_pokeball.type = common;
          curr_pokeball.boost = 1;
      }
      world.pc.trainer_bag.add_pokeball(curr_pokeball);
  }
  //starter revives
  for(i = 0; i < DEFAULT_REVIVES; i++){
      if(i == DEFAULT_REVIVES -2)
      {
          curr_revive.type = rare;
          curr_revive.heal = 15;
      }
      else if(i == DEFAULT_REVIVES -1)
      {
          curr_revive.type = ultra;
          curr_revive.heal = 20;
      }
      else
      {
          curr_revive.type = rare;
          curr_revive.heal = 15;
          
      }

          world.pc.trainer_bag.add_revive(curr_revive);
  }
  //starter potions
  for(i = 0; i < DEFAULT_POTIONS; i++){
      if(i == 6 || i == 7){
          curr_potion.type = ultra;
          curr_potion.heal = 20;
      }
      else if(i == 5)
      {
          curr_potion.type = rare;
          curr_potion.heal = 15;
      }
      else
      {
          curr_potion.type = common;
          curr_potion.heal = 10;
      }
          world.pc.trainer_bag.add_potion(curr_potion);
  }
  move(0,0);
  clrtoeol();
  mvprintw(0, 0, "All your supplies have been fully restocked! Have a good Day!");
  refresh();
  getch();
}

void io_pokemon_center()
{
  int i;
  mvprintw(0, 0, "Welcome to the Pokemon Center.  How can Nurse Joy assist you?");
  refresh();
  getch();
  for(i = 0; i < (int) world.pc.roster.size(); i++)
  {
    world.pc.roster[i].set_curr_hp(world.pc.roster[i].get_hp());
  }
  move(0,0);
  clrtoeol();
  mvprintw(0, 0, "All your pokemon have been restored to full health! Have a good Day!");
  refresh();
  getch();
}

void io_battle(character *aggressor, character *defender)
{
  npc *n = (npc *) ((aggressor == &world.pc) ? defender : aggressor);
  int i, key;
  bool battle_over = false;
  Pokemon *active_pokemon = &world.pc.roster[0];
  Pokemon *active_enemy_pokemon = &n->roster[0];
  //placeholder with trainer pokemon data
  clear();
  mvprintw(0, 0, "Hey Kid, Want to see my Pokemon?");
  mvprintw(2, 2, "This Trainer has %ld Pokemon :", n->roster.size());

  for(i = 0; i < (int) n->roster.size(); i++) {
    mvprintw(i*4 + 3, 10, "Pokemon %d: %s (Level %d)", i, n->roster[i].get_name().c_str(), n->roster[i].get_level());
    mvprintw(i*4 + 4, 15, "HP: %d", n->roster[i].get_hp());
    mvprintw(i*4 + 5, 15, "Attack: %d", n->roster[i].get_attack());
    mvprintw(i*4 + 6, 15, "Defense: %d", n->roster[i].get_defense());
  }
  refresh();
  getch();

  //battle
  while(!battle_over)
  {
    clear();
    if(active_enemy_pokemon->get_curr_hp() <= 0)
    {
      for(i = 0; i < (int) n->roster.size(); i++)
      {
        if(n->roster[i].get_curr_hp() > 0)
        {
          active_enemy_pokemon = &n->roster[i];
          break;
        }
        if(i == (int) n->roster.size() - 1)
        {
          //defeated message
          io_display();
          mvprintw(0, 0, "Aww, how'd you get so strong?  You and your pokemon must share a special bond!");
          battle_over = true;
          n->defeated = 1;
          if (n->ctype == char_hiker || n->ctype == char_rival) {
            n->mtype = move_wander;
          }
          refresh();
          getch();
          return;
        }
      }
    }
    if(active_pokemon->get_curr_hp() <= 0)
    {
      for(i = 0; i < (int) world.pc.roster.size(); i++)
      {
        if((world.pc.roster[i].get_curr_hp() > 0))
        {
          break;
        }
        if(i == (int) world.pc.roster.size() -1)
        {
          io_display();
          mvprintw(0, 0, "All your pokemon have fainted, better head to a PokeCenter");
          battle_over = true;
          refresh();
          getch();
          return;
        }
      }
      move(23, 0);
      clrtoeol();
      mvprintw(23, 0, "Your pokemon has fainted; revive it, or select a new one");
      refresh();
      getch();
    }

    mvprintw(0, 1, "%s", active_enemy_pokemon->get_name().c_str());
    mvprintw(1, 6, ":L%d", active_enemy_pokemon->get_level());
    mvprintw(2, 0, "HP:%d/%d", active_enemy_pokemon->get_curr_hp(), active_enemy_pokemon->get_hp());

    mvprintw(10, 35, "%s", active_pokemon->get_name().c_str());
    mvprintw(11, 40, ":L%d", active_pokemon->get_level());
    mvprintw(12, 0, "HP:%d/%d", active_pokemon->get_curr_hp(), active_pokemon->get_hp());


    mvprintw(15, 5, "1. Fight");
    mvprintw(17, 5, "2. Bag");
    mvprintw(19, 5, "3. Run");
    mvprintw(21, 5, "4. Pokemon");
    refresh();
    key = getch();

    switch(key){
      case '1':
        //fight
        battle_fight(active_enemy_pokemon, active_pokemon);
        break;
      case '2':
        //open bag
        battle_over = world.pc.trainer_bag.open_bag(true, active_enemy_pokemon);
        break;
      case '3':
        //attempt to run
        move(23, 0);
        clrtoeol();
        mvprintw(23, 0, "You can't flee from trainer battles ");
        refresh();
        getch();
        break;
      case '4':
        //switch pokemon
        *active_pokemon = select_pokemon();
        enemy_free_attack(active_enemy_pokemon, active_pokemon);
        break;
    }
    
  }
}

uint32_t move_pc_dir(uint32_t input, pair_t dest)
{

  dest[dim_y] = world.pc.pos[dim_y];
  dest[dim_x] = world.pc.pos[dim_x];

  switch (input) {
  case 1:
  case 2:
  case 3:
    dest[dim_y]++;
    break;
  case 4:
  case 5:
  case 6:
    break;
  case 7:
  case 8:
  case 9:
    dest[dim_y]--;
    break;
  }
  switch (input) {
  case 1:
  case 4:
  case 7:
    dest[dim_x]--;
    break;
  case 2:
  case 5:
  case 8:
    break;
  case 3:
  case 6:
  case 9:
    dest[dim_x]++;
    break;
  case '>':
    if (world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]] ==
        ter_mart) {
      io_pokemart();
    }
    if (world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]] ==
        ter_center) {
      io_pokemon_center();
    }
    break;
  }

  if (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]) {
    if (dynamic_cast<npc *> (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]) &&
        ((npc *) world.cur_map->cmap[dest[dim_y]][dest[dim_x]])->defeated) {
      // Some kind of greeting here would be nice
      return 1;
    } else if ((dynamic_cast<npc *>
                (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]))) {
      io_battle(&world.pc, world.cur_map->cmap[dest[dim_y]][dest[dim_x]]);
      // Not actually moving, so set dest back to PC position
      dest[dim_x] = world.pc.pos[dim_x];
      dest[dim_y] = world.pc.pos[dim_y];
    }
  }
  
  if (move_cost[char_pc][world.cur_map->map[dest[dim_y]][dest[dim_x]]] ==
      DIJKSTRA_PATH_MAX) {
    return 1;
  }

  //potentially spawn a pokemon
  gen_pokemon();

  return 0;
}

/**
 * For now is void, eventually will probably return a bool
 * true if pc has won, false otherwise
 * 
 * right now just a place holder for when a pokemon is spawned in the grass
 * 
*/
void io_battle_wild_pokemon(Pokemon *wild_pokemon)
{
  bool battle_over = false;
  int key, i;

  Pokemon *cur_pokemon;
  cur_pokemon = &world.pc.roster[0];
  clear();
  mvprintw(0, 0, "You have encountered a wild %s (Level %d)", wild_pokemon->get_name().c_str(), wild_pokemon->get_level());
  mvprintw(3, 10, "HP: %d", wild_pokemon->get_hp());
  mvprintw(4, 10, "Attack: %d", wild_pokemon->get_attack());
  mvprintw(5, 10, "Defense: %d", wild_pokemon->get_defense());
  mvprintw(6, 10, "Speed: %d", wild_pokemon->get_speed());
  mvprintw(7, 10, "Special Attack: %d", wild_pokemon->get_special_attk());
  mvprintw(8, 10, "Special Defese: %d", wild_pokemon->get_special_def());
  refresh();

  getch();

  //Initial battle screen
  while(!(battle_over)){
    clear();

    mvprintw(0, 1, "%s", wild_pokemon->get_name().c_str());
    mvprintw(1, 6, ":L%d", wild_pokemon->get_level());
    mvprintw(2, 0, "HP:%d/%d", wild_pokemon->get_curr_hp(), wild_pokemon->get_hp());

    mvprintw(10, 35, "%s", cur_pokemon->get_name().c_str());
    mvprintw(11, 40, ":L%d", cur_pokemon->get_level());
    mvprintw(12, 0, "HP:%d/%d", cur_pokemon->get_curr_hp(), cur_pokemon->get_hp());


    mvprintw(15, 5, "1. Fight");
    mvprintw(17, 5, "2. Bag");
    mvprintw(19, 5, "3. Run");
    mvprintw(21, 5, "4. Pokemon");
    refresh();
    key = getch();

    switch(key){
      case '1':
        //fight
        battle_fight(wild_pokemon, cur_pokemon);
        if(wild_pokemon->get_curr_hp() <= 0)
        {
          battle_over = 1;
        }
        if(cur_pokemon->get_curr_hp() <= 0)
        {
          for(i = 0; i < (int) world.pc.roster.size(); i++)
          {
            if(world.pc.roster[i].get_curr_hp() >= 0)
            {
              break;
            }
            if(i == (int) world.pc.roster.size() -1)
            {
              move(23, 0);
              clrtoeol();
              mvprintw(23, 0, "All your pokemon have fainted, better head to a PokeCenter");
              refresh();
              getch();
              return;
            }
          }
          move(23, 0);
          clrtoeol();
          mvprintw(23, 0, "Your pokemon has fainted; revive it, select a new one, or run");
          refresh();
          getch();
        }
        break;
      case '2':
        //open bag
        battle_over = world.pc.trainer_bag.open_bag(false, wild_pokemon);
        break;
      case '3':
        //attempt to run
        move(23, 0);
        clrtoeol();
        mvprintw(23, 0, "You are attempting to flee...");
        refresh();
        getch();
        if(rand() % 100 <= 33)
        {
          move(23, 0);
          clrtoeol();
          mvprintw(23, 0, "You got away");
          refresh();
          getch();
          battle_over = 1;
        }
        /* TODO: enemy gets to attack if failed to flee */
        else
        {
          move(23, 0);
          clrtoeol();
          mvprintw(23, 0, "Your attempt to flee was unsuccesful...");
          refresh();
          getch();
          enemy_free_attack(wild_pokemon, cur_pokemon);
        }
        break;
      case '4':
        //switch pokemon
        *cur_pokemon = select_pokemon();
        enemy_free_attack(wild_pokemon, cur_pokemon);
        break;
    }
    
  }
  return;
}


void io_teleport_world(pair_t dest)
{
  /* mvscanw documentation is unclear about return values.  I believe *
   * that the return value works the same way as scanf, but instead   *
   * of counting on that, we'll initialize x and y to out of bounds   *
   * values and accept their updates only if in range.                */
  int x = INT_MAX, y = INT_MAX;
  
  world.cur_map->cmap[world.pc.pos[dim_y]][world.pc.pos[dim_x]] = NULL;

  echo();
  curs_set(1);
  do {
    mvprintw(0, 0, "Enter x [-200, 200]:           ");
    refresh();
    mvscanw(0, 21, (char *) "%d", &x);
  } while (x < -200 || x > 200);
  do {
    mvprintw(0, 0, "Enter y [-200, 200]:          ");
    refresh();
    mvscanw(0, 21, (char *) "%d", &y);
  } while (y < -200 || y > 200);

  refresh();
  noecho();
  curs_set(0);

  x += 200;
  y += 200;

  world.cur_idx[dim_x] = x;
  world.cur_idx[dim_y] = y;

  new_map(1);
  io_teleport_pc(dest);
}

void io_select_starter()
{

  int user_input;
  int confirmation = 0;
  int poke_selected = 0;
  std::vector<Pokemon> starters;
  int i;
  for(i = 0; i < 3; i++) {
    Pokemon new_poke(pokemon[((rand() % 1092) + 1)]);
    new_poke.levelup();
    starters.push_back(new_poke);
  }

  mvprintw(0, 0, "Welcome to the Poke-Universe! First things first, you need pick your first Pokemon.");
  mvprintw(1, 0, "You have three options to select from. Choose wisely.");

  attron(COLOR_PAIR(COLOR_BLUE));
  mvprintw(3, 10, "Option 1: %s", starters[0].get_name().c_str());
  mvprintw(4, 15, "Base HP: %d", starters[0].get_hp());
  mvprintw(5, 15, "Base Attack: %d", starters[0].get_attack());
  mvprintw(6, 15, "Base Defense: %d", starters[0].get_defense());
  mvprintw(7, 15, "Base Speed: %d", starters[0].get_speed());
  mvprintw(8, 15, "Base Special Attack: %d", starters[0].get_special_attk());
  mvprintw(9, 15, "Base Special Defense: %d", starters[0].get_special_def());
  attroff(COLOR_PAIR(COLOR_BLUE));
  
  attron(COLOR_PAIR(COLOR_MAGENTA));
  mvprintw(10, 10, "Option 2: %s",            starters[1].get_name().c_str());
  mvprintw(11, 15, "Base HP: %d",             starters[1].get_hp());
  mvprintw(12, 15, "Base Attack: %d",         starters[1].get_attack());
  mvprintw(13, 15, "Base Defense: %d",        starters[1].get_defense());
  mvprintw(14, 15, "Base Speed: %d",          starters[1].get_speed());
  mvprintw(15, 15, "Base Special Attack: %d", starters[1].get_special_attk());
  mvprintw(16, 15, "Base Special Defense: %d",starters[1].get_special_def());
  attroff(COLOR_PAIR(COLOR_MAGENTA));

  attron(COLOR_PAIR(COLOR_GREEN));
  mvprintw(17, 10, "Option 3: %s",              starters[2].get_name().c_str());
  mvprintw(18, 15, "Base HP: %d",               starters[2].get_hp());
  mvprintw(19, 15, "Base Attack: %d",           starters[2].get_attack());
  mvprintw(20, 15, "Base Defense: %d",          starters[2].get_defense());
  mvprintw(21, 15, "Base Speed: %d",            starters[2].get_speed());
  mvprintw(22, 15, "Base Special Attack: %d",   starters[2].get_special_attk());
  mvprintw(23, 15, "Base Special Defense: %d",  starters[2].get_special_def());
  attroff(COLOR_PAIR(COLOR_GREEN));

  refresh();

  while(!(poke_selected))
  {
    user_input = getch();
    switch(user_input){
      case '1':
        move(0, 0);
        clrtoeol();
        mvprintw(0, 0, "You have selected %s, are you sure you want to continue (y/n)", starters[0].get_name().c_str());
        move(1, 0);
        clrtoeol();
        refresh();
        confirmation = getch();
        if(confirmation == 'y'){
          world.pc.roster.push_back(starters[0]);
          poke_selected = 1;
        }
        else if(confirmation == 'n') {
          poke_selected = 0;
        }
        else{
          move(0, 0);
          clrtoeol();
          mvprintw(0, 0, "y/n not selected, Cancelling..");
          refresh();
        }
        break;
      case '2':
        move(0, 0);
        clrtoeol();
        mvprintw(0, 0, "You have selected %s, are you sure you want to continue (y/n)", starters[1].get_name().c_str());
        move(1, 0);
        clrtoeol();
        refresh();
        confirmation = getch();
        if(confirmation == 'y'){
          world.pc.roster.push_back(starters[1]);
          poke_selected = 1;
        }
        else if(confirmation == 'n') {
          poke_selected = 0;
        }
        else{
          move(0, 0);
          clrtoeol();
          mvprintw(0, 0, "y/n not selected, Cancelling..");
          refresh();
        }
        break;
      case '3':
        move(0, 0);
        clrtoeol();
        mvprintw(0, 0, "You have selected %s, are you sure you want to continue (y/n)", starters[2].get_name().c_str());
        move(1, 0);
        clrtoeol();
        refresh();
        confirmation = getch();
        if(confirmation == 'y'){
          world.pc.roster.push_back(starters[2]);
          poke_selected = 1;
        }
        else if(confirmation == 'n') {
          poke_selected = 0;
        }
        else{
          move(0, 0);
          clrtoeol();
          mvprintw(0, 0, "y/n not selected, Cancelling..");
          refresh();
        }
        break;
      default:
        move(0, 0);
        clrtoeol();
        mvprintw(0, 0, "Enter 1, 2, or 3 to select a pokemon");
        move(1, 0);
        clrtoeol();
        refresh();
        break;
    }
  }

}

/* Add this function to display the bag (check if bag class works) */
// void io_display_bag(){


// }

void io_handle_input(pair_t dest)
{
  uint32_t turn_not_consumed;
  bool turn_con;
  int key;

  do {
    switch (key = getch()) {
    case '7':
    case 'y':
    case KEY_HOME:
      turn_not_consumed = move_pc_dir(7, dest);
      break;
    case '8':
    case 'k':
    case KEY_UP:
      turn_not_consumed = move_pc_dir(8, dest);
      break;
    case '9':
    case 'u':
    case KEY_PPAGE:
      turn_not_consumed = move_pc_dir(9, dest);
      break;
    case '6':
    case 'l':
    case KEY_RIGHT:
      turn_not_consumed = move_pc_dir(6, dest);
      break;
    case '3':
    case 'n':
    case KEY_NPAGE:
      turn_not_consumed = move_pc_dir(3, dest);
      break;
    case '2':
    case 'j':
    case KEY_DOWN:
      turn_not_consumed = move_pc_dir(2, dest);
      break;
    case '1':
    case 'b':
    case KEY_END:
      turn_not_consumed = move_pc_dir(1, dest);
      break;
    case '4':
    case 'h':
    case KEY_LEFT:
      turn_not_consumed = move_pc_dir(4, dest);
      break;
    case '5':
    case ' ':
    case '.':
    case KEY_B2:
      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      turn_not_consumed = 0;
      break;
    case '>':
      turn_not_consumed = move_pc_dir('>', dest);
      break;
    case 'Q':
      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      world.quit = 1;
      turn_not_consumed = 0;
      break;
      break;
    case 't':
      io_list_trainers();
      turn_not_consumed = 1;
      break;
    case 'p':
      /* Teleport the PC to a random place in the map.              */
      io_teleport_pc(dest);
      turn_not_consumed = 0;
      break;
   case 'f':
      /* Fly to any map in the world.                                */
      io_teleport_world(dest);
      turn_not_consumed = 0;
      break;    
    case 'B':
      /* View the pc's bag */
      turn_con = world.pc.trainer_bag.open_bag(true, &world.pc.roster[0]);
      if(turn_con)
      {

        turn_not_consumed = 1;
      }
      else
      {
        turn_not_consumed = 0;
      }
      break;
    // case 'P':
    //   break;
    case 'q':
      /* Demonstrate use of the message queue.  You can use this for *
       * printf()-style debugging (though gdb is probably a better   *
       * option.  Not that it matters, but using this command will   *
       * waste a turn.  Set turn_not_consumed to 1 and you should be *
       * able to figure out why I did it that way.                   */
      io_queue_message("This is the first message.");
      io_queue_message("Since there are multiple messages, "
                       "you will see \"more\" prompts.");
      io_queue_message("You can use any key to advance through messages.");
      io_queue_message("Normal gameplay will not resume until the queue "
                       "is empty.");
      io_queue_message("Long lines will be truncated, not wrapped.");
      io_queue_message("io_queue_message() is variadic and handles "
                       "all printf() conversion specifiers.");
      io_queue_message("Did you see %s?", "what I did there");
      io_queue_message("When the last message is displayed, there will "
                       "be no \"more\" prompt.");
      io_queue_message("Have fun!  And happy printing!");
      io_queue_message("Oh!  And use 'Q' to quit!");

      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      turn_not_consumed = 0;
      break;
    default:
      /* Also not in the spec.  It's not always easy to figure out what *
       * key code corresponds with a given keystroke.  Print out any    *
       * unhandled key here.  Not only does it give a visual error      *
       * indicator, but it also gives an integer value that can be used *
       * for that key in this (or other) switch statements.  Printed in *
       * octal, with the leading zero, because ncurses.h lists codes in *
       * octal, thus allowing us to do reverse lookups.  If a key has a *
       * name defined in the header, you can use the name here, else    *
       * you can directly use the octal value.                          */
      mvprintw(0, 0, "Unbound key: %#o ", key);
      turn_not_consumed = 1;
    }
    refresh();
  } while (turn_not_consumed);
}
