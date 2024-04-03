#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include "fileio.h"

/**
 * 
 * Looks for the database in two different locations
 * if it it finds the database, it then returns the specific file
 * if it fails to find the data base, returns 0
*/
std::fstream *find_in_database(std::string filename)
{
    std::fstream cur_csv;
    //first try to open from the /share/cs327/
    cur_csv.open("/share/cs327/pokedex/pokedex/data/csv/" + filename + ".csv");

    //found in /share/cs327/
    if(cur_csv.is_open()) {

        


    }
    return &cur_csv;
}

void parse_print_csv(std::string filename)
{
    std::fstream cur_csv;
    //try to find the file
    cur_csv = find_in_database(filename);


}


