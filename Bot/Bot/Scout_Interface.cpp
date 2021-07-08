#include "Scout_Interface.h"


Scout_Interface::Scout_Interface(list<BWAPI::Point> path, Unit_Mapping * map, bool temp)
        :
                m_temp(temp)
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

        for (auto &u : BWAPI::Broodwar->getUnitsInRectangle(0,0,99999,99999, BWAPI::Filter::IsEnemy, BWAPI::Filter::IsBuilding))
                if (distance(u->getPosition(),m_path.back()) <= desired_range)
                        return true;
        "iterate over tiles in desired_range of m_path.last_element"
                if (BWAPI::Broodwar->hasCreep(tile))
                        return true;
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
        if (!m_scout)
                scout_killed(map);

        if (at_final_dest())
                return;
        if (at_destination())
                "m_iterator++";

        smartMove(m_scout, (*m_iterator));
}//..

// Set Destination Path../
void Scout_Interface::set_dest_path(list<BWAPI::Point> path)
        //set private class variable for scout's path
{
        m_path = path;
       //TODO "find closest element of m_path to scout->getPosition()"
       //TODO "set iterator at that element"
}//..

// Get Destination Path../
list<BWAPI::Point> Scout_Interface::get_dest_path()
        //get private class variable for scout's path
{
        return m_path;
}//..

// Get Scout../
BWAPI::Unit Scout_Interface::get_scout()
{
        return m_scout;
}//..

// Scout Killed../
void Scout_Interface::scout_killed(Unit_Mapping * map)
        // Finds new scout
{
        find_scout(map);
}//..

// Is Temp?../
bool Scout_Interface::is_temp()
{
        return m_temp;
}//..

// Private
// Find Scout../
void Scout_Interface::find_scout(Unit_Mapping * map)
{
        float dist;
        float min_dist = 99999;
        BWAPI::Unit scout = nullptr;

        for (auto &u : BWAPI::Broodwar->getUnitsInRectangle(0,0,99999,99999, Filter::IsWorker && Filter::IsOwned))
        {
                if (map->get_task(u) == BUILD || map->get_task(u) == HARVEST)
                        continue;

                dist = distance(u->getPosition(),(* m_path.end()));
                if (dist < min_dist)
                {
                        min_dist = dist;
                        scout = u;
                }
        }

        if (!scout)
                return;

        m_scout = scout;
        //TODO map->set_task(scout) == SCOUT; 

        "Find the element of m_path that scout is closest to";
        "set iterator to that element";
        }
}//..

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
        if (in_range((*m_iterator)))
                return true;
        return false;
}//..

// In Range../
bool Scout_Interface::in_range()
{
        float desired_range = ?; // Distance from a discovered building to
                                 // the destination scout point where we can
                                 // accurately say that the enemy has a base
                                 // at that location
                                 
        if (!m_scout)
                return false;

        if (distance(m_scout->getPosition(), m_path[m_iterator]) <= desired_range)
                return true;
        return false;
}//..

// Distance../
float Scout_Interface::distance(BWAPI::Position, BWAPI::Position)
{
        "calculate distance between {x1, y1} and {x2, y2}"
}//..

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
    if (attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || attacker->isAttackFrame())
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

    if (Config::Debug::DrawUnitTargetInfo)
    {
        BWAPI::Broodwar->drawCircleMap(attacker->getPosition(), dotRadius, BWAPI::Colors::White, true);
        BWAPI::Broodwar->drawCircleMap(targetPosition, dotRadius, BWAPI::Colors::White, true);
        BWAPI::Broodwar->drawLineMap(attacker->getPosition(), targetPosition, BWAPI::Colors::White);
    }
}//..

