#include "BaseManager.h"

 list<BWAPI::Unit>  BaseManager::get_units()
{
	list<BWAPI::Unit> returnlist = list<BWAPI::Unit>();
	const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
	for ( auto unit :myUnits)
	{
		returnlist.push_back(unit);
	}
	return returnlist;
}

list<BWAPI::Unit> BaseManager::clean_unitlist(list<BWAPI::Unit> otherunitlist)
{
	int index = 0;
	
	if (otherunitlist.empty())
	{
		cout << "list is empty" << endl;
		return unitlist;
	}
	
	for (auto unit : otherunitlist)
	{
		if (!unit)
		{
			cout << index << ":-> " << "found empty element" << endl;
			otherunitlist.remove(unit);
			clean_unitlist(otherunitlist);
		}
	}
	return otherunitlist;
}

list<BWAPI::Unit> BaseManager::posses_units(list<BWAPI::Unit> common_units)
{
	int num_got = 0;
	
	for (int ID : unit_id) // takes old units
	{
		for(auto unit: common_units)
		{
			if(ID==unit->getID())
			{
				common_units.remove(unit); // delete in common manager list for return	
				unitlist.push_back(unit); //append own list for managment
				num_got++;
				
				
			}
		}
	}

	
	for ( auto unit : common_units) //taking additional units till we have enough
	{
		if (num_got < num_required)
		{
			common_units.remove(unit); // delete in common manager list for return	
			unitlist.push_back(unit); //append own list for managment
			num_got++;
		}
		else break;
	}
	
	unit_id = list<int>(); //update IDs
	for (auto unit: unitlist)
	{
		unit_id.push_back(unit->getID());
	}

	return common_units;
}

