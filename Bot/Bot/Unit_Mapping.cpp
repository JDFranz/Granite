#include <BWAPI.h>
#include "Unit_Mapping.h"
#include <fstream>
#include <string>

using namespace BWAPI;
using namespace Filter;

// Constructor../
Unit_Mapping::Unit_Mapping()
{
        set_start_count();
        //set_build_list();
}//..

// Initializers
// Set Count ../
void Unit_Mapping::set_start_count()
{
        /*
         * update - map         -> unit counts are set for match start
         *
         * idea - set all units to 0, some trickery for getting the count of
         *        starting units.
         */

        for(auto & u : BWAPI::UnitTypes::allUnitTypes())
        {
                g_unit_count[u] = 0;
                g_warp_count[u] = 0;
                e_unit_count[u] = 0;
                e_warp_count[u] = 0;
        }//set negative because game start will count created and completed
        g_unit_count[BWAPI::UnitTypes::Protoss_Nexus] = -1;
        g_unit_count[BWAPI::UnitTypes::Protoss_Probe] = -4;
        g_warp_count[BWAPI::UnitTypes::Protoss_Nexus] = 1;
        g_warp_count[BWAPI::UnitTypes::Protoss_Probe] = 4;
}//..

// Set Build List ../
void Unit_Mapping::set_build_list()
        /*
         * update - map         -> updates map with a list of building types
         *                         and their location
         *
         * idea - read and parse building data created with stardraft
         */ 
{
        std::string data;
        std::ifstream file("data/build.txt");

        std::cout << "reading file\n"; 

        if(!file)
        {
            std::cerr << "can't open map data";
            return;
        }
        file >> data;
        std::cout << "data reads: " << data << std::endl;
        parse_build_data(data);
        file.close();
}//..

// Helpers for set build list
// Parse Build Data../
void Unit_Mapping::parse_build_data(std::string data)
        /*
         * input - data         -> line of csv data
         *
         * update - m_building  -> vector with building type and location
         *
         * idea - tokenize csv data and add it to vector
         */
{
        int type, x, y;
        std::vector<std::string> tokens;
        tokens = parse_csv(data);

        for( auto &u : tokens)
                std::cout << u << " ";
        while (!tokens.empty())
        {
                type = get_type(tokens.back());
                tokens.pop_back();
                x = get_coord(tokens.back());
                tokens.pop_back();
                y = get_coord(tokens.back());
                tokens.pop_back();

                m_building.push_back( {type, {x, y}} );
        }
}//..

// Get Type../
int Unit_Mapping::get_type(std::string data)
{
        /*
         * input - data         -> String containing building type
         * output - bldg type   -> Enum value of that building type
         *
         * idea - switch case for strings
         */

        if (data == "Nexus")
                return NEXUS;
        if (data == "Pylon")
                return PYLON;
        if (data == "Gateway")
                return GATEWAY;
        if (data == "Assimilator")
                return ASSIMILATOR;
        if (data == "Shield")
                return SHIELD;
        if (data == "Forge")
                return FORGE;
        if (data == "Cannon")
                return CANNON;
        if (data == "Core")
                return CORE;
        if (data == "Stargate")
                return STARGATE;
        if (data == "Robo")
                return ROBO;
        if (data == "Observatory")
                return OBSERVATORY;
        if (data == "Bay")
                return BAY;
        if (data == "Citadel")
                return CITADEL;
        if (data == "Archives")
                return ARCHIVES;
        if (data == "Beacon")
                return BEACON;
        if (data == "Tribunal")
                return TRIBUNAL;
        return 0;
}//..

// Get Coordinate../
int Unit_Mapping::get_coord(std::string data)
{
        /* 
         * input - data         -> data in string form
         * output - data        -> data in integer form
         */

        return stoi(data);
}//..

// Get CSV Tokens../
std::vector<std::string> Unit_Mapping::parse_csv(std::string line)
{
        /*
         * input - line         -> String of csv data
         * output - vec         -> Vector containing tokenized strings
         *
         * idea - cuts on the comma, data must be formatted correctly
         *        no handling for special cases
         */

        std::vector<char *> vec;
                char * i = new char;
        char * sp = i;

        for (auto &c : line)
        {
                switch (c)
                {
                        case ',':
                                *i = '\0';
                                vec.push_back(sp);
                                i = new char;
                                sp = i;
                                break;
                        default:
                                *i++ = c;
                }

        }
        *i = '\0';
        vec.push_back(sp);
        return reverse_tokens(vec);
}

std::vector<std::string> Unit_Mapping::reverse_tokens(std::vector<char*> vec)
{
        std::vector<std::string> ret;

        while (!vec.empty())
        {
                ret.push_back( vec.back() );
                vec.pop_back();
        }
        return ret;
}//..

// Building construction map interface
// Get Build Type../
BWAPI::UnitType Unit_Mapping::get_build_type()
{
        /*
         * output - type        -> The building type of first unit in vector
         * idea - enum conversion
         */

        switch (m_building.front().type)
        {
                case NEXUS:
                        return BWAPI::UnitTypes::Protoss_Nexus;
                case PYLON:
                        return BWAPI::UnitTypes::Protoss_Pylon;
                case GATEWAY:
                        return BWAPI::UnitTypes::Protoss_Gateway;
                case ASSIMILATOR:
                        return BWAPI::UnitTypes::Protoss_Assimilator;
                case SHIELD:
                        return BWAPI::UnitTypes::Protoss_Shield_Battery;
                case FORGE:
                        return BWAPI::UnitTypes::Protoss_Forge;
                case CANNON:
                        return BWAPI::UnitTypes::Protoss_Photon_Cannon;
                case CORE:
                        return BWAPI::UnitTypes::Protoss_Cybernetics_Core;
                case STARGATE:
                        return BWAPI::UnitTypes::Protoss_Stargate;
                case ROBO:
                        return BWAPI::UnitTypes::Protoss_Robotics_Facility;
                case OBSERVATORY:
                        return BWAPI::UnitTypes::Protoss_Observatory;
                case BAY:
                    return BWAPI::UnitTypes::Protoss_Robotics_Support_Bay;
                case CITADEL:
                        return BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
                case ARCHIVES:
                        return BWAPI::UnitTypes::Protoss_Templar_Archives;
                case BEACON:
                        return BWAPI::UnitTypes::Protoss_Fleet_Beacon;
                case TRIBUNAL:
                    return BWAPI::UnitTypes::Protoss_Arbiter_Tribunal;
        }
}//..

// Get Building Location../
BWAPI::Position Unit_Mapping::get_build_loc()
{
        /*
         * output - loc         -> Position of first unit in building vector
         */
        return m_building.front().loc;
}//..

// Check if Build List is empty../
bool Unit_Mapping::bl_empty()
{
        /*
         * output - bool        -> True if the vector is empty
         *                      -> False otherwise
         */
        return m_building.empty();
}//..

// Remove entry from Build List../
void Unit_Mapping::remove_build()
{
        /*
         * update - m_building          -> Removes first element
         */
        m_building.erase(m_building.begin()); 
}//..


// Unit Counting interface
// Add To Count../
void Unit_Mapping::add_to_count(BWAPI::Unit u)
        /*
         * input - unit         -> unit to be counted
         *
         * update - one of the following
         *        g_unit_count  -> if isOwned and isCompleted
         *        e_unit_count  -> if !isOwned and isCopleted
         *        g_warp_count  -> if isOwned and !isCompleted
         *        e_warp_count  -> if !isOwned and !isCompleted
         *
         *
         * idea - if else handling to update the count
         *        some fancy handling for enemy units to get the transition
         *        from warping to completed
         *        
         */
{
        if ( !u->getPlayer()->isEnemy(Broodwar->self()))
        {
                if ( u->isCompleted() )
                        g_unit_count[u->getType()]++;
                else
                        g_warp_count[u->getType()]++;
        } else {
                if ( u->isCompleted() )
                {
                        if ( enemy_mapped(u) )
                                return;
                        e_unit_count[u->getType()]++;
                        if ( w_enemy_mapped(u) )
                        {
                                w_del_enemy_ID(u);
                                remove_from_count(u, !u->isCompleted());
                        }
                        add_enemy_ID(u);
                } else {
                        if ( w_enemy_mapped(u) )
                                return;
                        e_warp_count[u->getType()]++;
                        w_add_enemy_ID(u);
                }
        }
}//..

// Remove From count../
void Unit_Mapping::remove_from_count(BWAPI::Unit u, bool completed)
        /*
         * input - Unit u       -> Unit to remove from count
         *         completed    -> status of unit (isComplete)
         *
         * update - one of the following
         *              g_warp_count    -> isOwned && !completed
         *              g_unit_count    -> isOwned && completed
         *              e_warp_count    -> !isOwned && !completed
         *              e_unit_count    -> !isOwned && completed
         *
         * idea - if else handling also deletes enemy ID from list if
         *        necessary
         */
{
        if (!u->getPlayer()->isEnemy(Broodwar->self()))
        {//self
                if ( !completed )
                        g_warp_count[u->getType()]--;
                else
                        g_unit_count[u->getType()]--;
        } else {//enemy
                if ( !completed )
                {
                        e_warp_count[u->getType()]--;
                        w_del_enemy_ID(u);
                }
                else
                {
                        e_unit_count[u->getType()]--;
                        del_enemy_ID(u);
                }
        }
}//..

// Mapping Enemy Bases
// Add Enemy Base../
void Unit_Mapping::add_enemy_base(BWAPI::Unit u)
        /*
         * update - enemy_bases         -> add center of enemy resource depot
         *
         * idea - add the base with offset from top left corner
         */
{
        enemy_bases.push_back(u->getPosition() + Position(64, 48));
}//..

// Add Enemy Base../
void Unit_Mapping::add_enemy_base(BWAPI::Position pos)
        /*
         * update - enemy_bases         -> add center of enemy resource depot
         *
         * idea - add the base with offset from top left corner
         */
{
        enemy_bases.push_back(pos + Position(64, 48));
}//..

// Enemy Base Found?../
bool Unit_Mapping::enemy_base_found(BWAPI::Unit u)
{
        return enemy_bases.empty();
}//..

// Getting Granite's Mapped Units
int Unit_Mapping::g_count(BWAPI::UnitType u)// ../
{
        return g_unit_count[u];
}//..

int Unit_Mapping::g_total_count(BWAPI::UnitType u)// ../
{
        return g_warp_count[u] + g_unit_count[u];
}//..

int Unit_Mapping::g_warping_count(BWAPI::UnitType u)// ../
{
        return g_warp_count[u];
}//..

// Getting Enemy's Mapped Units
int Unit_Mapping::e_count(BWAPI::UnitType u)// ../
{
        return e_unit_count[u];
}//..

int Unit_Mapping::e_total_count(BWAPI::UnitType u)// ../
{
        return e_warp_count[u] + e_unit_count[u];
}//..

int Unit_Mapping::e_warping_count(BWAPI::UnitType u)// ../
{
        return e_warp_count[u];
}//..

// Enemy ID mapping
// Mapping enemy unit ID ../
bool Unit_Mapping::enemy_mapped(BWAPI::Unit u)
{
        return enemy_units[u->getID()];
}

void Unit_Mapping::add_enemy_ID(BWAPI::Unit u)
{
        enemy_units[u->getID()] = true;
}
void Unit_Mapping::del_enemy_ID(BWAPI::Unit u)
{
        enemy_units[u->getID()] = false;
}//..

// Mapping warping enemy unit ID ../
bool Unit_Mapping::w_enemy_mapped(BWAPI::Unit u)
{
        return w_enemy_units[u->getID()];
}
void Unit_Mapping::w_add_enemy_ID(BWAPI::Unit u)
{
        w_enemy_units[u->getID()] = true;
}
void Unit_Mapping::w_del_enemy_ID(BWAPI::Unit u)
{
        w_enemy_units[u->getID()] = false;
}//..

// Mapping Tasks for workers
int Unit_Mapping::get_task(BWAPI::Unit u)// ../
{
        if (!u)
                return -1;
        if ( u->getType().isWorker() )
                return worker_task[u];
        return -1;
}//..

void Unit_Mapping::set_task(BWAPI::Unit u, enum worker_detail task)// ../
{
        if (!u)
                return;
        if ( u->getType().isWorker() )
                worker_task[u] = task;
}//..

//Unit health mapping
int Unit_Mapping::get_unit_shp(BWAPI::Unit u)// ../
{
        return unit_shp[u];
}//..

void Unit_Mapping::set_unit_shp(BWAPI::Unit u)// ../
{
        unit_shp[u] = u->getShields() + u->getHitPoints();
}//..

//Unit under attack map
bool Unit_Mapping::is_under_fire(BWAPI::Unit u)// ../
{
        return under_fire[u].is;
}//..

int Unit_Mapping::last_frame_attacked(BWAPI::Unit u)// ../
{
        return under_fire[u].last_frame;
}//..

void Unit_Mapping::set_under_fire(BWAPI::Unit u)// ../
{
        under_fire[u].is = true;
        under_fire[u].last_frame = Broodwar->getFrameCount();
        worker_task[u] = EVADE;
        set_unit_shp(u);
}//..

void Unit_Mapping::set_not_under_fire(BWAPI::Unit u)// ../
{
        under_fire[u].is = false;
        worker_task[u] = IDLE;
}//..

//Worker defense engagement map
void Unit_Mapping::clear_worker_engage(BWAPI::Unitset enemy)// ../
{
        for ( auto & u : enemy)
        {
                for ( auto & y : worker_engage[u] )
                {
                        worker_task[y] = IDLE;
                        y->stop();
                }
                worker_engage[u].clear();
        }
}//..

void Unit_Mapping::set_worker_engage(BWAPI::Unitset enemy, // ../
                BWAPI::Unitset worker)
{
        Unit to_add;
        for ( auto & u : enemy )
        {
                for ( int i = 0; i < 3; i++)
                {
                        to_add = closest_attacker(u, worker);
                        if (to_add)
                        {
                                worker_task[to_add] = ATTACK;
                                worker_engage[u].insert(to_add);
                        } else { return; }
                }
        }
}//..

BWAPI::Unitset Unit_Mapping::get_defenders(BWAPI::Unit enemy)// ..//
{
        return worker_engage[enemy];
}//..

BWAPI::Unit Unit_Mapping::closest_attacker(BWAPI::Unit enemy, // ../
                BWAPI::Unitset worker)
{
        int distance, closest_dist = 9999999;
        Unit closest = nullptr;

        for ( auto & y : worker )
        {
                if ( worker_task[y] == ATTACK ||
                                worker_task[y] == BUILD ||
                                worker_task[y] == EVADE )
                        continue;
                distance = enemy->getDistance(y);
                if ( distance < closest_dist )
                {
                        closest_dist = distance;
                        closest = y;
                }
        }
        return closest;
}//..

