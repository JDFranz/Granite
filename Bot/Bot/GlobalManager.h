#pragma once
#include <list>
#include "BWAPI.h"
#include  "EconomyManager.h"
#include "ScoutManager.h"

class GlobalManager 
{

	EconomyManager ecoman = EconomyManager() ;
	ScoutManager scoutman = ScoutManager();
	
	list<BWAPI::Unit> unitlist; //units for the Manager to command
	list<int> unit_id; //used to compare which unit was managed last frame
	int num_required; //how many units this manager wants to manage

protected:
	int num_existing_units=0;
public:
	

	list<BWAPI::Unit>  get_units();
	list<BWAPI::Unit> onFrame();
	list<BWAPI::Unit> filter_worker();

	
};

