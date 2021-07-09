#include "Scout_Interface.h"
#include "Unit_Mapping.h"
#include "Tools.h"


class Globalmanager;

class Scouting
{
	// all interfaces are totally seperated and do not communicate with each other
	//TODO integrate this into Unit_Mapping ad into the main_bot 
	
    friend class Globalmanager;
    //this is the class to control a list of scouts
        public: 
            
				void scout_start_loc(Unit_Mapping * map);
				
                void update_scout_interface(Unit_Mapping * map);
                void replace_if_scout(BWAPI::Unit unit, Unit_Mapping * map);
                bool in_remaining_path(BWAPI::Position element);
                list<BWAPI::Position> closest_path(BWAPI::Unit scout);

	//setters getters: 
				void set_paths(list < BWAPI::Position>);

	//Add Temp path to remove paths to take

private:
			bool e_iscovered_enemy = false; //whether we theoretically know where the enemy is
            
			//TODO this needs actually to be a list of lists of BWAPI::Positions
            list<BWAPI::Position> m_scout_total_paths = list<BWAPI::Position>();
	// when a new scout is added one path from m_scout_paths gets removed into the scouts paths.
            list<BWAPI::Position> m_scout_remaining_path=list<BWAPI::Position>();//what is this for? -> for holdin elements that might get deleted from the path
            list<Scout_Interface> m_scouts= list<Scout_Interface>();

                void parse_start_loc(Unit_Mapping * map);
                void on_arrived(Scout_Interface scout, Unit_Mapping * map);
                void on_discovery(Scout_Interface * scout, Unit_Mapping * map);
                void Scouting::update_scout_path(Scout_Interface* scout, Unit_Mapping* map);
                void Scouting::add_scout(Unit_Mapping* map);
                

                
};
