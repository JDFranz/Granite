#include "Unit_Mapping.h"
#include <list>
#include "BWAPI.h"
using namespace std;

class Scouting;

class Scout_Interface
{
    friend class Scouting;
        public:
                Scout_Interface();
                Scout_Interface(list<BWAPI::Position> path, Unit_Mapping* map, bool temp);
                ~Scout_Interface();
                bool discovered(Unit_Mapping * m_map);
                bool at_final_dest();
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
                bool m_temp;

                void find_scout(Unit_Mapping* map);
                bool at_destination();
                bool in_range();
                float distance(BWAPI::Position, BWAPI::Position);
                void SmartMove(BWAPI::Unit attacker, const BWAPI::Position & targetPosition);

};
