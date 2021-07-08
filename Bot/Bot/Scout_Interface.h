#include "Unit_Mapping.h"
#include <list>
using namespace std;

class Scout_Interface
{
        public:
                Scout_Interface();
                Scout_Interface(list<BWAPI::Point> path, Unit_Mapping* map, bool temp);
                ~Scout_Interface();
                bool discovered(Unit_Mapping * m_map);
                bool at_final_dest();
                bool move(Unit_Mapping* map);
                void set_dest_path(list<BWAPI::Point> dest_path);
                list<BWAPI::Point> get_dest_path();
                BWAPI::Unit get_scout();
                void scout_killed(Unit_Mapping* map);
                bool Scout_Interface::is_temp();

        private:
				list<BWAPI::Point> m_path;
                list<BWAPI::Point>::iterator m_iterator;
                BWAPI::Unit m_scout;
                bool m_temp;

                void find_scout(Unit_Mapping* map);
                bool at_destination();
                bool in_range();
                float distance(BWAPI::Position, BWAPI::Position);
                void SmartMove(BWAPI::Unit attacker, const BWAPI::Position & targetPosition);

};
