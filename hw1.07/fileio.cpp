
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <climits>

#include "fileio.h"

csv_file all_csv_files[NUM_FILES];
pokemon poke_arr[NUM_POKEMON];
moves moves_arr[NUM_MOVES];
poke_moves poke_moves_arr[NUM_POKE_MOVES];
experience experience_arr[NUM_EXPERIENCE];
poke_species poke_species_arr[NUM_POKE_SPECIES];
type_names type_names_arr[NUM_TYPE_NAMES];
poke_stats poke_stats_arr[NUM_POKE_STATS];
stats stats_arr[NUM_STATS];
poke_types poke_types_arr[NUM_POKE_TYPES];


void parse_pokemon(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    /* id */
    next = line_str.find(',');
    if(next == 0){
        poke_arr[line_ind].id = INT_MAX;
    }
    else{
        poke_arr[line_ind].id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* name */
    next = line_str.find(',');
    if(next == 0){
        poke_arr[line_ind].name = ' ';
    }
    else{
        poke_arr[line_ind].name = line_str.substr(0, next);
    }
    line_str.erase(0, next +1);

    /* height */
    next = line_str.find(',');
    if(next == 0){
        poke_arr[line_ind].h = INT_MAX;
    }
    else{
        poke_arr[line_ind].h = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* weight */
    next = line_str.find(',');
    if(next == 0){
        poke_arr[line_ind].w = INT_MAX;
    }
    else{
        poke_arr[line_ind].w = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* base_xp */
    next = line_str.find(',');
    if(next == 0){
        poke_arr[line_ind].base_xp = INT_MAX;
    }
    else{
        poke_arr[line_ind].base_xp = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* order */
    next = line_str.find(',');
    if(next == 0){
        poke_arr[line_ind].order = INT_MAX;
    }
    else{
        poke_arr[line_ind].order = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* is_default */
    next = std::stoi(line_str);
    if(next == 0){
        poke_arr[line_ind].is_default = false;
    }
    else{
        poke_arr[line_ind].is_default = true;
    }

    return;
}

void parse_moves(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    /* id */
    next = line_str.find(',');
    if(next == 0){
        moves_arr[line_ind].id = INT_MAX;
    }
    else{
        moves_arr[line_ind].id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* name */
    next = line_str.find(',');
    if(next == 0){
        moves_arr[line_ind].name = ' ';
    }
    else{
        moves_arr[line_ind].name = line_str.substr(0, next);
    }
    line_str.erase(0, next +1);

    /* gen_id */
    next = line_str.find(',');
    if(next == 0){
        moves_arr[line_ind].gen_id = INT_MAX;
    }
    else{
        moves_arr[line_ind].gen_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* type_id */
    next = line_str.find(',');
    if(next == 0){
        moves_arr[line_ind].type_id = INT_MAX;
    }
    else{
        moves_arr[line_ind].type_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* power */
    next = line_str.find(',');
    if(next == 0){
        moves_arr[line_ind].power = INT_MAX;
    }
    else{
        moves_arr[line_ind].power = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* pp */
    next = line_str.find(',');
    if(next == 0){
        moves_arr[line_ind].pp = INT_MAX;
    }
    else{
        moves_arr[line_ind].pp = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* accuracy */
    next = std::stoi(line_str);
    if(next == 0){
        moves_arr[line_ind].accuracy = INT_MAX;
    }
    else{
        moves_arr[line_ind].accuracy = std::stoi(line_str.substr(0, next));;
    }

    /* priority */
    next = std::stoi(line_str);
    if(next == 0){
        moves_arr[line_ind].priority = INT_MAX;
    }
    else{
        moves_arr[line_ind].priority = std::stoi(line_str.substr(0, next));
    }

    /* target_id */
    next = std::stoi(line_str);
    if(next == 0){
        moves_arr[line_ind].target_id = INT_MAX;
    }
    else{
        moves_arr[line_ind].target_id = std::stoi(line_str.substr(0, next));
    }

    /* damage_class_id */
    next = std::stoi(line_str);
    if(next == 0){
        moves_arr[line_ind].damage_class_id = INT_MAX;
    }
    else{
        moves_arr[line_ind].damage_class_id = std::stoi(line_str.substr(0, next));
    }

    /* effect_id */
    next = std::stoi(line_str);
    if(next == 0){
        moves_arr[line_ind].effect_id = INT_MAX;
    }
    else{
        moves_arr[line_ind].effect_id = std::stoi(line_str.substr(0, next));
    }

    /* effect_chance */
    next = std::stoi(line_str);
    if(next == 0){
        moves_arr[line_ind].effect_chance = INT_MAX;
    }
    else{
        moves_arr[line_ind].effect_chance = std::stoi(line_str.substr(0, next));
    }

    /* contest_type_id */
    next = std::stoi(line_str);
    if(next == 0){
        moves_arr[line_ind].contest_type_id = INT_MAX;
    }
    else{
        moves_arr[line_ind].contest_type_id = std::stoi(line_str.substr(0, next));
    }

    /* contest_effect_id */
    next = std::stoi(line_str);
    if(next == 0){
        moves_arr[line_ind].contest_effect_id = INT_MAX;
    }
    else{
        moves_arr[line_ind].contest_effect_id = std::stoi(line_str.substr(0, next));
    }

    /* super_contest_effect_id */
    if(line_str.at(0) == '\0')
    {
        moves_arr[line_ind].super_contest_effect_id = INT_MAX;
    }
    else{
        moves_arr[line_ind].super_contest_effect_id = std::stoi(line_str);
    }

    return;

}

void parse_poke_moves(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    /* poke_id */
    next = line_str.find(',');
    if(next == 0){
        poke_moves_arr[line_ind].pokemon_id = INT_MAX;
    }
    else{
        poke_moves_arr[line_ind].pokemon_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* version group id */
    next = line_str.find(',');
    if(next == 0){
        poke_moves_arr[line_ind].version_group_id = ' ';
    }
    else{
        poke_moves_arr[line_ind].version_group_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* move_id */
    next = line_str.find(',');
    if(next == 0){
        poke_moves_arr[line_ind].moves_id = INT_MAX;
    }
    else{
        poke_moves_arr[line_ind].moves_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* pokemon_move_method_id */
    next = line_str.find(',');
    if(next == 0){
        poke_moves_arr[line_ind].pokemon_move_method_id = INT_MAX;
    }
    else{
        poke_moves_arr[line_ind].pokemon_move_method_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* level */
    next = line_str.find(',');
    if(next == 0){
        poke_moves_arr[line_ind].level = INT_MAX;
    }
    else{
        poke_moves_arr[line_ind].level = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* order */
    if(line_str.at(0) == '\0')
    {
        poke_moves_arr[line_ind].order = INT_MAX;
    }
    else{
        poke_moves_arr[line_ind].order = std::stoi(line_str);
    }

    return;
}

void parse_experience(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    /* growth_rate_id */
    next = line_str.find(',');
    if(next == 0){
        experience_arr[line_ind].growth_rate_id = INT_MAX;
    }
    else{
        experience_arr[line_ind].growth_rate_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* level */
    next = line_str.find(',');
    if(next == 0){
        experience_arr[line_ind].level = ' ';
    }
    else{
        experience_arr[line_ind].level = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* experience */
    if(line_str.at(0) == '\0')
    {
        experience_arr[line_ind].xp = INT_MAX;
    }
    else{
        experience_arr[line_ind].xp = std::stoi(line_str);
    }

    return;
}

void parse_poke_species(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    /* id */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].id = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* name */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].name = ' ';
    }
    else{
        poke_species_arr[line_ind].name = line_str.substr(0, next);
    }
    line_str.erase(0, next +1);

    /* gen_id */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].gen_id = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].gen_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* evolves_from_species_id */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].evolves_from_species_id = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].evolves_from_species_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* evolution_chain_id */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].evolution_chain_id = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].evolution_chain_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* color_id */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].color_id = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].color_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* shape_id */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].shape_id = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].shape_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* habitat_id */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].habitat_id = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].habitat_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* gender_rate */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].gender_rate = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].gender_rate = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* capture_rate */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].capture_rate = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].capture_rate = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* base_happiness */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].base_happiness = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].base_happiness = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* is_baby */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].is_baby = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].is_baby = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* hatch_counter */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].hatch_counter = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].hatch_counter = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* has_gender_differences */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].has_gender_differences = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].has_gender_differences = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* growth_rate_id */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].growth_rate_id = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].growth_rate_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* forms_switchable */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].forms_switchable = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].forms_switchable = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* is_legendary */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].is_legendary = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].is_legendary = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* is_mythical */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].is_mythical = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].is_mythical = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* order */
    next = line_str.find(',');
    if(next == 0){
        poke_species_arr[line_ind].order = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].order = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* conquest_order */
    if(line_str.at(0) == '\0')
    {
        poke_species_arr[line_ind].conquest_order = INT_MAX;
    }
    else{
        poke_species_arr[line_ind].conquest_order = std::stoi(line_str);
    }

    return;
}

void parse_type_names(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    next = line_str.find(',');
    if(next == 0)
    {
        type_names_arr[line_ind].type_id = INT_MAX;
    }
    else
    {
        type_names_arr[line_ind].type_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    next = line_str.find(',');
    if(next == 0)
    {
        type_names_arr[line_ind].local_language_id = INT_MAX;
    }
    else{
        type_names_arr[line_ind].local_language_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    if(type_names_arr[line_ind].local_language_id == 9) {
        type_names_arr[line_ind].name = line_str;
    }
    else{
        type_names_arr[line_ind].name = "";
        
    }
    return;
}

void parse_poke_stats(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    /* poke_id */
    next = line_str.find(',');
    if(next == 0){
        poke_stats_arr[line_ind].poke_id = INT_MAX;
    }
    else{
        poke_stats_arr[line_ind].poke_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* stat_id */
    next = line_str.find(',');
    if(next == 0){
        poke_stats_arr[line_ind].stat_id = INT_MAX;
    }
    else{
        poke_stats_arr[line_ind].stat_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* base_stat */
    next = line_str.find(',');
    if(next == 0){
        poke_stats_arr[line_ind].base_stat = INT_MAX;
    }
    else{
        poke_stats_arr[line_ind].base_stat = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* effort */
    poke_stats_arr[line_ind].effort = std::stoi(line_str);

    return;
}

void parse_stats(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    /* id */
    next = line_str.find(',');
    if(next == 0){
        stats_arr[line_ind].id = INT_MAX;
    }
    else{
        stats_arr[line_ind].id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* damage_class_id */
    next = line_str.find(',');
    if(next == 0){
        stats_arr[line_ind].damage_class_id = INT_MAX;
    }
    else{
        stats_arr[line_ind].damage_class_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* name */
    next = line_str.find(',');
    if(next == 0){
        stats_arr[line_ind].name = "";
    }
    else{
        stats_arr[line_ind].name = line_str.substr(0, next);
    }
    line_str.erase(0, next +1);

    /* is_battle_only */
    next = line_str.find(',');
    if(next == 0){
        stats_arr[line_ind].is_battle_only = INT_MAX;
    }
    else{
        stats_arr[line_ind].is_battle_only = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);
    
    /* game_index */
    if(line_str.at(0) == '\0')
    {
        stats_arr[line_ind].game_index = INT_MAX;
    }
    else
    {
        stats_arr[line_ind].game_index = std::stoi(line_str);
    }
    return;
}

void parse_poke_types(char * line, int line_ind)
{
    std::string line_str(line);
    int next;

    /* poke_id */
    next = line_str.find(',');
    if(next == 0){
        poke_types_arr[line_ind].poke_id = INT_MAX;
    }
    else{
        poke_types_arr[line_ind].poke_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);

    /* type_id */
    next = line_str.find(',');
    if(next == 0){
        poke_types_arr[line_ind].type_id = INT_MAX;
    }
    else{
        poke_types_arr[line_ind].type_id = std::stoi(line_str.substr(0, next));
    }
    line_str.erase(0, next +1);
    
    /* slot */
    if(line_str.at(0) == '\0')
    {
        poke_types_arr[line_ind].slot = INT_MAX;
    }
    else
    {
        poke_types_arr[line_ind].slot = std::stoi(line_str);
    }
    return;
}
/**
 * 
 * Looks for the database in two different locations
 * if it it finds the database, it then calls the
 * parsing funciton for that specific file
 * if it fails to find the data base, returns 0
*/
void find_in_database(std::string filename)
{
    std::fstream cur_csv;

    int i;
    //buffer to hold the current line being read from the file
    char * buf;
    buf = (char *) malloc(sizeof(char) * 800);
    //first try to open from the /share/cs327/
    cur_csv.open("/share/cs327/pokedex/pokedex/data/csv/" + filename + ".csv");

    std::cout << "/share/cs327/pokedex/pokedex/data/csv/" + filename + ".csv" << std::endl;
    //found in /share/cs327/
    std::cout << std::to_string(cur_csv.is_open()) << std::endl;
    if(cur_csv.is_open()) {

        std::cout << "Found csv in /shared/..." << std::endl;
        //pokemon.csv
        if(filename == "pokemon") {
            
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKEMON; i++) {
                cur_csv.getline(buf, 800);
                parse_pokemon(buf, i);
            }
        }
        //moves.csv
        else if(filename == "moves")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_MOVES; i++) {
                cur_csv.getline(buf, 800);
                parse_moves(buf, i);
            }
        }
        //pokemon_moves.csv
        else if(filename == "pokemon_moves")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKE_MOVES; i++) {
                cur_csv.getline(buf, 800);
                parse_poke_moves(buf, i);
            }
        }
        //experience.csv
        else if(filename == "experience")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_EXPERIENCE; i++) {
                cur_csv.getline(buf, 800);
                parse_experience(buf, i);
            }
        }
        //pokemon_species.csv
        else if(filename == "pokemon_species")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKE_SPECIES; i++) {
                cur_csv.getline(buf, 800);
                parse_poke_species(buf, i);
            }
        }
        //type_names.csv
        else if(filename == "type_names")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_TYPE_NAMES; i++) {
                cur_csv.getline(buf, 800);
                parse_type_names(buf, i);
            }
        }
        //pokemon_stats.csv
        else if(filename == "pokemon_stats")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKE_STATS; i++) {
                cur_csv.getline(buf, 800);
                parse_poke_stats(buf, i);
            }
        }
        //stats.csv
        else if(filename == "stats")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_STATS; i++) {
                cur_csv.getline(buf, 800);
                parse_stats(buf, i);
            }
        }
        //pokemon_types.csv
        else if(filename == "pokemon_types")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKE_TYPES; i++) {
                cur_csv.getline(buf, 800);
                parse_poke_types(buf, i);
            }
        }
        print_parsed_csv(filename);
        return;
    }

    std::cout << "Failed to find csv in /shared/..." << std::endl;
    //next try to open from the $HOME/.poke
    char * home_env;
    home_env = getenv("HOME");
    cur_csv.open(strcat(home_env, "/.poke327/pokedex/pokedex/data/csv/") + filename + ".csv");
    if(cur_csv.is_open()){

        std::cout << "Found csv in /HOME/..." << std::endl;
        //pokemon.csv
        if(filename == "pokemon") {
            
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKEMON; i++) {
                cur_csv.getline(buf, 800);
                parse_pokemon(buf, i);
            }
        }
        //moves.csv
        else if(filename == "moves")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_MOVES; i++) {
                cur_csv.getline(buf, 800);
                parse_moves(buf, i);
            }
        }
        //pokemon_moves.csv
        else if(filename == "pokemon_moves")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKE_MOVES; i++) {
                cur_csv.getline(buf, 800);
                parse_poke_moves(buf, i);
            }
        }
        //experience.csv
        else if(filename == "experience")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_EXPERIENCE; i++) {
                cur_csv.getline(buf, 800);
                parse_experience(buf, i);
            }
        }
        //pokemon_species.csv
        else if(filename == "pokemon_species")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKE_SPECIES; i++) {
                cur_csv.getline(buf, 800);
                parse_poke_species(buf, i);
            }
        }
        //type_names.csv
        else if(filename == "type_names")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_TYPE_NAMES; i++) {
                cur_csv.getline(buf, 800);
                parse_type_names(buf, i);
            }
        }
        //pokemon_stats.csv
        else if(filename == "pokemon_stats")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKE_STATS; i++) {
                cur_csv.getline(buf, 800);
                parse_poke_stats(buf, i);
            }
        }
        //stats.csv
        else if(filename == "stats")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_STATS; i++) {
                cur_csv.getline(buf, 800);
                parse_stats(buf, i);
            }
        }
        //pokemon_types.csv
        else if(filename == "pokemon_types")
        {
            cur_csv.getline(buf, 800); //header

            for(i = 0; i < NUM_POKE_TYPES; i++) {
                cur_csv.getline(buf, 800);
                parse_poke_types(buf, i);
            }
        }

        print_parsed_csv(filename);
        return;
    }
    
    std::cout << "File not found" << std::endl;
    free(buf);
    return;
}

/**
 * Function used while printing that converts ints into strings.
 * If int is INT_MAX, then the string returned is ' '.
 * If int is not INT_MAX then it returns a string of that int.
*/
std::string int_to_string(int i) {
    std::string ret_str;
    if(i == INT_MAX) {
        ret_str.clear();
        ret_str.at(0) = ' ';
    }
    else
    {
        ret_str = std::to_string(i);
    }

    return ret_str;
}

void print_parsed_csv(std::string filename)
{
    int i;
    std::cout << "Printing csv" << std::endl;
        //pokemon.csv
        if(filename == "pokemon") {
            
            for(i = 0; i < NUM_POKEMON; i++) {
                std::cout << int_to_string(poke_arr[i].id) + ", " + poke_arr[i].name + ", " + int_to_string(poke_arr[i].species_id) + ", " +
                int_to_string(poke_arr[i].h) + ", " + int_to_string(poke_arr[i].w) + ", " + int_to_string(poke_arr[i].base_xp) + ", " +
                int_to_string(poke_arr[i].order) + ", " + int_to_string(poke_arr[i].is_default) << std::endl;
            }
        }
        //moves.csv
        else if(filename == "moves")
        {
            for(i = 0; i < NUM_MOVES; i++) {
                std::cout << int_to_string(moves_arr[i].id) + ", " + moves_arr[i].name + ", " + int_to_string(moves_arr[i].gen_id) + ", " +
                int_to_string(moves_arr[i].type_id) + ", " + int_to_string(moves_arr[i].power) + ", " + int_to_string(moves_arr[i].pp) + ", " +
                int_to_string(moves_arr[i].accuracy) + int_to_string(moves_arr[i].priority) + int_to_string(moves_arr[i].target_id) +
                int_to_string(moves_arr[i].damage_class_id) + ", " + int_to_string(moves_arr[i].effect_id) + ", " + int_to_string(moves_arr[i].effect_chance) + ", " +
                int_to_string(moves_arr[i].contest_type_id) + ", " + int_to_string(moves_arr[i].contest_effect_id) + ", " + int_to_string(moves_arr[i].super_contest_effect_id) << std::endl;
            }
        }
        //pokemon_moves.csv
        else if(filename == "pokemon_moves")
        {
            for(i = 0; i < NUM_POKE_MOVES; i++) {
                std::cout << int_to_string(poke_moves_arr[i].pokemon_id) + ", " + int_to_string(poke_moves_arr[i].version_group_id) + ", " +
                int_to_string(poke_moves_arr[i].moves_id) + ", " + int_to_string(poke_moves_arr[i].pokemon_move_method_id) + ", " + 
                int_to_string(poke_moves_arr[i].level) + ", " + int_to_string(poke_moves_arr[i].order) << std::endl;
            }
        }
        //experience.csv
        else if(filename == "experience")
        {
            for(i = 0; i < NUM_EXPERIENCE; i++) {
                std::cout << int_to_string(experience_arr[i].growth_rate_id) + ", " + int_to_string(experience_arr[i].level) + ", " +
                int_to_string(experience_arr[i].xp) + ", " << std::endl;
            }
        }
        //pokemon_species.csv
        else if(filename == "pokemon_species")
        {
            for(i = 0; i < NUM_POKE_SPECIES; i++) {
                std::cout << int_to_string(poke_species_arr[i].id) + ", " + poke_species_arr[i].name + ", " + int_to_string(poke_species_arr[i].gen_id) + ", " +
                int_to_string(poke_species_arr[i].evolves_from_species_id) + ", " + int_to_string(poke_species_arr[i].evolution_chain_id) + ", " +
                int_to_string(poke_species_arr[i].color_id) + ", " + int_to_string(poke_species_arr[i].shape_id) + ", " + ", " + int_to_string(poke_species_arr[i].habitat_id) + ", " +
                int_to_string(poke_species_arr[i].gender_rate) + ", " + int_to_string(poke_species_arr[i].capture_rate) + ", " + int_to_string(poke_species_arr[i].base_happiness) + ", " +
                int_to_string(poke_species_arr[i].is_baby) + ", " + int_to_string(poke_species_arr[i].hatch_counter) + ", " + int_to_string(poke_species_arr[i].has_gender_differences) + ", " +
                int_to_string(poke_species_arr[i].growth_rate_id) + ", " + int_to_string(poke_species_arr[i].forms_switchable) + ", " + int_to_string(poke_species_arr[i].is_legendary) + ", " +
                int_to_string(poke_species_arr[i].is_mythical) + ", " + int_to_string(poke_species_arr[i].order) + ", " + int_to_string(poke_species_arr[i].conquest_order) << std::endl;
            }
        }
        //type_names.csv
        else if(filename == "type_names")
        {
            for(i = 0; i < NUM_TYPE_NAMES; i++) {
                std::cout << int_to_string(type_names_arr[i].type_id) + ", " + int_to_string(type_names_arr[i].local_language_id) + ", " +
                type_names_arr[i].name + ", " << std::endl;
            }
        }
        //pokemon_stats.csv
        else if(filename == "pokemon_stats")
        {
            for(i = 0; i < NUM_POKE_STATS; i++) {
                std::cout << int_to_string(poke_stats_arr[i].poke_id) + ", " + int_to_string(poke_stats_arr[i].stat_id) + ", " +
                int_to_string(poke_stats_arr[i].base_stat) + ", " + int_to_string(poke_stats_arr[i].effort) << std::endl;
            }
        }
        //stats.csv
        else if(filename == "stats")
        {
            for(i = 0; i < NUM_STATS; i++) {
                std::cout << int_to_string(stats_arr[i].id) + ", " + int_to_string(stats_arr[i].damage_class_id) + ", " +
                stats_arr[i].name + ", " + int_to_string(stats_arr[i].is_battle_only) + ", " +
                int_to_string(stats_arr[i].game_index) << std::endl;
            }
        }
        //pokemon_types.csv
        else if(filename == "pokemon_types")
        {
            for(i = 0; i < NUM_POKE_TYPES; i++) {
                std::cout << int_to_string(poke_types_arr[i].poke_id) + ", " + int_to_string(poke_types_arr[i].type_id) + ", " +
                int_to_string(poke_types_arr[i].slot) << std::endl;
            }
        }

        return;
}


