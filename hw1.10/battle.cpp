#include "poke327.h"
#include "battle.h"
#include <ncurses.h>


double random_in_range(double min, double max)
{
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

double calculate_damage(move_db move, Pokemon attacker, Pokemon defender)
{
    double damage;
    double random;
    double critical_hit;
    double stab;
    int power;
    if(move.power == INT32_MAX)
    {
        power = 1;
    }
    else{
        power = move.power;
    }
    random = random_in_range(0.85, 1.00);
    if(attacker.get_type() == move.type_id)
    {
        stab = 1.5;
    }
    else{
        stab = 1.0;
    }
    if(rand() % 255 < (attacker.get_speed()/2))
    {
        critical_hit = 1.5;
    }
    else{
        critical_hit = 1;
    }
    damage = (((((2.0*attacker.get_level()) / 5.0) + 2.0) *
    power * (attacker.get_attack()/defender.get_defense())/50.0) + 2.0) * 
    critical_hit * random * stab;

    return damage;
}

void battle_fight(Pokemon *enemy, Pokemon *pc_pokemon)
{
    int enemy_move;
    int key;
    bool move_selected = false;
    int pc_damage;
    int enemy_damage;
    
    //clear battle options
    move(15, 0);
    clrtoeol();
    move(17, 0);
    clrtoeol();
    move(19, 0);
    clrtoeol();
    move(21, 0);
    clrtoeol();

    mvprintw(17, 0, "1. %s", pc_pokemon->get_move(0).identifier);
    mvprintw(19, 0, "2. %s", pc_pokemon->get_move(1).identifier);
    mvprintw(21, 0, "3. Back");
    refresh();

    while(!move_selected)
    {

        key = getch();

        switch(key){
            /* Move 1 */
            case '1':
                enemy_move = rand() % 2;
                //determine who attacks first
                if(enemy->get_move(enemy_move).priority > pc_pokemon->get_move(0).priority)
                //enemy first
                {
                    /* ENEMY */
                    //enemy hit
                    if(rand() % 100 <= enemy->get_move(enemy_move).accuracy)
                    {
                        enemy_damage = calculate_damage(enemy->get_move(enemy_move), *enemy, *pc_pokemon);
                        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier, enemy_damage);
                        refresh();
                        getch();
                        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
                        if(pc_pokemon->get_curr_hp() <= 0)
                        {
                            move(12, 0);
                            clrtoeol();
                            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name().c_str());
                            refresh();
                            getch();
                            return;
                        }
                        move(12, 0);
                        clrtoeol();
                        mvprintw(12, 0, "HP:%d/%d", pc_pokemon->get_curr_hp(), pc_pokemon->get_hp());
                        refresh();
                        getch();
                    }
                    //enemy miss
                    else
                    {
                        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier);
                        refresh();
                        getch();
                    }

                    /* PC */
                    //pc hit
                    if(rand() % 100 <= pc_pokemon->get_move(0).accuracy)
                    {
                        pc_damage = calculate_damage(pc_pokemon->get_move(0), *pc_pokemon, *enemy);
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "You have selected %s, it hit for %d", pc_pokemon->get_move(0).identifier, pc_damage);
                        refresh();
                        getch();
                        enemy->set_curr_hp(enemy->get_curr_hp() - pc_damage);
                        if(enemy->get_curr_hp() <= 0)
                        {
                            move(2, 0);
                            clrtoeol();
                            mvprintw(2, 0, "HP:0/%d", enemy->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", enemy->get_name().c_str());
                            refresh();
                            getch();
                            return;
                        }
                        move(2, 0);
                        clrtoeol();
                        mvprintw(2, 0, "HP:%d/%d", enemy->get_curr_hp(), enemy->get_hp());
                        refresh();
                        getch();
                    }
                    //pc miss
                    else
                    {
                        mvprintw(23, 0, "You have selected %s, but it missed...", pc_pokemon->get_move(0).identifier);
                        refresh();
                        getch();
                    }
                }
                else
                // pc first
                {
                    /* PC */
                    //pc hit
                    if(rand() % 100 <= pc_pokemon->get_move(0).accuracy)
                    {
                        pc_damage = calculate_damage(pc_pokemon->get_move(0), *pc_pokemon, *enemy);
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "You have selected %s, it hit for %d", pc_pokemon->get_move(0).identifier, pc_damage);
                        refresh();
                        getch();
                        enemy->set_curr_hp(enemy->get_curr_hp() - pc_damage);
                        if(enemy->get_curr_hp() <= 0)
                        {
                            move(2, 0);
                            clrtoeol();
                            mvprintw(2, 0, "HP:0/%d", enemy->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", enemy->get_name().c_str());
                            refresh();
                            getch();
                            return;
                        }
                        move(2, 0);
                        clrtoeol();
                        mvprintw(2, 0, "HP:%d/%d", enemy->get_curr_hp(), enemy->get_hp());
                        refresh();
                        getch();
                    }
                    //pc miss
                    else
                    {
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "You have selected %s, but it missed...", pc_pokemon->get_move(0).identifier);
                        refresh();
                        getch();
                    }

                    /* ENEMY */
                    //enemy hit
                    if(rand() % 100 <= enemy->get_move(enemy_move).accuracy)
                    {
                        enemy_damage = calculate_damage(enemy->get_move(enemy_move), *enemy, *pc_pokemon);
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier, enemy_damage);
                        refresh();
                        getch();
                        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
                        if(pc_pokemon->get_curr_hp() <= 0)
                        {
                            move(12, 0);
                            clrtoeol();
                            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name().c_str());
                            refresh();
                            getch();
                            return;
                        }
                        move(12, 0);
                        clrtoeol();
                        mvprintw(12, 0, "HP:%d/%d", pc_pokemon->get_curr_hp(), pc_pokemon->get_hp());
                        refresh();
                        getch();
                    }
                    //enemy miss
                    else
                    {
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier);
                        refresh();
                        getch();
                    }
                }
                move_selected = true;
                break;
            /* Move 2 */
            case '2':
                enemy_move = rand() % 2;
                //determine who attacks first
                if(enemy->get_move(enemy_move).priority > pc_pokemon->get_move(1).priority)
                //enemy first
                {
                    /* ENEMY */
                    //enemy hit
                    if(rand() % 100 <= enemy->get_move(enemy_move).accuracy)
                    {
                        enemy_damage = calculate_damage(enemy->get_move(enemy_move), *enemy, *pc_pokemon);
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier, enemy_damage);
                        refresh();
                        getch();
                        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
                        if(pc_pokemon->get_curr_hp() <= 0)
                        {
                            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name().c_str());
                            refresh();
                            getch();
                            return;
                        }
                        mvprintw(12, 0, "HP:%d/%d", pc_pokemon->get_curr_hp(), pc_pokemon->get_hp());
                        refresh();
                        getch();
                    }
                    //enemy miss
                    else
                    {
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier);
                        refresh();
                        getch();
                    }

                    /* PC */
                    //pc hit
                    if(rand() % 100 <= pc_pokemon->get_move(1).accuracy)
                    {
                        pc_damage = calculate_damage(pc_pokemon->get_move(1), *pc_pokemon, *enemy);
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "You have selected %s, it hit for %d", pc_pokemon->get_move(1).identifier, pc_damage);
                        refresh();
                        getch();
                        enemy->set_curr_hp(enemy->get_curr_hp() - pc_damage);
                        if(enemy->get_curr_hp() <= 0)
                        {
                            mvprintw(2, 0, "HP:0/%d", enemy->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", enemy->get_name().c_str());
                            refresh();
                            getch();
                            return;
                        }
                        mvprintw(2, 0, "HP:%d/%d", enemy->get_curr_hp(), enemy->get_hp());
                        refresh();
                        getch();
                    }
                    //pc miss
                    else
                    {
                        move(23, 0);
                        clrtoeol();
                        mvprintw(23, 0, "You have selected %s, but it missed...", pc_pokemon->get_move(1).identifier);
                        refresh();
                        getch();
                    }
                }
                else
                // pc first
                {
                    /* PC */
                    //pc hit
                    if(rand() % 100 <= pc_pokemon->get_move(1).accuracy)
                    {
                        pc_damage = calculate_damage(pc_pokemon->get_move(1), *pc_pokemon, *enemy);
                        mvprintw(23, 0, "You have selected %s, it hit for %d", pc_pokemon->get_move(1).identifier, pc_damage);
                        refresh();
                        getch();
                        enemy->set_curr_hp(enemy->get_curr_hp() - pc_damage);
                        if(enemy->get_curr_hp() <= 0)
                        {
                            mvprintw(2, 0, "HP:0/%d", enemy->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", enemy->get_name().c_str());
                            refresh();
                            getch();
                            return;
                        }
                        mvprintw(2, 0, "HP:%d/%d", enemy->get_curr_hp(), enemy->get_hp());
                        refresh();
                        getch();
                    }
                    //pc miss
                    else
                    {
                        mvprintw(23, 0, "You have selected %s, but it missed...", pc_pokemon->get_move(1).identifier);
                        refresh();
                        getch();
                    }

                    /* ENEMY */
                    //enemy hit
                    if(rand() % 100 <= enemy->get_move(enemy_move).accuracy)
                    {
                        enemy_damage = calculate_damage(enemy->get_move(enemy_move), *enemy, *pc_pokemon);
                        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier, enemy_damage);
                        refresh();
                        getch();
                        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
                        if(pc_pokemon->get_curr_hp() <= 0)
                        {
                            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name().c_str());
                            refresh();
                            getch();
                            return;
                        }
                        mvprintw(12, 0, "HP:%d/%d", pc_pokemon->get_curr_hp(), pc_pokemon->get_hp());
                        refresh();
                        getch();
                    }
                    //enemy miss
                    else
                    {
                        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier);
                        refresh();
                        getch();
                    }
                }
                move_selected = true;
                break;
            case '3':
                move_selected = true;
                return;
            default:
                move(23, 0);
                clrtoeol();
                mvprintw(23, 0, "Pick move one or two, or select '3' to go back");
                break;
            
        }
    }

    return;
}

/**
 * This is called whenever the PC tries to run, use an item, or switch pokemon.
 * Therefore the PC is using its action to do one of the previously mentioned actions,
 * so the enemy gets to attack the PC for free
*/
void enemy_free_attack(Pokemon *enemy, Pokemon *pc_pokemon)
{
    int enemy_move = rand() % 2;
    double enemy_damage;
    if(rand() % 100 <= enemy->get_move(enemy_move).accuracy)
    {
        enemy_damage = calculate_damage(enemy->get_move(enemy_move), *enemy, *pc_pokemon);
        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier, (int) enemy_damage);
        refresh();
        getch();
        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
        if(pc_pokemon->get_curr_hp() <= 0)
        {
            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
            move(23, 0);
            clrtoeol();
            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name().c_str());
            refresh();
            getch();
            return;
        }
        mvprintw(12, 0, "HP:%d/%d", pc_pokemon->get_curr_hp(), pc_pokemon->get_hp());
        refresh();
        getch();
    }
    //enemy miss
    else
    {
        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name().c_str(), enemy->get_move(enemy_move).identifier);
        refresh();
        getch();
    }
}

Pokemon select_pokemon()
{
    int i, key;
    bool pokemon_selected = false;
    Pokemon new_pokemon;
    clear();

    attron(COLOR_PAIR(COLOR_CYAN));
    mvprintw(0, 33, "POKEMON");
    attroff(COLOR_PAIR(COLOR_CYAN));

    for(i = 0; i < (int) world.pc.roster.size(); i++)
    {
        mvprintw(i+3, 5, "%d, -- %s", i+1, world.pc.roster[i].get_name().c_str());
    }
    mvprintw(0, 0, "Select a pokemon from your roster");
    refresh();

    while(!pokemon_selected)
    {
        key = getch();
        switch(key)
        {
           case '1':
                if(world.pc.roster[0].get_curr_hp() > 0)
                {
                    new_pokemon = world.pc.roster[0];
                    pokemon_selected = true;
                }
                else
                {
                    mvprintw(22, 0, "This pokemon has fainted");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '2':
                if(((int) world.pc.roster.size() > 1) && (world.pc.roster[1].get_curr_hp()))
                {
                    new_pokemon = world.pc.roster[0];
                    pokemon_selected = true;
                }
                else
                {
                    mvprintw(22, 0, "You only have 1 Pokemon, or this pokemon has fainted");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '3':
                if(((int) world.pc.roster.size() > 2) && (world.pc.roster[2].get_curr_hp()))
                {
                    new_pokemon = world.pc.roster[2];
                    pokemon_selected = true;
                }
                else
                {
                    mvprintw(22, 0, "You only have 2 Pokemon, or this pokemon has fainted");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '4':
                if(((int) world.pc.roster.size() > 3) && (world.pc.roster[3].get_curr_hp() > 0))
                {
                    new_pokemon = world.pc.roster[3];
                    pokemon_selected = true;
                }
                else
                {
                    mvprintw(22, 0, "You only have 3 Pokemon, or this pokemon has fainted");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '5':
                if(((int) world.pc.roster.size() > 4) && (world.pc.roster[4].get_curr_hp() > 0))
                {
                    new_pokemon = world.pc.roster[4];
                    pokemon_selected = true;
                }
                else
                {
                    mvprintw(22, 0, "You only have 4 Pokemon, or this pokemon has fainted");
                    refresh();
                    getch();
                    move(22, 0);
                    clrtoeol();
                    refresh();
                }
                break;
            case '6':
                if(((int) world.pc.roster.size() > 5) && (world.pc.roster[5].get_curr_hp() > 0))
                {
                    new_pokemon = world.pc.roster[5];
                    pokemon_selected = true;
                }
                else
                {
                    mvprintw(22, 0, "You only have 5 Pokemon, or this pokemon has fainted");
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
    return new_pokemon;
}



