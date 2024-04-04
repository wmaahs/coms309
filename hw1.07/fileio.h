#ifndef FILEIO_H
#define FILEIO_H

#define NUM_FILES 9

#define NUM_POKEMON 1092
#define NUM_MOVES 10018
#define NUM_POKE_MOVES 528239
#define NUM_POKE_SPECIES 898
#define NUM_EXPERIENCE 601
#define NUM_TYPE_NAMES 193
#define NUM_POKE_STATS 6552
#define NUM_STATS 8
#define NUM_POKE_TYPES 1675


class csv_file {
public:
    std::string filename;
};

class pokemon : public csv_file {
public:
    int id;
    std::string name;
    int species_id;
    int h;
    int w;
    int base_xp;
    int order;
    bool is_default;
};

class moves : public csv_file {
public:
    int id;
    std::string name;
    int gen_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;
};
class poke_moves : public csv_file {
public:
    int pokemon_id;
    int version_group_id;
    int moves_id;
    int pokemon_move_method_id;
    int level;
    int order;
};
class poke_species : public csv_file {
public:
    int id;
    std::string name;
    int gen_id;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is_baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;
};
class experience : public csv_file {
public:
    int growth_rate_id;
    int level;
    int xp;
};
class type_names : public csv_file {
public:
    int type_id;
    int local_language_id;
    std::string name;
};
class poke_stats : public csv_file {
public:
    int poke_id;
    int stat_id;
    int base_stat;
    int effort;
};
class stats : public csv_file {
public:
    int id;
    int damage_class_id;
    std::string name;
    int is_battle_only;
    int game_index;
};
class poke_types : public csv_file {
public:
    int poke_id;
    int type_id;
    int slot;
};
#endif