
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
        curr_pokeball.type = common;
        curr_pokeball.boost = 1;
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
        curr_potion.type = common;
        curr_potion.heal = 10;
        potions.push_back(curr_potion);
    }
}

void Bag::open_bag()
{
    int key;
    clear();
    mvprintw(1, 0, "1. Pokeballs (%d)", (int) pokeballs.size());
    mvprintw(3, 0, "2. Revives (%d)", (int) revives.size());
    mvprintw(5, 0, "3. Potions (%d)", (int) potions.size());
    refresh();

    key = getch();
    switch (key)
    {
    case '1':
        
        break;
    case '2':
        break;
    case '3':
        break;
    default:
        break;
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
    mvprintw(0, 33, "POKEBALLS (%d)", pokeballs.size());
    attroff(COLOR_PAIR(COLOR_CYAN));

    for(i = 0; i < pokeballs.size(); i++)
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
    int i;
    revive_t curr_revive;
    clear();
    if(revives.empty()){
        mvprintw(0, 1, "You do not have any Revives left!");
        refresh();
        getch();
        return;
    }

    attron(COLOR_PAIR(COLOR_GREEN));
    mvprintw(0, 33, "REVIVES (%d)", revives.size());
    attroff(COLOR_PAIR(COLOR_GREEN));

    for(i = 0; i < revives.size(); i++)
    {
        curr_revive = revives.at(i);
        if(curr_revive.type == common)
        {

            mvprintw(i, 5, "%d - Common", i);
        }
        if(curr_revive.type == rare)
        {
            mvprintw(i, 5, "%d - Rare", i);
        }
        if(curr_revive.type == ultra)
        {
            mvprintw(i, 5, "%d - Ultra", i);
        }
    }
    refresh();
    getch();
    return;
}
void Bag::open_potions()
{
    int i;
    potion_t curr_potion;
    clear();
    if(potions.empty()){
        mvprintw(0, 1, "You do not have any Potions left!");
        refresh();
        getch();
        return;
    }

    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(0, 33, "POTIONS (%d)", potions.size());
    attroff(COLOR_PAIR(COLOR_RED));

    for(i = 0; i < potions.size(); i++)
    {
        curr_potion = potions.at(i);
        if(curr_potion.type == common)
        {
            mvprintw(i, 5, "%d - Common", i);
        }
        if(curr_potion.type == rare)
        {
            mvprintw(i, 5, "%d - Rare", i);
        }
        if(curr_potion.type == ultra)
        {
            mvprintw(i, 5, "%d - Ultra", i);
        }
    }
    refresh();
    getch();
    return;
}