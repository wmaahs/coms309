#ifndef BAG_H
#define BAG_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#define DEFAULT_POKEBALLS 7
#define DEFAULT_POTIONS 8
#define DEFAULT_REVIVES 5

typedef enum item_types{
    common,
    rare,
    ultra
} item_types_t;

typedef struct pokeball{
    item_types_t type;
    int boost;
} pokeball_t;

typedef struct revive{
    item_types_t type;
    int heal;
} revive_t;

typedef struct potion{
    item_types_t type;
    int heal;
} potion_t;

class Bag {
private:
    std::vector<pokeball_t> pokeballs;
    std::vector<revive_t> revives;
    std::vector<potion_t> potions;
public:
    Bag();

    std::vector<pokeball_t> get_pokeballs(){return pokeballs;}
    std::vector<revive_t> get_revives(){return revives;}
    std::vector<potion_t> get_potions(){return potions;}
    
    void add_pokeball(pokeball_t pokeball) {pokeballs.push_back(pokeball);}
    void add_revive(revive_t revive) {revives.push_back(revive);}
    void add_potion(potion_t potion) {potions.push_back(potion);}
    pokeball_t use_pokeball(int index);
    revive_t use_revive(int index);
    potion_t use_potion(int index);

    bool open_bag(bool trainer_battle, Pokemon *enemy);
    void open_pokeballs(Pokemon *enemy);
    void open_revives();
    void open_potions();
};

#endif /* BAG_H */
