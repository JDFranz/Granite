#pragma once
#include <list>
#include "BWAPI.h"
#include  "EconomyManager.h"
#include "ScoutManager.h"

class GlobalManager 
{

	EconomyManager ecoman = EconomyManager() ;
	ScoutManager scoutman = ScoutManager();


	Unit_Mapping* m_map;
	Scouting* m_scout;

	int num_needed_scouts = 1;
	
	//lists of units per task
	list<BWAPI::Unit> unitlist;
	list<BWAPI::Unit> scoutlist;
	list<BWAPI::Unit> workerlist;
	list<BWAPI::Unit> buildinglist;
	list<BWAPI::Unit> militarylist;
	
	list<int> worker_ids; //used to compare which unit was managed last frame
	list<int> scout_ids;
	 //how many units this manager wants to manage

protected:
	int num_existing_units=0;
public:
	

	list<BWAPI::Unit>  get_units();
	list<BWAPI::Unit> onFrame();
	list<BWAPI::Unit> filter_worker();
	list<BWAPI::Unit> filter_scout();
	void onStart();
};

