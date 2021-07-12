#include "Scouting.h"
using namespace std;

void Scouting::scout_start_loc(Unit_Mapping* map)
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
	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	auto e_pot_pos = map->e_get_potential_startpositions();

	vector<BWAPI::Position> path1;
	vector<BWAPI::Position> path2;

	if (size(e_pot_pos) == 1) // one other startlocation-> enemy will be there
	{
		e_basediscovered = true;
		path1.push_back(*(e_pot_pos.begin()));
		map->add_enemy_base(*(e_pot_pos.begin()));
		m_scout_total_paths.push_back(path1);
		m_scout_not_assigned_paths.push_back(path1);
		return;
	}
	else
	{ // we assume there is an infinite number of e_pot_baselocs but we want to assign two scouts maximum
		//get distances
		int num_pos = 0;
		for (auto& u : e_pot_pos)
		{
			if (num_pos == 0 || num_pos == 1) path1.push_back(u);
			else path2.push_back(u);
			num_pos++;
		}
		m_scout_total_paths.push_back(path2);
		m_scout_total_paths.push_back(path1);

		m_scout_not_assigned_paths.push_back(path2);
		m_scout_not_assigned_paths.push_back(path1);
		Phase = Scouting_phases::INITIAL_SCOUTING;

		if (Scout_Debugging) std::cout << "	->num of paths: " << size(m_scout_total_paths) << std::endl;
		// preload the first path into m_scout_assigned_paths add a scout
		add_scout(map);

		/*TODO: order the path to have efficient pathfinding
		m_scout_assigned_paths.push_back(path2);
		m_scout_assigned_paths.push_back(path1);
		// find out min and max distance
		int min_dist = 99999;
		int max_dist = 0;
		BWAPI::Position g_loc = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
		for (auto& pos : pot_loc)
		{
			for (auto& comp_pos : pot_loc)
			{
				if (min_dist < pos.getDistance(g_loc))
				{
					if (pos.getDistance(g_loc) < comp_pos.getDistance(g_loc))//
					{
						//if min_dist is smaller than all of
						min_dist = pos.getDistance(g_loc);
					}
				}

				if (max_dist > pos.getDistance(g_loc))
				{
					if (pos.getDistance(g_loc) > comp_pos.getDistance(g_loc))
					{
						//if max_dist is larger than the other distances to g_base  make that the next max_dist
						max_dist = pos.getDistance(g_loc);
					}
				}
			}
		}*/
	}
}

void Scouting::print()
{
	cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
	cout << "m_scout_assigned_paths" << endl;
	for (auto path : m_scout_assigned_paths)
	{
		for (auto waypoint : path)
		{
			cout << " -> " << waypoint.x << " " << waypoint.y;
			if (waypoint == BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()))
			{
				cout << "<== THIS IS G STARTLOC" << endl;
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << "m_scout_not_assigned_paths" << endl;
	for (auto path : m_scout_not_assigned_paths)
	{
		for (auto waypoint : path)
		{
			cout << " -> " << waypoint.x << " " << waypoint.y;
			if (waypoint == BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()))
			{
				cout << "<== THIS IS G STARTLOC" << endl;
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << "m_scout_total_paths" << endl;
	for (auto path : m_scout_total_paths)
	{
		for (auto waypoint : path)
		{
			cout << " -> " << waypoint.x << " " << waypoint.y;
			if (waypoint == BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()))
			{
				cout << "<== THIS IS G STARTLOC" << endl;
			}
		}
		cout << endl;
	}

	cout << "our scouts:" << endl;
	for (auto scout : m_scouts)
	{
		cout << scout.get_scout()->getID() << ", ";
	}
	cout << endl;
	cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
}

void Scouting::onFrame(Unit_Mapping* map)
/**\name:update_scout_interface
 * --------------------> status:debugging
 * \call : by Granitebot class
 * \brief : is exectued on every Frame
 * \detail :
 * nothing to scout for -> do nothing
 * enemy sighted -> call discorvered
 * scouting path ends-> call arrived
 * nothing-> continue scouting
 * \param : map sightings pos of scout etc.
 */
{
	//check Statespace
	if (Phase == Scouting_phases::STARTING) scout_start_loc(map);
	else if (Phase == Scouting_phases::INITIAL_SCOUTING);
	else if (Phase == Scouting_phases::E_BASE_LOCATED);
	else if (Phase == Scouting_phases::E_BASE_SIGHTED)return;
	else if (Phase == Scouting_phases::LATE_GAME)return;
	else cout << "UNDEFINED STATE" << endl;

	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"
	// return if there is nothing to scout for
	if (m_scout_not_assigned_paths.empty() && m_scouts.empty())
		return;

	// add a scout if no one is scouting
	if (m_scouts.empty())
		add_scout(map);

	// update scouts if they discovered enemy or arrived at destination

	for (auto& u : m_scouts)
	{
		if (u.is_e_discovered(map))
		{
			on_e_base_discovery(&u, map);
			return;
		}
		if (u.at_final_dest())
		{
			on_arrived_at_final_dest(u, map);
		}
	}

	// order scouts to move
	for (auto& u : m_scouts)
	{
		u.move(map);
	}
	if (Scout_Debugging) print();
}

void Scouting::replace_if_scout(BWAPI::Unit unit, Unit_Mapping* map)
/**\name:replace_if_scout
 * --------------------> status: debugging
 * \call : onDestroyed
 * \brief : tracks the location of scout death
 * \param : unit f the unit that was killed was a scout track it on map
 * \param : map
 */

{
	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	for (auto& u : m_scouts)
	{
		if (u.get_scout()->getID() == unit->getID()) u.scout_killed(map);
	}
}

void Scouting::on_arrived_at_final_dest(Scout_Interface scout, Unit_Mapping* map)
/**\name:on_arrived
 * --------------------> status:
 * \call : when a scout arrived at the end of a path
 * \brief : if a scout arrives at the end of a path, and has not discovered an enemy add another scout -> exponential search
 * \detail :
 * \param : scout:  the scout who arrived at the end of a path
 * \param : map: getting a new scout, getting new path
 */
{
	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	update_scout_path(&scout, map);
	add_scout(map);
}

void Scouting::on_e_base_discovery(Scout_Interface* scout, Unit_Mapping* map)
/*
 * update - scout object       -> new destination path
 *          m_scouts           -> add a scout if there are more
 *                                destination paths remove scout if
 *                                none
 *          m_map              -> add enemy base location
 *
 *          idea - Check m_scout_total_paths for more paths
 *                      if one exists assign it to the scout
 *                      else remove scout from vector and set it to IDLE
 *                 if another path exists add a new
 *                 scout and assign it the path
 */
{
	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	// record discovery
	map->add_enemy_base(scout->get_scout());
	e_basediscovered = true;

	// remove temporary paths
	//for (auto &u : m_scout_total_paths)
	for (auto it_path = m_scout_total_paths.begin(); it_path != m_scout_total_paths.end(); it_path = it_path)
	{
		for (auto it_temp = m_scout_assigned_paths.begin(); it_temp != m_scout_assigned_paths.end(); it_temp = it_temp)
		{
			if ((*it_path) == (*it_temp)) it_path = m_scout_total_paths.erase(it_path);
			else it_path++;
		}
	}
	m_scout_assigned_paths.clear();

	update_scout_path(scout, map);

	// halt temporary scouts
	for (auto& u : m_scouts)
		for (auto it_scouts = m_scouts.begin(); it_scouts != m_scouts.end(); it_scouts = it_scouts)

		{
			if ((*it_scouts).at_final_dest())
			{
				map->set_task((*it_scouts).get_scout(), IDLE);
				it_scouts = m_scouts.erase(it_scouts);
			}
			else it_scouts++;
		}

	Phase = Scouting_phases::E_BASE_SIGHTED;
}

void Scouting::update_scout_path(Scout_Interface* scout, Unit_Mapping* map)
{
	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

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
	 * clear and delete the scout form our scoutingunitvector and make IDLE
	 */

	if (!m_scout_assigned_paths.empty())
	{
		vector<BWAPI::Position>  path = closest_path(scout->get_scout());
		scout->set_dest_path_stupidly(path);
		m_scout_assigned_paths.pop_back();
	}
	else { //there are no more paths to scout
		map->set_task(scout->get_scout(), IDLE);
		for (auto it = m_scouts.begin(); it != m_scouts.end(); it = it)//overload would be prettier but is problematic
		{
			if (scout->get_scout()->getID() == it->get_scout()->getID()) it = m_scouts.erase(it);
			else it++;
		}
	}
}

void Scouting::add_scout(Unit_Mapping* map)
{/* if there is a remaining path in m_scout_not_assigned_paths :
	* let Scoutinterface create a new interface. Scoutinterface will:
	* -> find a unit near a path and make that unit a scout(handled by Scoutinterface)
	* -> set the path for the unit to be a remaining path to scout
	*/

	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	if (!m_scout_not_assigned_paths.empty()) // if there are paths that have no scout
	{
		vector < BWAPI::Position> path = m_scout_not_assigned_paths.front();
		m_scout_not_assigned_paths.pop_back();
		m_scouts.push_back(Scout_Interface(path, map)); // then this
		m_scout_assigned_paths.push_back(path);
	}
}

bool Scouting::pos_in_path(BWAPI::Position position, vector<BWAPI::Position> path)
{
	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	if ((std::find(path.begin(), path.end(), position) != path.end()))
		//(m_scout_assigned_paths.contains(element))
		return true;
	return false;
}

vector<BWAPI::Position> Scouting::closest_path(BWAPI::Unit scout)
{
	if (Scout_Debugging) std::cout << "Scouting::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	BWAPI::Position scout_pos = scout->getPosition();
	vector<int> distances;
	int mindist = 999999;
	for (auto path : m_scout_not_assigned_paths)
	{
		distances.push_back(scout_pos.getDistance((path.front())));
		for (auto dist : distances)
		{
			if (mindist > dist)
			{
				mindist == dist;
			}

			for (auto path : m_scout_not_assigned_paths)
			{
				if (path.front().getDistance(scout_pos) == mindist)
					return path;
			}
		}
	}
}