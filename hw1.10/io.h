#ifndef IO_H
# define IO_H

#include "pokemon.h"

class character;
typedef int16_t pair_t[2];

void io_init_terminal(void);
void io_select_starter();
void io_reset_terminal(void);
void io_display(void);
void io_handle_input(pair_t dest);
void io_queue_message(const char *format, ...);
void io_battle(character *aggressor, character *defender);
void io_battle_wild_pokemon(Pokemon *wild_pokemon);
void save_game();


#endif
