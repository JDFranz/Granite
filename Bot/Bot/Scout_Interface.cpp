#include "Scout_Interface.h"
#include <BWAPI/AIModule.h>

Scout_Interface::Scout_Interface(list<BWAPI::Position> path, Unit_Mapping* map)
{
	set_dest_path(path);
	find_scout(map);
}

Scout_Interface::~Scout_Interface()

{
	if (m_scout)
		m_scout->stop();
}

bool Scout_Interface::discovered(Unit_Mapping* m_map)
/**\name:discovered
 * --------------------> status: debugging
*\call : on every update
*\brief : determines whether the enemy is found by ckecking
* whether an enemy is sighted within a certain proximity
* of a base location :
*->we are trying to avoid recognising fake bases.
*\detail :
*\param : m_map
*\return : bool whether it determines an enemy base
*
 */
{
	float desired_range = 12345; // range of building or creep discovered from
							 // scouting destination that qualifies an
							 // enemy base at that location.
							// 10 build
	bool discovered = false;
	for (BWAPI::Unit u : BWAPI::Broodwar->getUnitsInRectangle(0, 0, 99999, 99999, (BWAPI::Filter::IsEnemy&&
		BWAPI::Filter::IsBuilding)))
	{
		if (distance(u->getPosition(), m_path.back()) <= desired_range)
			return true;
		//"iterate over tiles in desired_range of m_path.last_element"

		if (BWAPI::Broodwar->hasCreep(u->getTilePosition())) //if there is creep then there must be a nearby tumor and we have discovered the enemy
			return true;
	}
}

bool Scout_Interface::at_final_dest()
/**\name:at_final_dest
 * --------------------> status:
 * \call : multiple
 * \brief : check if scout can see final destination of its assigned path
 * \detail : /
 * \return : bool check if scout can see final destination of its assigned path
 */
{
	if (at_destination() && (m_iterator == m_path.end()))
		return true;
	return false;
}

bool Scout_Interface::move(Unit_Mapping* map)
/**\name:move
 * --------------------> status:debugging
 * \call : when scout arrives at a destination
 * \brief :
 * \detail :
 * \param : map when a scout is killed find one
 * \return : bool returns true if at final dest
 */
{
	if (!m_scout)scout_killed(map);
	if (at_final_dest())return true;
	if (at_destination()) m_iterator++;
	SmartMove(m_scout, (*m_iterator));
	return true;
}
void Scout_Interface::set_dest_path(list<BWAPI::Position> path)
/**\name:set_dest_path
 * --------------------> status: debugging
 * \call : when a new scout is needed it has to have a path
 * \brief : find the element of path, which is closest to the new scout, let him start scouting there by setting the iterator to that element.
 * \detail :
 * \param : path the path a scout needs to complete
 */
{
	list<int> distances;
	//creating a list of distances
	for (auto it = path.begin(); it != path.end(); it++)
	{
		distances.push_back(m_scout->getDistance((*it)));
	}

	int mindist = 99999;
	//finding  minimum of distances
	for (auto dist1 : distances)
	{
		for (auto dist2 : distances)
		{
			if (dist1 <= dist2 && dist1 < mindist)
			{
				mindist = dist1;
			}
		}
	}
	//set the iterator to that element
	for (auto it = path.begin(); it != path.end(); it++)
	{
		if (mindist == (m_scout->getDistance((*it))))
			m_iterator = it;
	}
}
list<BWAPI::Position> Scout_Interface::get_dest_path()
{
	return m_path;
}
BWAPI::Unit Scout_Interface::get_scout()
{
	return m_scout;
}
void Scout_Interface::scout_killed(Unit_Mapping* map)
// Finds new scout
{
	find_scout(map);
}

void Scout_Interface::find_scout(Unit_Mapping* map)
{
	float dist;
	float min_dist = 99999;
	BWAPI::Unit scout = nullptr; // setting the scout to nullptr to assume dead till found

	for (auto& u : BWAPI::Broodwar->getUnitsInRectangle(0, 0, 99999, 99999, BWAPI::Filter::IsOwned))//BWAPI::Filter::IsWorker &&
	{
		if (!u->getType().isWorker()) continue; //check if unit is worker

		if (map->get_task(u) == BUILD || map->get_task(u) == HARVEST) //that means every other worker has to be sorted beforehand
			continue;

		//"Find the element of m_path that scout is closest to";
		//"set iterator to that element";
		for (auto it = m_path.begin(); it != m_path.end(); it = it)
		{
			dist = distance(u->getPosition(), (*it));
			if (dist < min_dist) // if the unit is within a certain proximity of the scouting path end then this must be the scouting unit
			{
				//found the scout
				min_dist = dist;
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
/**\name:at_destination
 * --------------------> status: debugging
 * \call :
 * \brief :
 * \detail :
 * \return : bool
 */
{
	if (waypoint_in_sight())
		return true;
	return false;
}
bool Scout_Interface::waypoint_in_sight()
/**\name:in_range
 * --------------------> status: Debugging
 * \call : at_destination ,
 * \brief : checks whether a probe scouter can see a position.
 * \detail :/
 * \return : bool whether insight
 */
{
	if (!m_scout)
		return false;
	// sightrange of a protoss probe is 8 tiles so 8*32= 256 pixels? ASK
	if (m_scout->getDistance((*m_iterator)) <= 256)return true;
	return false;
}

float Scout_Interface::distance(BWAPI::Position pos1, BWAPI::Position pos2)
/**\name:distance
 * --------------------> status: Debugging
 * \call :multiple
 * \brief : calculates distance between two positions
 * \detail :/
 * \param : pos1
 * \param : pos2
 * \return : float the distance
 */
{
	return pos1.getDistance(pos2);
}

void Scout_Interface::SmartMove(BWAPI::Unit attacker, const BWAPI::Position& targetPosition)
/**\name:SmartMove
 * --------------------> status: TODO
 * \call :
 * \brief : probably to evade attacker
 * \detail :
 * \param : attacker
 * \param : targetPosition
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