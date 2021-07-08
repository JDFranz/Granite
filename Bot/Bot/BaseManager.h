#pragma once

#include <list>
#include "BWAPI.h"
#include "GraniteBot.h"

using namespace	std;
/*
 * This is the baseclass for all other manager classes it provides basic functionality
 */
class BaseManager
{
	friend class GraniteBot;

	list<BWAPI::Unit> clean_unitlist(list<BWAPI::Unit> otherunitlist);

protected:

	list<BWAPI::Unit> posses_units(list<BWAPI::Unit> unitlist);
	list<BWAPI::Unit> unitlist; //units for the Manager to command
	list<int> unit_id; //used to compare which unit was managed last frame
	int num_required; //how many units this manager wants to manage
	

public:
	list<BWAPI::Unit> get_units();// gets all the units of the game and returns them 
	void set_num_required(int number_of_required_units);
	virtual list<BWAPI::Unit> onFrame(list<BWAPI::Unit>  otherunitlist) =0; //no implementation just to clarify that every manager has a OnFrame method
};

