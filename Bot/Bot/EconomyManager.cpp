#include "EconomyManager.h"

vector<BWAPI::Unit> EconomyManager::posses_units(vector<BWAPI::Unit> common_units)
{
	if (Economy_Debugging) std::cout << "EconomyManager::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	int num_got = 0;

	for (int ID : unit_id) // takes old units
	{
		for (auto it = common_units.cbegin(); it != common_units.cend(); it = it)
		{
			if (ID == (*it)->getID())
			{
				unitvector.push_back(*it);
				it = common_units.erase(it);
				// delete in common manager vector for return
				//append own vector for managment
				num_got++;
			}
			else it++;
		}
	}

	for (auto it = common_units.begin(); it != common_units.end(); it = it) //taking additional units till we have enough
	{
		if (num_got < num_required)
		{
			unitvector.push_back(move(*it)); //append own vector for managment
			it = common_units.erase(it); // delete in common manager vector for return
			num_got++;
		}
		else break;
	}

	unit_id = vector<int>(); //update IDs
	for (auto unit : unitvector)
	{
		unit_id.push_back(unit->getID());
	}
	cout << size(unit_id) << " working units" << endl;
	return common_units;
}

void EconomyManager::clear()
{
	if (Economy_Debugging) std::cout << "EconomyManager::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	unitvector.clear(); //resets the vector of workers
}

vector<BWAPI::Unit> EconomyManager::onFrame(vector<BWAPI::Unit> workerunits)
{
	if (Economy_Debugging) std::cout << "EconomyManager::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	cout << "EconomyManager::onFrame" << endl;
	num_required = 100;
	vector<BWAPI::Unit>return_units = posses_units(workerunits);//writes back to member unitsvector
	sendIdleWorkersToMinerals(); //accesses unitvector member and sends included workers to gather
	trainAdditionalWorkers();
	buildAdditionalSupply();
	clear();
	return return_units;
}

void EconomyManager::sendIdleWorkersToMinerals()
{
	if (Economy_Debugging) std::cout << "EconomyManager::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	for (auto& unit : unitvector)
	{
		// Check the unit type, if it is an idle worker, then we want to send it somewhere
		if (unit->getType().isWorker() && unit->isIdle())
		{
			// Get the closest mineral to this worker unit
			BWAPI::Unit closestMineral = Tools::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());

			// If a valid mineral was found, right click it with the unit in order to start harvesting
			if (closestMineral) { unit->rightClick(closestMineral); }
		}
	}
}

void EconomyManager::trainAdditionalWorkers()
{
	if (Economy_Debugging) std::cout << "EconomyManager::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
	const int workersWanted = 20;
	const int workersOwned = Tools::CountUnitsOfType(workerType, BWAPI::Broodwar->self()->getUnits());
	if (workersOwned < workersWanted)
	{
		// get the unit pointer to my depot
		const BWAPI::Unit myDepot = Tools::GetDepot();

		// if we have a valid depot unit and it's currently not training something, train a worker
		// there is no reason for a bot to ever use the unit queueing system, it just wastes resources
		if (myDepot && !myDepot->isTraining()) { myDepot->train(workerType); }
	}
}

void EconomyManager::buildAdditionalSupply()
{
	if (Economy_Debugging) std::cout << "EconomyManager::" << __func__ << std::endl;//for debugging purposes change in "Debugger.hpp"

	// Get the amount of supply supply we currently have unused
	const int unusedSupply = Tools::GetTotalSupply(true) - BWAPI::Broodwar->self()->supplyUsed();

	// If we have a sufficient amount of supply, we don't need to do anything
	if (unusedSupply >= 2) { return; }

	// Otherwise, we are going to build a supply provider
	const BWAPI::UnitType supplyProviderType = BWAPI::Broodwar->self()->getRace().getSupplyProvider();

	const bool startedBuilding = Tools::BuildBuilding(supplyProviderType);
	if (startedBuilding)
	{
		BWAPI::Broodwar->printf("Started Building %s", supplyProviderType.getName().c_str());
	}
}