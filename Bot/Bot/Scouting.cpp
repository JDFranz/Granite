#include "Scouting.h"

//Public
// Scout for enemy start location../
void Scouting::scout_start_loc(Unit_Mapping* map) //DONE
        /*
         * update - enemy_loc     -> if 2 player map... set coords and return.
         *
         *        or
         *
         *          m_scout_total_paths -> Initialize with starting locations
         *                           (these are single coordinate paths)
         *
         *          m_temps       -> Initialize for each path
         *
         * idea - if not a 2 player map add scout destinations
         *
         */
{
        if (size(map->e_get_potential_startpositions())==1) //
        {
             e_iscovered_enemy = true;
             map->add_enemy_base(*(map->e_get_potential_startpositions().begin()));
        }

        for (auto &u : map->e_get_potential_startpositions())
        {
                        
                m_scout_total_paths.push_back(u);
                m_scout_remaining_path.push_back(u); 
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
        if (m_scout_total_paths.empty() && m_scouts.empty())
                return;

        // add a scout if no one is scouting
        if (m_scouts.empty())
                add_scout(map);

        // update scouts if they discovered enemy or arrived at destination
        for (auto &u : m_scouts)
        {
                 if (u.discovered(map))
                          on_discovery(&u, map);
                  if (u.at_final_dest())
                          on_arrived(u, map);  
        }

        // order scouts to move
        for (auto &u : m_scouts)
        {
             u.move(map);
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
                if ( u.get_scout() == unit )
                        u.scout_killed(map);
}//..

// On Arrived../
void Scouting::on_arrived(Scout_Interface scout, Unit_Mapping* map)
        /*
         * update - scout object       -> new destination path
         *          m_scouts           -> add a scout of there are more
         *                                destination paths
         */
{
        update_scout_path(&scout,map);
        add_scout(map);
}//..

// On Discovery../
void Scouting::on_discovery(Scout_Interface * scout, Unit_Mapping* map)
        /*
         * update - scout object       -> new destination path
         *          m_scouts           -> add a scout if there are more
         *                                destination paths remove scout if
         *                                none
         *          m_map              -> add enemy base location
         * 
         *          idea - Check m_scout_total_paths for more paths
         *                      if one exists assign it to the scout
         *                      else remove scout from list and set it to IDLE
         *                 if another path exists add a new
         *                 scout and assign it the path
         */
{
        // record discovery
        map->add_enemy_base(scout->get_scout());
        e_iscovered_enemy = true;

        // remove temporary paths
        //for (auto &u : m_scout_total_paths)
        for (auto it_path=m_scout_total_paths.begin(); it_path !=m_scout_total_paths.end(); it_path = it_path)
        {
            for (auto it_temp = m_scout_remaining_path.begin(); it_temp != m_scout_remaining_path.end(); it_temp = it_temp)
                if ((*it_path) == (*it_temp)) it_path = m_scout_total_paths.erase(it_path);
                else it_path++;
        	
        }
        m_scout_remaining_path.clear();

        update_scout_path(scout,map);

        // halt temporary scouts
        for (auto &u : m_scouts)
        for (auto it_scouts = m_scouts.begin(); it_scouts != m_scouts.end(); it_scouts = it_scouts)

        {
            if ((*it_scouts).is_temp())
            {
                it_scouts = m_scouts.erase(it_scouts);
            }
            else it_scouts++;
        }
}

void Scouting::update_scout_path( Scout_Interface * scout ,Unit_Mapping* map)
{
        /* @CALL: When a Scouting_unit arrives at a way point of it's path
         *
         * @Brief:
         * are there segments of paths left for scouting?
         * ->YES:
         * find the closes path for the scout from the remainder of scoutpaths
         * mark the first waypoint as done by popping it off ( the scout is at this waypoint)
         * set the remaining path to be the scout's path 
         * 
         * ->NO: (we have scouted this route)
         * clear and delete the scout form our scoutingunitlist and make IDLE
         */

        if ( !m_scout_remaining_path.empty() )  
        {
				list<BWAPI::Position>  path = closest_path(scout->get_scout());
                scout->set_dest_path(path);
                m_scout_remaining_path.pop_back();
        } else { //there are no more paths to scout
                map->set_task(scout->get_scout(), IDLE);
                m_scouts.erase((std::find(m_scouts.begin(), m_scouts.end(), scout)));
                delete(scout);
        }
}
void Scouting::add_scout(Unit_Mapping* map )
{
	/* if there is a remaining to scout:
	 * let Scoutinterface create a new interface. Scoutinterface will:
	 * -> find a unit near a path and make that unit a scout(handled by Scoutinterface)
	 * -> set the path for the unit to be a remaining path to scout
	 */
        if ( !m_scout_total_paths.empty() )
        {
                m_scouts.push_back( Scout_Interface(m_scout_remaining_path, map));
        }
}


bool Scouting::in_remaining_path(BWAPI::Position position)
{
        if ((std::find(m_scout_remaining_path.begin(), m_scout_remaining_path.end(), position) != m_scout_remaining_path.end()))//(m_scout_remaining_path.contains(element))
                return true;
        return false;
}
         

list<BWAPI::Position> Scouting::closest_path(BWAPI::Unit scout)
{   
        BWAPI::Position scout_pos = scout->getPosition();
		float dist=scout_pos.getDistance((m_scout_remaining_path.back()));
		
        return m_scout_total_paths;//TODO list of lists
}



