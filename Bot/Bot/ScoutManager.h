#pragma once
#include <list>
#include "BWAPI.h"

using namespace std;

class ScoutManager
{
	list<BWAPI::Unit> unitlist; //units for the Manager to command
	list<int> unit_id; //used to compare which unit was managed last frame
	int num_required=1; //how many units this manager wants to manage
	//normally we want one scout

	list<BWAPI::Unit> posses_units(list<BWAPI::Unit> unitlist);

public:
	void clear();
	list<BWAPI::Unit> onFrame(list<BWAPI::Unit> otherunitlist);
	void scoutEnemyBaseLocationDirectly(BWAPI::Unit scout);//directly scout enemy base
	void ReturnToOwnBase(BWAPI::Unit scout);
	void MoveToPerimeter(BWAPI::Unit scout, int perimeter); // move within a certain perimeter of enemy startpos.
};

