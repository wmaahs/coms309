
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
    random = random_in_range(0.85, 1.00);
    if(attacker.get_type() == move.type_id)
    {
        stab = 1.5;
    }
    else{
        stab = 1.0;
    }
    if(rand() % 2 == 0)
    {
        critical_hit = 1;
    }
    else{
        critical_hit = 1.5;
    }
    damage = (((((2.0*attacker.get_level()) / 5.0) + 2.0) *
    move.power * (attacker.get_attack()/defender.get_defense())/50.0) + 2.0) * 
    critical_hit * random * stab;
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
                        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name(), enemy->get_move(enemy_move).identifier, enemy_damage);
                        refresh();
                        getch();
                        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
                        if(pc_pokemon->get_curr_hp() <= 0)
                        {
                            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name());
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
                        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name(), enemy->get_move(enemy_move).identifier);
                        refresh();
                        getch();
                    }

                    /* PC */
                    //pc hit
                    if(rand() % 100 <= pc_pokemon->get_move(0).accuracy)
                    {
                        pc_damage = calculate_damage(pc_pokemon->get_move(0), *pc_pokemon, *enemy);
                        mvprintw(23, 0, "You have selected %s, it hit for %d", pc_pokemon->get_move(0).identifier, pc_damage);
                        refresh();
                        getch();
                        enemy->set_curr_hp(enemy->get_curr_hp() - pc_damage);
                        if(enemy->get_curr_hp() <= 0)
                        {
                            mvprintw(2, 0, "HP:0/%d", enemy->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", enemy->get_name());
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
                        mvprintw(23, 0, "You have selected %s, it hit for %d", pc_pokemon->get_move(0).identifier, pc_damage);
                        refresh();
                        getch();
                        enemy->set_curr_hp(enemy->get_curr_hp() - pc_damage);
                        if(enemy->get_curr_hp() <= 0)
                        {
                            mvprintw(2, 0, "HP:0/%d", enemy->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", enemy->get_name());
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
                        mvprintw(23, 0, "You have selected %s, but it missed...", pc_pokemon->get_move(0).identifier);
                        refresh();
                        getch();
                    }

                    /* ENEMY */
                    //enemy hit
                    if(rand() % 100 <= enemy->get_move(enemy_move).accuracy)
                    {
                        enemy_damage = calculate_damage(enemy->get_move(enemy_move), *enemy, *pc_pokemon);
                        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name(), enemy->get_move(enemy_move).identifier, enemy_damage);
                        refresh();
                        getch();
                        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
                        if(pc_pokemon->get_curr_hp() <= 0)
                        {
                            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name());
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
                        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name(), enemy->get_move(enemy_move).identifier);
                        refresh();
                        getch();
                    }
                }
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
                        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name(), enemy->get_move(enemy_move).identifier, enemy_damage);
                        refresh();
                        getch();
                        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
                        if(pc_pokemon->get_curr_hp() <= 0)
                        {
                            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name());
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
                        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name(), enemy->get_move(enemy_move).identifier);
                        refresh();
                        getch();
                    }

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
                            mvprintw(23, 0, "%s has fainted", enemy->get_name());
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
                            mvprintw(23, 0, "%s has fainted", enemy->get_name());
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
                        mvprintw(23, 0, "%s attacked with %s, it hit for %d", enemy->get_name(), enemy->get_move(enemy_move).identifier, enemy_damage);
                        refresh();
                        getch();
                        pc_pokemon->set_curr_hp(pc_pokemon->get_curr_hp() - enemy_damage);
                        if(pc_pokemon->get_curr_hp() <= 0)
                        {
                            mvprintw(12, 0, "HP:0/%d", pc_pokemon->get_hp());
                            move(23, 0);
                            clrtoeol();
                            mvprintw(23, 0, "%s has fainted", pc_pokemon->get_name());
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
                        mvprintw(23, 0, "%s attacked with %s, but it missed...", enemy->get_name(), enemy->get_move(enemy_move).identifier);
                        refresh();
                        getch();
                    }
                }
                break;
            case '3':
                return;
            default:
                mvprintw(23, 0, "Pick move one or two");
                break;
            
        }
    }

    return;
}

