#include "Unit_Mapping.h"
#include <list>
#include "BWAPI.h"
using namespace std;

class Scouting;

class Scout_Interface
{
    friend class Scouting;
    friend class Unit_Mapping;
        public:
                Scout_Interface();
                Scout_Interface(list<BWAPI::Position> path, Unit_Mapping* map);
                //Scout_Interface(BWAPI::Unit scout);
                ~Scout_Interface();
                bool discovered(Unit_Mapping * m_map);
	
                bool in_range();
                bool at_destination(); //operating on in_range
                bool at_final_dest(); //operating on in_range
				// TODO ADD get_scout_loc
	// get three paths and scouts those delete
	// if we find the enemy drop the remaining
	// if not find enemy update interface with new path
	// and create a new 
                bool move(Unit_Mapping* map);
                void set_dest_path(list<BWAPI::Position> dest_path);
                list<BWAPI::Position> get_dest_path();
                BWAPI::Unit get_scout();
                void scout_killed(Unit_Mapping* map);
                bool Scout_Interface::is_temp();

        private:
				list<BWAPI::Position> m_path;
                list<BWAPI::Position>::iterator m_iterator;
                BWAPI::Unit m_scout;

                void find_scout(Unit_Mapping* map);
                
                const float distance(BWAPI::Position, BWAPI::Position);
                void SmartMove(BWAPI::Unit attacker, const BWAPI::Position & targetPosition);

};
