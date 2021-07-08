#pragma once
#include "Tools.h"
#include <list>

using namespace std;

class EconomyManager 
{
	list<BWAPI::Unit> unitlist; //units for the Manager to command
	list<int> unit_id; //used to compare which unit was managed last frame
	int num_required; //how many units this manager wants to manage

	list<BWAPI::Unit> posses_units(list<BWAPI::Unit> unitlist);

public:
	void clear();
	list<BWAPI::Unit> onFrame(list<BWAPI::Unit> otherunitlist) ;
	void sendIdleWorkersToMinerals();
	void trainAdditionalWorkers();
	void buildAdditionalSupply();
};

