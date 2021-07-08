
#include "Scout_Interface.h"
#include "Unit_Mapping.h"



class Scouting
{

        public:

                void scout_start_loc(Unit_Mapping * map);
                void update_scout_interface(Unit_Mapping * map);
                void replace_if_scout(BWAPI::Unit unit, Unit_Mapping * map);

        private:

            list<BWAPI::Point> m_scout_paths; BWAPI::Point<>
            list<BWAPI::Point> m_scout_temps;
            list<BWAPI::Point> m_scouts;

                void parse_start_loc(Unit_Mapping * map);
                void on_arrived(Scout_Interface scout, Unit_Mapping * map);
                void on_discovery(Scout_Interface * scout, Unit_Mapping * map);
                void Scouting::update_scout_path( Scout_Interface * scout );
                void Scouting::add_scout();
};
