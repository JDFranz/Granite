#pragma once
#include "Tools.h"
#include <vector>

using namespace std;
const bool Economy_Debugging = true;
class EconomyManager
{
	vector<BWAPI::Unit> unitvector; //units for the Manager to command
	vector<int> unit_id; //used to compare which unit was managed last frame
	int num_required; //how many units this manager wants to manage

	vector<BWAPI::Unit> posses_units(vector<BWAPI::Unit> unitvector);

public:
	void clear();
	vector<BWAPI::Unit> onFrame(vector<BWAPI::Unit> otherunitvector);
	void sendIdleWorkersToMinerals();
	void trainAdditionalWorkers();
	void buildAdditionalSupply();
};
