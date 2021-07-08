#include "EconomyManager.h"


list<BWAPI::Unit> EconomyManager::posses_units(list<BWAPI::Unit> common_units)
{
    int num_got = 0;

    
    for (int ID : unit_id) // takes old units
    {
        for (auto it = common_units.cbegin(); it != common_units.cend(); it = it)
        {
            if (ID == (*it)->getID())
            {
                unitlist.push_back(*it);
                it=common_units.erase(it);
                // delete in common manager list for return	
                //append own list for managment
                num_got++;


            }
            else it++;
        }
    }



    for (auto it = common_units.cbegin(); it != common_units.cend(); it=it) //taking additional units till we have enough
    {
        if (num_got < num_required)
        {
            unitlist.push_back(move(*it)); //append own list for managment
            it=common_units.erase(it); // delete in common manager list for return	
            num_got++;
        }
        else break;
    }

    
    
    unit_id = list<int>(); //update IDs
    for (auto unit : unitlist)
    {
        unit_id.push_back(unit->getID());
    }
    cout << size(unit_id) << " working units" << endl;
    return common_units;
}

void EconomyManager::clear()
{
    unitlist = list<BWAPI::Unit>(); //resets the list of workers
}

list<BWAPI::Unit> EconomyManager::onFrame(list<BWAPI::Unit> workerunits)
{
    clear();
    cout << "EconomyManager::onFrame" << endl;
    num_required = 100;
	list<BWAPI::Unit>return_units= posses_units(workerunits);//writes back to member unitslist
    sendIdleWorkersToMinerals(); //accesses unitlist member and sends included workers to gather
    trainAdditionalWorkers();
    buildAdditionalSupply();
    return return_units;
}

void EconomyManager::sendIdleWorkersToMinerals()
{
    for (auto& unit : unitlist)
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
