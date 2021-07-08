#include "Scouting.h"

//Public
// Scout for enemy start location../
void Scouting::scout_start_loc(Unit_Mapping* map)
        /*
         * update - enemy_loc     -> if 2 player map... set coords and return.
         *
         *        or
         *
         *          m_scout_paths -> Initialize with starting locations
         *                           (these are single coordinate paths)
         *
         *          m_temps       -> Initialize for each path
         *
         * idea - if not a 2 player map add scout destinations
         *
         */
{
        if ( "two_player_map" )
        {
                parse_start_loc(map);
                return;
        }

        for (auto &u : BWAPI::Broodwar->getStartLocations())
        {
                if ("u == BWAPI::Broodwar->self()->getStartLocation()")
                        continue;
                m_scout_paths.push_back(u);
                m_scout_temps.push_back(u);
        }
}//..

// Update Scout Interface../
void Scouting::update_scout_interface(Unit_Mapping* map)
        /*
         * update -  scouts             -> add if there is no scout
         *                              -> remove if scouting complete   
         *
         *           scout_paths        -> removes paths that have been
         *                                 assigned to scout.
         *
         * idea - This update ensures that scouting is being carried out
         *
         */
{
        // return if there is nothing to scout for
        if (m_scout_paths.isEmpty() && m_scouts.isEmpty())
                return;

        // add a scout if no one is scouting
        if (m_scouts.isEmpty())
                add_scout();

        // update scouts if they discovered enemy or arrived at destination
        for (auto &u : m_scouts)
        {
                if (u->discovered())
                        on_discovery(u, map);
                if (u->at_final_dest())
                        on_arrived(u, map);
        }

        // order scouts to move
        for (auto &u : m_scouts)
        {
                u->move();
        }
}//..

// Replace If Scout ../
void Scouting::replace_if_scout(BWAPI::Unit unit, Unit_Mapping * map)
        /*
         * Called by - onDestroyed
         *
         * update - scout_interface object      -> replace m_scout
         *
         * idea - calls scout_killed if the destroyed unit was a scout
         */
{
        for ( auto &u : m_scouts )
                if ( u->get_scout() == unit )
                        u->scout_killed(map);
}//..

//Private
// Parse Start Location../
void Scouting::parse_start_loc(Map_Interface * map)
        /*
         * updates - m_map      -> adds enemy base to m_map
         * 
         * idea - element of 2 item list that is not our start location
         */
{
        BWAPI::TilePosition::list starts;
        BWAPI::TilePosition our_start;
        BWAPI::TilePosition enemy_location;

        starts = BWAPI::Broodwar->getStartLocations();
        our_start = BWAPI::Broodwar->self()->getStartLocation();
        
        enemy_location = "element in starts that is not our_start";

        map->add_enemy_base(BWAPI::Position(enemy_location));
} //..

// On Arrived../
void Scouting::on_arrived(Scout_Interface scout, Map_Interface * map)
        /*
         * update - scout object       -> new destination path
         *          m_scouts           -> add a scout of there are more
         *                                destination paths
         */
{
        update_scout_path(scout);
        add_scout();
}//..

// On Discovery../
void Scouting::on_discovery(Scout_Interface * scout, Map_Interface * map)
        /*
         * update - scout object       -> new destination path
         *          m_scouts           -> add a scout if there are more
         *                                destination paths remove scout if
         *                                none
         *          m_map              -> add enemy base location
         * 
         *          idea - Check m_scout_paths for more paths
         *                      if one exists assign it to the scout
         *                      else remove scout from list and set it to IDLE
         *                 if another path exists add a new
         *                 scout and assign it the path
         */
{
        // record discovery
        map->add_enemy_base(scout->get_final_dest());

        // remove temporary paths
        for (auto &u : m_scout_paths)
        {
                if (in_scout_temps(u))
                        m_scout_paths.remove(u);
        }
        m_scout_temps.clear();

        update_scout_path(scout);

        // halt temporary scouts
        for (auto &u : m_scouts)
        {
                if (u->is_temp())
                {
                        m_scouts.remove(u);
                        delete(u)
                }
        }
}//..

// Update scout path../
void Scouting::update_scout_path( Scout_Interface * scout )
{
        ds element;
        bool temp;

        if ( !m_scout_paths.isEmpty() )
        {
                element = closest_path(scout->get_scout());
                temp = in_scout_temps(element);

                scout->set_dest_path(elemebt);
                m_scout_paths.remove(element);
        } else {
                map->set_task(scout->get_scout(), IDLE);
                m_scouts.remove(scout);
                delete(scout);
        }
}//..

// Add Scout../
void Scouting::add_scout()
{
        ds element;
        bool temp;

        // add another scout if more paths exist
        if ( !m_scout_paths.isEmpty() )
        {
                element = m_scout_paths.next_element;
                temp = in_scout_temps(element);

                m_scouts.add(new Scout_Interface(element, map, temp));
                m_scout_paths.remove(element);
        }
}//..

// In m_scout_temps?../
bool Scouting::in_scout_temps(ds element)
{
        if (m_scout_temps.contains(element))
                return true;
        return false;
}//..
         
// Closest Path../
ds Scouting::closest_path(BWAPI::Unit scout)
{
        BWAPI::Position scout_pos;

        scout_pos = scout->getPosition();

        "return m_scout_path that has closest last_element to scout_pos"
}//..

