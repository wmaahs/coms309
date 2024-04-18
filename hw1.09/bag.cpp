
#include "poke327.h"
#include "bag.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <ncurses.h>


Bag::Bag()
{
    int i;
    pokeball_t curr_pokeball;
    revive_t curr_revive;
    potion_t curr_potion;

    //starter balls
    for(i = 0; i < DEFAULT_POKEBALLS; i++) {
        if(i == DEFAULT_POKEBALLS -1){
            curr_pokeball.type = ultra;
            curr_pokeball.boost = 5;
        }
        else
        {
            curr_pokeball.type = common;
            curr_pokeball.boost = 1;
        }
        pokeballs.push_back(curr_pokeball);
    }
    //starter revives
    for(i = 0; i < DEFAULT_REVIVES; i++){
        curr_revive.type = common;
        curr_revive.heal = 10;
        revives.push_back(curr_revive);
    }
    //starter potions
    for(i = 0; i < DEFAULT_POTIONS; i++){
        if(i == 5 || i == 6){
            curr_potion.type = ultra;
            curr_potion.heal = 20;
        }
        else
        {
            curr_potion.type = common;
            curr_potion.heal = 10;
        }
            potions.push_back(curr_potion);
    }
}

void Bag::open_bag()
{
    int key;
    bool option_selected = false;
    clear();

    attron(COLOR_PAIR(COLOR_YELLOW));
    mvprintw(0, 37, "BAG");
    attroff(COLOR_PAIR(COLOR_YELLOW));

    attron(COLOR_PAIR(COLOR_CYAN));
    mvprintw(1, 0, "1. Pokeballs (%d)", (int) pokeballs.size());
    attroff(COLOR_PAIR(COLOR_CYAN));

    attron(COLOR_PAIR(COLOR_GREEN));
    mvprintw(3, 0, "2. Revives (%d)", (int) revives.size());
    attroff(COLOR_PAIR(COLOR_GREEN));

    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(5, 0, "3. Potions (%d)", (int) potions.size());
    attroff(COLOR_PAIR(COLOR_RED));

    refresh();

    while(!option_selected)
    {
        key = getch();
        switch (key)
        {
        case '1':
            this->open_pokeballs();
            option_selected = true;
            break;
        case '2':
            this->open_revives();
            option_selected = true;
            break;
        case '3':
            this->open_potions();
            option_selected = true;
            break;
        case '4':
            return;
        default:
            mvprintw(0, 1, "Select 1, 2, 3, or 4");
            refresh();
            break;
        }
    }
}

pokeball_t Bag::use_pokeball(int index)
{
    pokeball_t selected_pokeball = pokeballs.at(index);
    pokeballs.erase(pokeballs.begin()+index);
    return selected_pokeball;
}
revive_t Bag::use_revive(int index)
{
    revive_t selected_revive = revives.at(index);
    revives.erase(revives.begin()+index);
    return selected_revive;
}
potion_t Bag::use_potion(int index)
{
    potion_t selected_potion = potions.at(index);
    potions.erase(potions.begin()+index);
    return selected_potion;
}

void Bag::open_pokeballs()
{
    int i;
    pokeball_t curr_pokeball;
    clear();

    if(pokeballs.empty()){
        mvprintw(0, 1, "You do not have any Pokeballs left!");
        refresh();
        getch();
        return;
    }
    
    attron(COLOR_PAIR(COLOR_CYAN));
    mvprintw(0, 33, "POKEBALLS (%d)",(int) pokeballs.size());
    attroff(COLOR_PAIR(COLOR_CYAN));

    for(i = 0; i < (int) pokeballs.size(); i++)
    {
        curr_pokeball = pokeballs.at(i);
        if(curr_pokeball.type == common)
        {
            mvprintw(i+2, 5, "%d - Common", i);
        }
        if(curr_pokeball.type == rare)
        {
            mvprintw(i+2, 5, "%d - Rare", i);
        }
        if(curr_pokeball.type == ultra)
        {
            mvprintw(i+2, 5, "%d - Ultra", i);
        }
    }
    refresh();
    getch();
    return;
}

void Bag::open_revives()
{
    int i, key;
    revive_t curr_revive;
    revive_t selected_revive;
    bool revive_selected = false;
    clear();
    if(revives.empty()){
        mvprintw(0, 1, "You do not have any Revives left!");
        refresh();
        getch();
        return;
    }

    attron(COLOR_PAIR(COLOR_GREEN));
    mvprintw(0, 33, "REVIVES (%d)",(int) revives.size());
    attroff(COLOR_PAIR(COLOR_GREEN));

    for(i = 0; i < (int) revives.size(); i++)
    {
        curr_revive = revives.at(i);
        if(curr_revive.type == common)
        {
            mvprintw(i, 5, "%d - Common", i);
        }
        else if(curr_revive.type == rare)
        {
            mvprintw(i, 5, "%d - Rare", i);
        }
        else if(curr_revive.type == ultra)
        {
            mvprintw(i, 5, "%d - Ultra", i);
        }
        else
        {
            mvprintw(i, 5, "oops");
        }
    }
    refresh();


    while((key != 27) || (!revive_selected))
    {
        key = getch();
        if(key < (int) potions.size())
        {
            selected_revive = use_revive(key);
            revive_selected = true;
            move(21, 0);
            clrtoeol();
        }
        else{
            mvprintw(21, 0, "use the number keys to select a revive (esc to exit)");
            refresh();
        }
    }
    if(revive_selected)
    {
        mvprintw(22, 0, "You have selected to use revive %d", key);
        if(world.pc.roster.size() == 1){

        }
        else
        {

            mvprintw(23, 0, "Select a pokemon that you would like to use it on: ");
            for(i = 0; i < (int) world.pc.roster.size(); i++){
                mvprintw(3, 35, "%d - %s", i, world.pc.roster[i].get_name().c_str());
            }
            refresh();
            while(!revive_selected)
            {
                key = getch();
                if(key < (int) world.pc.roster.size())
                {
                    world.pc.roster[key].set_curr_hp(world.pc.roster[key].get_curr_hp() + selected_revive.heal);
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You revived %s to %d HP", world.pc.roster[key].get_name().c_str(), world.pc.roster[key].get_curr_hp());
                    move(22, 0);
                    clrtoeol();
                    refresh();
                    revive_selected = true;
                }
                else{
                    mvprintw(21, 0, "Use the number keys to select a pokemon");
                    refresh();
                }
            }
        }
    }
    return;
}
void Bag::open_potions()
{
    int i;
    int key;
    potion_t curr_potion;
    potion_t selected_potion;
    bool potion_selected;
    bool pokemon_selected;
    clear();
    if(potions.empty()){
        mvprintw(0, 1, "You do not have any Potions left!");
        refresh();
        getch();
        return;
    }

    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(0, 33, "POTIONS (%d)",(int) potions.size());
    attroff(COLOR_PAIR(COLOR_RED));

    for(i = 0; i < (int) potions.size(); i++)
    {
        curr_potion = potions.at(i);
        if(curr_potion.type == common)
        {
            attron(COLOR_PAIR(COLOR_GREEN));
            mvprintw(i+1, 5, "%d - Common", i);
            attroff(COLOR_PAIR(COLOR_GREEN));
        }
        if(curr_potion.type == rare)
        {
            attron(COLOR_PAIR(COLOR_BLUE));
            mvprintw(i+1, 5, "%d - Rare", i);
            attroff(COLOR_PAIR(COLOR_BLUE));
        }
        if(curr_potion.type == ultra)
        {
            attron(COLOR_PAIR(COLOR_MAGENTA));
            mvprintw(i+1, 5, "%d - Ultra", i);
            attroff(COLOR_PAIR(COLOR_MAGENTA));
        }
    }
    refresh();

    while((key != 27) || (!potion_selected))
    {
        key = getch();
        if(key < (int) potions.size())
        {
            selected_potion = use_potion(key);
            potion_selected = true;
            move(21, 0);
            clrtoeol();
        }
        else{
            mvprintw(21, 0, "use the number keys to select a potion (esc to exit)");
            refresh();
        }
    }
    if(potion_selected)
    {
        mvprintw(22, 0, "You have selected to use potion %d", key);
        if((int) world.pc.roster.size() == 1){
            world.pc.roster[key].set_curr_hp(world.pc.roster[key].get_curr_hp() + selected_potion.heal);
            move(21, 0);
            clrtoeol();
            mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[key].get_name().c_str(), world.pc.roster[key].get_curr_hp());
            move(22, 0);
            clrtoeol();
            refresh();
        }
        else
        {

            mvprintw(23, 0, "Select a pokemon that you would like to use it on: ");
            for(i = 0; i < (int) world.pc.roster.size(); i++){
                mvprintw(3, 35, "%d - %s", i, world.pc.roster[i].get_name().c_str());
            }
            refresh();
            while(!pokemon_selected)
            {
                key = getch();
                if(key < (int) world.pc.roster.size())
                {
                    world.pc.roster[key].set_curr_hp(world.pc.roster[key].get_curr_hp() + selected_potion.heal);
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[key].get_name().c_str(), world.pc.roster[key].get_curr_hp());
                    move(22, 0);
                    clrtoeol();
                    refresh();
                    pokemon_selected = true;
                }
                else{
                    mvprintw(21, 0, "Use the number keys to select a pokemon");
                    refresh();
                }
            }
        }
    }

    return;
}
