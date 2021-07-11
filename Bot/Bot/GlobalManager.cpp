#include "GlobalManager.h"

list<BWAPI::Unit> GlobalManager::onFrame()
{
	unitlist = get_units();
	num_existing_units = unitlist.size();
	//m_scout->ad(this->filter_scout());//get a scout unit into the scout class
	m_scout->onFrame(m_map);
	//unitlist = scoutman.onFrame(unitlist); My old scouting function
	list<BWAPI::Unit>workers = filter_worker();
	unitlist = ecoman.onFrame(workers);
	return unitlist;
}

//this function returns a list of all workers and removes them from the global unitlist
list<BWAPI::Unit> GlobalManager::filter_worker()
{
	int num_got = 0;
	workerlist = list<BWAPI::Unit>();
	for (int ID : worker_ids) // takes old workers by Id
	{
		for (auto it = unitlist.cbegin(); it != unitlist.cend(); it = it)
		{
			if (ID == (*it)->getID())
			{
				workerlist.push_back(*it);
				it = unitlist.erase(it);
				// delete in common manager list for return
				//append own list for managment
				num_got++;
			}
			else it++;
		}
	}

	list<BWAPI::Unit> notworker(unitlist);

	//Adding all further workers to the workerlist
	for (auto it = unitlist.begin(); it != unitlist.end(); it = it)
	{
		if ((*it)->getType().isWorker())
		{
			workerlist.push_back(*it);
			it = unitlist.erase(it);
		}
		else it++;
	}

	scout_ids = list<int>(); //update IDs
	for (auto unit : workerlist)
	{
		worker_ids.push_back(unit->getID());
	}
	cout << size(scout_ids) << " working units" << endl;

	return workerlist;
}

list<BWAPI::Unit> GlobalManager::filter_scout()
{
	/* extrcat the number of required scouts into member scoutlist
	 * adds unit ids to scout_id
	 * removes scouts from unitlist
	 */
	 //number of required scouts
	scoutlist = list<BWAPI::Unit>();
	int num_got = 0;
	for (int ID : scout_ids) // takes old units
	{
		for (auto it = unitlist.cbegin(); it != unitlist.cend(); it = it)
		{
			if (ID == (*it)->getID())
			{
				scoutlist.push_back(*it);
				it = unitlist.erase(it);
				// delete in common manager list for return
				//append own list for managment
				num_got++;
			}
			else it++;
		}
	}

	for (auto it = unitlist.cbegin(); it != unitlist.cend(); it = it) //taking additional units till we have enough
	{
		if (num_got < num_needed_scouts)
		{
			scoutlist.push_back(move(*it)); //append own list for managment
			it = unitlist.erase(it); // delete in common manager list for return
			num_got++;
		}
		else break;
	}

	scout_ids = list<int>(); //update IDs
	for (auto unit : scoutlist)
	{
		scout_ids.push_back(unit->getID());
	}
	cout << size(scout_ids) << " working units" << endl;

	return scoutlist;
}

void GlobalManager::onStart()
{
	m_map = new Unit_Mapping();
	m_scout = new Scouting();
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