
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
        else if(i == DEFAULT_POKEBALLS -2){
            curr_pokeball.type = rare;
            curr_pokeball.boost = 3;
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

            revives.push_back(curr_revive);
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
            potions.push_back(curr_potion);
    }
}

bool Bag::open_bag(bool trainer_battle, Pokemon *enemy)
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
            if(trainer_battle == false)
            {
                this->open_pokeballs(enemy);
                option_selected = true;
                return true;
            }
            else
            {
                move(21, 0);
                clrtoeol();
                mvprintw(21, 0, "You can only use pokeballs in battles against wild pokemon");
                refresh();
                getch();
                move(21, 0);
                clrtoeol();
                refresh();
            }
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
    return false;
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

void Bag::open_pokeballs(Pokemon *enemy)
{
    int i, key;
    pokeball_t curr_pokeball;
    pokeball_t selected_pokeball;
    bool pokeball_selected = false;
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
    mvprintw(10, 5, "8 - Back");
    refresh();
    
    while(!pokeball_selected)
    {
        key = getch();
        switch(key)
        {
            case '1':
                if((int) pokeballs.size() > 0)
                {
                    selected_pokeball = use_pokeball(0);
                    pokeball_selected = true;
                }
                else{
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You dont have that many pokeballs");
                    refresh();
                    getch();
                    move(21, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '2':
                if((int) pokeballs.size() > 1)
                {
                    selected_pokeball = use_pokeball(1);
                    pokeball_selected = true;
                }
                else{
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You dont have that many pokeballs");
                    refresh();
                    getch();
                    move(21, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '3':
                if((int) pokeballs.size() > 2)
                {
                    selected_pokeball = use_pokeball(2);
                    pokeball_selected = true;
                }
                else{
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You dont have that many pokeballs");
                    refresh();
                    getch();
                    move(21, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '4':
                if((int) pokeballs.size() > 3)
                {
                    selected_pokeball = use_pokeball(3);
                    pokeball_selected = true;
                }
                else{
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You dont have that many pokeballs");
                    refresh();
                    getch();
                    move(21, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '5':
                if((int) pokeballs.size() > 4)
                {
                    selected_pokeball = use_pokeball(4);
                    pokeball_selected = true;
                }
                else{
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You dont have that many pokeballs");
                    refresh();
                    getch();
                    move(21, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '6':
                if((int) pokeballs.size() > 5)
                {
                    selected_pokeball = use_pokeball(5);
                    pokeball_selected = true;
                }
                else{
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You dont have that many pokeballs");
                    refresh();
                    getch();
                    move(21, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '7':
                if((int) pokeballs.size() > 6)
                {
                    selected_pokeball = use_pokeball(6);
                    pokeball_selected = true;
                }
                else{
                    move(21, 0);
                    clrtoeol();
                    mvprintw(21, 0, "You dont have that many pokeballs");
                    refresh();
                    getch();
                    move(21, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '8':
                return;
                break;
            default:
                mvprintw(21, 0, "Use the number keys to select a pokeball (8 to go back)");
                refresh();
                getch();
                move(21, 0);
                clrtoeol();
                refresh();
                break;
        }
    }
    if(pokeball_selected)
    {
        if(world.pc.roster.size() < 7)
        {
            world.pc.roster.push_back(*enemy);
        }
        else
        {
            world.pc.roster.erase(world.pc.roster.end()-1);
            world.pc.roster.push_back(*enemy);
        }
    }
    return;
}

void Bag::open_revives()
{
    int i, key;
    revive_t curr_revive;
    revive_t selected_revive;
    bool revive_selected = false;
    bool pokemon_selected = false;
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


    while((key != 27) && (!revive_selected))
    {
        key = getch();
        //'1' = int 50, not 1..
        switch(key)
        {
            case '0':
                selected_revive = use_revive(0);
                revive_selected = true;
                move(21, 0);
                clrtoeol();
                break;
            case '1':
                if((int) revives.size() > 1)
                {
                    selected_revive = use_revive(1);
                    revive_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 1 revive");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '2':
                if((int) revives.size() > 2)
                {
                    selected_revive = use_revive(2);
                    revive_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 2 revives");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '3':
                if((int) revives.size() > 3)
                {
                    selected_revive = use_revive(3);
                    revive_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 3 revives");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                
                break;
            case '4':
                if((int) revives.size() > 4)
                {
                    selected_revive = use_revive(4);
                    revive_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 4 revives");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            default:
                mvprintw(21, 0, "use the number keys to select a revive (esc to exit)");
                refresh();
                break;
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
                mvprintw(i+3, 35, "%d - %s", i+1, world.pc.roster[i].get_name().c_str());
            }
            refresh();
            while(!pokemon_selected)
            {
                key = getch();
                switch(key)
                {
                    case '1':
                        world.pc.roster[0].set_curr_hp(world.pc.roster[0].get_curr_hp() + selected_revive.heal);
                        move(21, 0);
                        clrtoeol();
                        mvprintw(21, 0, "You revived %s to %d HP", world.pc.roster[0].get_name().c_str(), world.pc.roster[0].get_curr_hp());
                        move(22, 0);
                        clrtoeol();
                        refresh();
                        pokemon_selected = true;
                        break;
                    case '2':
                        if((int) world.pc.roster.size() > 1)
                        {
                            world.pc.roster[1].set_curr_hp(world.pc.roster[1].get_curr_hp() + selected_revive.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You revived %s to %d HP", world.pc.roster[1].get_name().c_str(), world.pc.roster[1].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 1 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    case '3':
                        if((int) world.pc.roster.size() > 2)
                        {
                            world.pc.roster[2].set_curr_hp(world.pc.roster[2].get_curr_hp() + selected_revive.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You revived %s to %d HP", world.pc.roster[2].get_name().c_str(), world.pc.roster[2].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 2 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    case '4':
                        if((int) world.pc.roster.size() > 3)
                        {
                            world.pc.roster[3].set_curr_hp(world.pc.roster[3].get_curr_hp() + selected_revive.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You revived %s to %d HP", world.pc.roster[3].get_name().c_str(), world.pc.roster[3].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 3 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    case '5':
                        if((int) world.pc.roster.size() > 4)
                        {
                            world.pc.roster[4].set_curr_hp(world.pc.roster[4].get_curr_hp() + selected_revive.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You revived %s to %d HP", world.pc.roster[4].get_name().c_str(), world.pc.roster[4].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 4 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    case '6':
                        if((int) world.pc.roster.size() > 5)
                        {
                            world.pc.roster[5].set_curr_hp(world.pc.roster[5].get_curr_hp() + selected_revive.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You revived %s to %d HP", world.pc.roster[5].get_name().c_str(), world.pc.roster[5].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 5 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    default:
                        mvprintw(21, 0, "Use the number keys to select a pokemon");
                        refresh();
                        break;
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

    while((key != 27) && (!potion_selected))
    {
        key = getch();
        switch(key)
        {
            case '0':
                selected_potion = use_potion(0);
                potion_selected = true;
                move(21, 0);
                clrtoeol();
                break;
            case '1':
                if((int) potions.size() > 1)
                {
                    selected_potion = use_potion(1);
                    potion_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 1 potion");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '2':
                if((int) potions.size() > 2)
                {
                    selected_potion = use_potion(2);
                    potion_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 2 potions");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '3':
                if((int) potions.size() > 3)
                {
                    selected_potion = use_potion(3);
                    potion_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 3 potions");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                
                break;
            case '4':
                if((int) potions.size() > 4)
                {
                    selected_potion = use_potion(4);
                    potion_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 4 potions");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '5':
            if((int) potions.size() > 5)
                {
                    selected_potion = use_potion(5);
                    potion_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 5 potions");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }

                break;
            case '6':
                if((int) potions.size() > 6)
                {
                    selected_potion = use_potion(6);
                    potion_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 6 potions");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '7':
                if((int) potions.size() > 7)
                {
                    selected_potion = use_potion(7);
                    potion_selected = true;
                    move(21, 0);
                    clrtoeol();
                }
                else
                {
                    mvprintw(22, 0, "You only have 7 potions");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            default:
                mvprintw(21, 0, "use the number keys to select a potion (esc to exit)");
                refresh();
                break;
        }
    }
    if(potion_selected)
    {
        mvprintw(22, 0, "You have selected to use potion %d", key);
        if((int) world.pc.roster.size() == 1){
            world.pc.roster[0].set_curr_hp(world.pc.roster[0].get_curr_hp() + selected_potion.heal);
            move(21, 0);
            clrtoeol();
            mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[0].get_name().c_str(), world.pc.roster[0].get_curr_hp());
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
                switch(key)
                {
                    case '1':
                        world.pc.roster[0].set_curr_hp(world.pc.roster[0].get_curr_hp() + selected_potion.heal);
                        move(21, 0);
                        clrtoeol();
                        mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[0].get_name().c_str(), world.pc.roster[0].get_curr_hp());
                        move(22, 0);
                        clrtoeol();
                        refresh();
                        pokemon_selected = true;
                        break;
                    case '2':
                        if((int) world.pc.roster.size() > 1)
                        {
                            world.pc.roster[1].set_curr_hp(world.pc.roster[1].get_curr_hp() + selected_potion.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[1].get_name().c_str(), world.pc.roster[1].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 1 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    case '3':
                        if((int) world.pc.roster.size() > 2)
                        {
                            world.pc.roster[2].set_curr_hp(world.pc.roster[2].get_curr_hp() + selected_potion.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[2].get_name().c_str(), world.pc.roster[2].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 2 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    case '4':
                        if((int) world.pc.roster.size() > 3)
                        {
                            world.pc.roster[3].set_curr_hp(world.pc.roster[3].get_curr_hp() + selected_potion.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[3].get_name().c_str(), world.pc.roster[3].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 3 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    case '5':
                        if((int) world.pc.roster.size() > 4)
                        {
                            world.pc.roster[4].set_curr_hp(world.pc.roster[4].get_curr_hp() + selected_potion.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[4].get_name().c_str(), world.pc.roster[4].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 4 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    case '6':
                        if((int) world.pc.roster.size() > 5)
                        {
                            world.pc.roster[5].set_curr_hp(world.pc.roster[5].get_curr_hp() + selected_potion.heal);
                            move(21, 0);
                            clrtoeol();
                            mvprintw(21, 0, "You healed %s to %d HP", world.pc.roster[5].get_name().c_str(), world.pc.roster[5].get_curr_hp());
                            move(22, 0);
                            clrtoeol();
                            refresh();
                            pokemon_selected = true;
                        }
                        else
                        {
                            mvprintw(22, 0, "You only have 5 Pokemon");
                            refresh();
                            getch();
                            move(22, 0);
                            clrtoeol();
                            refresh();
                        }
                        break;
                    default:
                        mvprintw(21, 0, "Use the number keys to select a pokemon");
                        refresh();
                        break;
                }
            }
        }
    }

    return;
}
