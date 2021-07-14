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
	Scout_Interface(vector<BWAPI::Position> path, shared_ptr< Unit_Mapping> map);
	//Scout_Interface(BWAPI::Unit scout);
	~Scout_Interface();
	bool is_e_discovered();
	void find_scout(shared_ptr<Unit_Mapping> map);
	bool is_at_destination(); //operating on in_range
	bool is_waypoint_in_sight();
	bool is_at_final_dest();
	bool move(shared_ptr< Unit_Mapping> map);

	void set_dest_path_stupidly(vector<BWAPI::Position> path);
	void set_dest_path_smartly(vector<BWAPI::Position> dest_path);

	vector<BWAPI::Position> get_dest_path();
	BWAPI::Unit get_scout();
	void scout_killed(shared_ptr< Unit_Mapping> map);

private:

	vector<BWAPI::Position> m_path_togo;
	BWAPI::Unit m_scout;

	float distance(BWAPI::Position, BWAPI::Position);
	void SmartMove(BWAPI::Unit attacker, const BWAPI::Position& targetPosition);
};
