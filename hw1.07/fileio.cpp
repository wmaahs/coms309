#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

#include "fileio.h"
csv_file all_csv_files[NUM_FILES];
pokemon poke_arr[NUM_POMEMON];
moves moves_arr[NUM_MOVES];
poke_moves poke_moves_arr[NUM_POKE_MOVES];
experience experience_arr[NUM_EXPERIENCE];
poke_species poke_species_arr[NUM_POKE_SPECIES];
type_names type_names_arr[NUM_TYPE_NAMES];
poke_stats poke_stats_arr[NUM_POKE_STATS];
stats stats_arr[NUM_STATS];
poke_types poke_types_arr[NUM_POKE_TYPES];

/**
 * 
 * Looks for the database in two different locations
 * if it it finds the database, it then calls the
 * parsing funciton for that specific file
 * if it fails to find the data base, returns 0
*/
int find_in_database(std::string filename)
{
    std::fstream cur_csv;
    //first try to open from the /share/cs327/
    cur_csv.open("/share/cs327/pokedex/pokedex/data/csv/" + filename + ".csv");

    //found in /share/cs327/
    if(cur_csv.is_open()) {

        //pokemon.csv
        if(filename == "pokemon.csv") {

        }
        
        return 1;
    }

    //next try to open from the $HOME/.poke
    char * home_env;
    home_env = getenv("HOME");
    cur_csv.open(strcat(home_env, "/.poke327/pokedex/pokedex/data/csv/") + filename + ".csv");
    if(cur_csv.is_open()){

        return 1;
    }
    
    return 0;
}

// void print_parsed_csv(std::string filename)
// {

// }


