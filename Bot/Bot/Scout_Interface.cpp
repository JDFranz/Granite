#include "Scout_Interface.h"


Scout_Interface::Scout_Interface(list<BWAPI::Position> path, Unit_Mapping * map)
{
        set_dest_path(path);
        find_scout(map);
}



Scout_Interface::~Scout_Interface()
{
        if (m_scout)
                m_scout->stop();
}

// Public
// Discovered?../
bool Scout_Interface::discovered(Unit_Mapping * m_map)

        /*
         * output - true or false
         *
         * idea   - Enemy is considered discovered if a building or zerg creep 
         *          is visible within a certain radius of the final destination
         */

{
        float desired_range = 12345; // range of building or creep discovered from
                                 // scouting destination that qualifies an
                                 // enemy base at that location.
        //                       // 10 build
        //TODO
        // for (auto &u : BWAPI::Broodwar->getUnitsInRectangle(0,0,99999,99999, BWAPI::Filter::IsEnemy, BWAPI::Filter::IsBuilding))//and the filter
        // 
        //         if (distance(u->getPosition(),m_path.back()) <= desired_range)
        //                 return true;
        // "iterate over tiles in desired_range of m_path.last_element"
        //         if (BWAPI::Broodwar->hasCreep(u))
        //                 return true;
        return false;

}//..

// At Final Destination?../
bool Scout_Interface::at_final_dest()
        /*
         * output - true or false
         *
         * idea - True when scout is within sight range of dest
         *                  and iterator is at the last dest in path
         *
         */
{
        if (at_destination() && (m_iterator ==m_path.end()))
                return true;
        return false;
}//..

// Move../
bool Scout_Interface::move(Unit_Mapping * map)
        /*
         * update - iterator            -> increase if at destination
         *          m_scout             -> Right click the destination
         */ 
{
        // if (!m_scout) TODO
        //         scout_killed(map);
        //
        // if (at_final_dest())
        //         return;
        // if (at_destination())
        //         "m_iterator++";
        //
        // SmartMove(m_scout, (*m_iterator));
    return false;
}//..

// Set Destination Path../
void Scout_Interface::set_dest_path(list<BWAPI::Position> path)
        //set private class variable for scout's path
{
        m_path = path;
       //TODO "find closest element of m_path to scout->getPosition()"
       //TODO "set iterator at that element"
}
list<BWAPI::Position> Scout_Interface::get_dest_path()
        //get private class variable for scout's path
{
        return m_path;
}
BWAPI::Unit Scout_Interface::get_scout()
{
        return m_scout;
}
void Scout_Interface::scout_killed(Unit_Mapping * map)
        // Finds new scout
{
        find_scout(map);
}

void Scout_Interface::find_scout(Unit_Mapping * map)
{
        float dist;
        float min_dist =99999 ;
        BWAPI::Unit scout = nullptr; // setting the scout to nullptr to assume dead till found

        for (auto &u : BWAPI::Broodwar->getUnitsInRectangle(0,0,99999,99999, BWAPI::Filter::IsOwned))//BWAPI::Filter::IsWorker && 
        {

            if (!u->getType().isWorker()) continue; //check if unit is worker
        	
            if (map->get_task(u) == BUILD || map->get_task(u) == HARVEST) //that means every other worker has to be sorted beforehand
                    continue;

        	
            //"Find the element of m_path that scout is closest to";
			//"set iterator to that element";
        	for (auto it=m_path.begin(); it!= m_path.end(); it=it)
        	{
                dist = distance(u->getPosition(), (*it));
                if (dist < min_dist) // if the unit is within a certain proximity of the scouting path end then this must be the scouting unit 
                {
                	//found the scout
                    min_dist = dist ; //+10 to leave a certain threshold
                    m_scout = u;
                }
        	}
        }

        if (!scout)
                return; // if there is no scout near any of the path positions
						// for later shouldn't we find a new one?
        
        map->set_task(m_scout, SCOUT); // set the task for this unit as Scouting 

        
}

// At Destination?../
bool Scout_Interface::at_destination()
        /*
         * update - iterator if true;
         *
         * output - true or false
         *
         * idea - if scout is within sight range of dest return true
         */
{
        // if (in_range((*m_iterator))) //in range determines wether the unit is on 
        //         return true;
        // return false;
	//TODO
    return true;
}//..

// In Range../
bool Scout_Interface::in_range()
{
	//
        float desired_range = 1000; // Distance from a discovered building to
                                 // the destination scout point where we can
                                 // accurately say that the enemy has a base
                                 // at that location
                                 
        if (!m_scout)
                return false;

	
        if (m_scout->getDistance((*m_iterator))<=desired_range)return true;
        //if (distance(m_scout->getPosition(), (*m_iterator)) <= desired_range)
                
        return false;
}//..

// Distance../
float Scout_Interface::distance(BWAPI::Position pos1, BWAPI::Position pos2)
{
    return pos1.getDistance(pos2);
       //"calculate distance between {x1, y1} and {x2, y2}"
}

// Smart Move../
void Scout_Interface::SmartMove(BWAPI::Unit attacker, const BWAPI::Position & targetPosition)
        /*
         * From UAlbertaBot by Dave Churchill
         *
         * idea - move the unit... smartly
         */
{

    if (!attacker || !targetPosition.isValid())
    {
        return;
    }

    // if we have issued a command to this unit already this frame, ignore this one
    if ((attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount()) || attacker->isAttackFrame())
    {
        return;
    }

    // get the unit's current command
    BWAPI::UnitCommand currentCommand(attacker->getLastCommand());

    // if we've already told this unit to move to this position, ignore this command
    if ((currentCommand.getType() == BWAPI::UnitCommandTypes::Move) && (currentCommand.getTargetPosition() == targetPosition) && attacker->isMoving())
    {
        return;
    }

    // if nothing prevents it, attack the target
    attacker->move(targetPosition);

    //  if (Config::Debug::DrawUnitTargetInfo)
    //  {
    //      BWAPI::Broodwar->drawCircleMap(attacker->getPosition(), dotRadius, BWAPI::Colors::White, true);
    //      BWAPI::Broodwar->drawCircleMap(targetPosition, dotRadius, BWAPI::Colors::White, true);
    //      BWAPI::Broodwar->drawLineMap(attacker->getPosition(), targetPosition, BWAPI::Colors::White);
    // }
}


