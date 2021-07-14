#include "Scout_Interface.h"
#include "Unit_Mapping.h"

using namespace std;
enum class Scouting_phases {
	STARTING,
	INITIAL_SCOUTING,
	E_BASE_LOCATED,
	E_BASE_SIGHTED,
	E_BASE_Fully_SCOUTED,
	LATE_GAME
};

class Scouting
{
	// all interfaces are totally seperated and do not communicate with each other
	//TODO integrate this into Unit_Mapping ad into the main_bot

	//this is the class to control a vector of scouts
public:

	void on_start(Unit_Mapping* map);
	const void print();
	void onFrame(shared_ptr<Unit_Mapping> map);
	void replace_if_scout(BWAPI::Unit unit, shared_ptr<Unit_Mapping> map);
	bool in_remaining_path(BWAPI::Position element);
	vector<BWAPI::Position> closest_path(BWAPI::Unit scout);

	//setters getters:
	void set_paths(vector < BWAPI::Position>);

	//Add Temp path to remove paths to take

private:
	bool e_basediscovered = false; //whether we theoretically know where the enemy is

	vector<vector<BWAPI::Position>> m_scout_total_paths = vector<vector<BWAPI::Position>>();
	/**var: m_scout_total_paths
	 * purpose: storing the total paths g(GraniteBot) wants to explore atm. example in the beginning paths to potential enemy startloc..
	 * details: Scoutinterfaces will not see this information they get a subvector of m_scout_assigned_paths
	 * -> this vector gets smaller once a final destination on the path is reached.
	 */
	vector<vector<BWAPI::Position>> m_scout_assigned_paths = vector<vector<BWAPI::Position>>();
	/**var: m_scout_assigned_paths
	 * purpose: storing the paths which the scouts are currently working on.
	 * details:
	 * -> if a path is here then it should also be in m_scout_total_paths. if a path is not here but in m_scout_total_paths then the path needs a scout.
	 * -> this vector gets smaller once a final destination on the path is reached.
	 */

	vector<vector<BWAPI::Position>> m_scout_not_assigned_paths = vector<vector<BWAPI::Position>>();
	/**var: m_scout_not_assigned_paths
	 * purpose: holding the inverse of m_scout_assigned_paths to circumvent unnecessary vector comparison
	 * details: on add scout these paths move to m_scout_assigned_paths and get an assigned scout
	 */

	vector<Scout_Interface> m_scouts = vector<Scout_Interface>();

	bool pos_in_path(BWAPI::Position position, vector<BWAPI::Position> path);
	void on_arrived_at_final_dest(Scout_Interface scout, shared_ptr< Unit_Mapping> map);
	void on_e_base_discovery(Scout_Interface* scout, shared_ptr<Unit_Mapping> map);
	void Scouting::update_scout_path(Scout_Interface* scout, shared_ptr<Unit_Mapping> map);
	void Scouting::add_scout(shared_ptr<Unit_Mapping> map);
	void scout_start_loc(shared_ptr<Unit_Mapping> map);
	void add_lost_scout(const shared_ptr<Unit_Mapping>& map);
	void return_idle_to_G_base(shared_ptr<Unit_Mapping> map);
	void scout_enemy_base_on_sighted(shared_ptr<Unit_Mapping> map);
	Scouting_phases Phase = Scouting_phases::STARTING;
};
