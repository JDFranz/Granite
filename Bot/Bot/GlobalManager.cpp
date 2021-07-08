#include "GlobalManager.h"


list<BWAPI::Unit> GlobalManager::onFrame()
{
	unitlist = get_units();
	num_existing_units= unitlist.size();
	unitlist = scoutman.onFrame(unitlist);
	list<BWAPI::Unit>workers = filter_worker();
	unitlist=ecoman.onFrame(workers);
	return unitlist;
}

//this function returns a list of all workers and removes them from the global unitlist
list<BWAPI::Unit> GlobalManager::filter_worker()
{
	list<BWAPI::Unit> notworker(unitlist);
	list<BWAPI::Unit> workerlist= list<BWAPI::Unit>();
	for (auto& unit :unitlist) 
	{
		if (unit->getType().isWorker())
		{
			workerlist.push_back(move(unit));
			
		}
	}
	
	for (auto it = notworker.begin(); it!= notworker.end(); it=it)
	{
		if ((*it)->getType().isWorker())
		{

			it = notworker.erase(it);
		}
		else it++;
	}
	
	
	return workerlist;
	
}


list<BWAPI::Unit>  GlobalManager::get_units()
{
	list<BWAPI::Unit> returnlist = list<BWAPI::Unit>();
	const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
	for (auto unit : myUnits)
	{
		returnlist.push_back(unit);
	}
	return returnlist;
}
