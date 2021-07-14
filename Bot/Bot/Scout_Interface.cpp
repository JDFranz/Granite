#include "Scout_Interface.h"
#include <BWAPI/AIModule.h>

Scout_Interface::Scout_Interface(vector<BWAPI::Position> path, shared_ptr< Unit_Mapping> map) :
	m_path_togo(path)
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	find_scout(map, false);
	set_dest_path_stupidly(path);
}

Scout_Interface::Scout_Interface(vector<BWAPI::Position> path, shared_ptr<Unit_Mapping> map, bool lost) :
	m_path_togo(path)
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	find_scout(map, lost);
	set_dest_path_stupidly(path);
}

Scout_Interface::~Scout_Interface()
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	//if (m_scout) m_scout->stop();
}

bool Scout_Interface::is_e_discovered()
/**\name:discovered
 * --------------------> status: working
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
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	if (m_path_togo.empty())
	{
		return false;
	}
	float desired_range = 12345; // range of building or creep discovered from
							 // scouting destination that qualifies an
							 // enemy base at that location.
							// 10 build
	bool discovered = false;
	for (BWAPI::Unit u : BWAPI::Broodwar->getUnitsInRectangle(0, 0, 99999, 99999, (BWAPI::Filter::IsEnemy&&
		BWAPI::Filter::IsBuilding)))
	{
		if (distance(u->getPosition(), m_path_togo.back()) <= desired_range)
		{
			return true;
		}
		//"iterate over tiles in desired_range of m_path_togo.last_element"

		if (BWAPI::Broodwar->hasCreep(u->getTilePosition())) //if there is creep then there must be a nearby tumor and we have discovered the enemy
			return true;
	}
}

bool Scout_Interface::is_at_final_dest()
/**\name:at_final_dest
 * --------------------> status: working
 * \call : multiple
 * \brief : check if scout can see final destination of its assigned path
 * \detail : /
 * \return : bool check if scout can see final destination of its assigned path
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	if (m_path_togo.empty())
	{
		if (Scout_Debugging) std::cout << "-> Add or remove Scout" << endl;
		return true;
	}
	if (is_at_destination() && m_path_togo.empty()) return true;
	return false;
}

bool Scout_Interface::move(shared_ptr< Unit_Mapping> map)
/**\name:move
 * --------------------> status:working
 * \call : when scout arrives at a destination
 * \brief :
 * \detail :
 * \param : map when a scout is killed find one
 * \return : bool returns true if at final dest
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	if (!m_scout)scout_killed(map);
	if (is_at_final_dest())return true;
	if (is_at_destination()) m_path_togo.erase(m_path_togo.begin());
	if (!m_path_togo.empty())SmartMove(m_scout, (*m_path_togo.begin()));

	return true;
}
void Scout_Interface::set_dest_path_stupidly(vector<BWAPI::Position> path)
/**\name:set_dest_path_stupidly
 * --------------------> status: working
 * \call : on constructor
 * detail : !!! you can't use this in a constructor before pre assigning path and iterator
 * \brief : set the first path element of a scout to the first element of a path
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	if (Scout_Debugging)
	{
		if (path.empty()) cout << "<== Path is empty" << endl;
		for (auto waypt : path)
		{
			std::cout << waypt.x << " " << waypt.y << "-->";
			if (waypt == BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()))
			{
				cout << "<== THIS IS G STARTLOC" << endl;
			}
		}
		std::cout << std::endl;
	}
	if (path.empty()) delete this;
	m_path_togo = path;
}

void Scout_Interface::set_dest_path_smartly(vector<BWAPI::Position> path) //why? I think it is too complicated
/**\name:set_dest_path
 * --------------------> status: working
 * \call : when a new scout is needed it has to have a path
 * \brief : find the element of path, which is closest to the new scout, let him start scouting there by setting the iterator to that element.
 * \detail : !!! you can't use this in a constructor before pre assigning path and iterator
 * \param : path the path a scout needs to complete
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	if (Scout_Debugging)
	{
		for (auto waypt : path)
		{
			std::cout << waypt.x << " " << waypt.y << "-->";
		}
		std::cout << std::endl;
	}
	vector<int> distances;
	//creating a vector of distances
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
			m_path_togo.begin() = it;
	}
}
vector<BWAPI::Position> Scout_Interface::get_dest_path()
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	return m_path_togo;
}

BWAPI::Unit Scout_Interface::get_scout()
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	return m_scout;
}
void Scout_Interface::scout_killed(shared_ptr< Unit_Mapping> map)
// Finds new scout
{
	if (Scout_Debugging) std::cout << "Scout_Interface::scout_killed" << std::endl;
	//for debugging purposes change in "Debugger.hpp"
	find_scout(map, false);
}

void Scout_Interface::find_scout(shared_ptr< Unit_Mapping> map, bool far_from_g_base)
/**\name:find_scout
 * --------------------> status: Debugging
 * \call : everytime a new scout is needed
 * \brief : Scans through all the workers and makes an IDLE worker a Scout
 * \detail :looks for the Unit with the closest position to the iterator(position) makes that one the scout
 * \param : map
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	float min_dist = 99999;
	BWAPI::Unit scout = nullptr; // setting the scout to nullptr to assume dead till found

	for (auto& u : BWAPI::Broodwar->getUnitsInRectangle(0, 0, 99999, 99999, BWAPI::Filter::IsOwned))//BWAPI::Filter::IsWorker &&
	{
		if (!(u->getType().isWorker())) continue; //check if unit is worker

		if (map->get_task(u) == BUILD || map->get_task(u) == HARVEST) //that means every other worker has to be sorted beforehand
			continue;
		if (map->get_task(u) == SCOUT) continue; // we would be reassigning a scout however we want to add

		if (far_from_g_base) // we just want to have lost scouts
		{
			if (u->getDistance(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation())) < 5000) //we are not looking for workers within a certain proximity
			{
				continue;
			}
		}
		//"Find the element of m_path_togo that scout is closest to";
		//"set iterator to that element";
		for (auto it = m_path_togo.begin(); it != m_path_togo.end(); it++)
		{
			float dist = distance(u->getPosition(), (*it));
			if (dist < min_dist) // if the unit is within a certain proximity of the scouting path end then this must be the scouting unit
			{
				//found the scout
				min_dist = dist;
				m_scout = u;
			}
		}
	}

	if (!m_scout)
	{
		if (Scout_Debugging) std::cout << "--->Scout not found" << std::endl;
		return;
	}

	if (m_scout)
	{
		if (Scout_Debugging) std::cout << "--->Scout is found" << std::endl;
		// if there is no scout near any of the path positions
				// for later shouldn't we find a new one?
		//if (Unit_Mapping_Debugging)map->print();
		map->set_task(m_scout, SCOUT); // set the task for this unit as Scouting
	}
}

// At Destination?../
bool Scout_Interface::is_at_destination()
/**\name:at_destination
 * --------------------> status: working
 * \call : update_scout_path
 * \brief : checks whether a this scout can see a destination
 * \return : bool
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	if (is_waypoint_in_sight())
		return true;
	return false;
}
bool Scout_Interface::is_waypoint_in_sight()
/**\name:in_range
 * --------------------> status: working
 * \call : at_destination ,
 * \brief : checks whether a probe scouter can see a position.
 * \detail :/
 * \return : bool whether insight
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	if (Scout_Debugging)
	{
		cout << "	current waypoint: " << (*m_path_togo.begin()).x << " " << (*m_path_togo.begin()).y << std::endl;
		std::cout << "	Scout pos: " << (m_scout->getPosition()).x << " " << (m_scout->getPosition()).y << std::endl;
	}

	if (!m_scout) return false;
	// sightrange of a protoss probe is 8 tiles so 8*32= 256 pixels? ASK
	if (m_scout->getDistance((*m_path_togo.begin())) <= 256)
	{
		if (Scout_Debugging) std::cout << "		->can see waypoint" << std::endl;
		m_scout->stop();
		return true;
	}
	return false;
}

float Scout_Interface::distance(BWAPI::Position pos1, BWAPI::Position pos2)
/**\name:distance
 * --------------------> status: working
 * \call :multiple
 * \brief : calculates distance between two positions
 * \detail :/
 * \param : pos1
 * \param : pos2
 * \return : float the distance
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	return pos1.getDistance(pos2);
}

void Scout_Interface::SmartMove(BWAPI::Unit attacker, const BWAPI::Position& targetPosition)
/**\name:SmartMove
 * --------------------> status: t
 * \call : ::move
 * \brief : probably to evade attacker
 * \detail :
 * \param : attacker
 * \param : targetPosition
 */
{
	if (Scout_Debugging) std::cout << "Scout_Interface::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
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

	//  if ()
	//  {
	//      BWAPI::Broodwar->drawCircleMap(attacker->getPosition(), dotRadius, BWAPI::Colors::White, true);
	//      BWAPI::Broodwar->drawCircleMap(targetPosition, dotRadius, BWAPI::Colors::White, true);
	//      BWAPI::Broodwar->drawLineMap(attacker->getPosition(), targetPosition, BWAPI::Colors::White);
	// }
}