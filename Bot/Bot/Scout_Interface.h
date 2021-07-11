#include "Unit_Mapping.h"
#include <vector>
#include "BWAPI.h"
using namespace std;

class Scouting;

const  bool Scout_Debugging = true;

class Scout_Interface
{
	friend class Scouting;
	friend class Unit_Mapping;
public:
	Scout_Interface();
	Scout_Interface(vector<BWAPI::Position> path, Unit_Mapping* map);
	//Scout_Interface(BWAPI::Unit scout);
	~Scout_Interface();
	bool discovered(Unit_Mapping* m_map);
	bool at_destination(); //operating on in_range
	bool waypoint_in_sight();
	bool at_final_dest();
	bool move(Unit_Mapping* map);
	void set_dest_path(vector<BWAPI::Position> dest_path);
	vector<BWAPI::Position> get_dest_path();
	BWAPI::Unit get_scout();
	void scout_killed(Unit_Mapping* map);

private:

	vector<BWAPI::Position> m_path;
	vector<BWAPI::Position>::iterator m_iterator;
	BWAPI::Unit m_scout;

	void find_scout(Unit_Mapping* map);

	float distance(BWAPI::Position, BWAPI::Position);
	void SmartMove(BWAPI::Unit attacker, const BWAPI::Position& targetPosition);
};
