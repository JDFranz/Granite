#include "ScoutManager.h"

list<BWAPI::Unit> ScoutManager::posses_units(list<BWAPI::Unit> common_units)
{
    int num_got = 0;


    for (int ID : unit_id) // takes old units
    {
        for (auto it = common_units.cbegin(); it != common_units.cend(); it = it)
        {
            if (ID == (*it)->getID())
            {
                unitlist.push_back(*it);
                it = common_units.erase(it);
                // delete in common manager list for return	
                //append own list for managment
                num_got++;


            }
            else it++;
        }
    }



    for (auto it = common_units.cbegin(); it != common_units.cend(); it = it) //taking additional units till we have enough
    {
        if (num_got < num_required)
        {
            unitlist.push_back(move(*it)); //append own list for managment
            it = common_units.erase(it); // delete in common manager list for return	
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

void ScoutManager::clear()
{
    unitlist = list<BWAPI::Unit>(); //resets the list of workers
}

list<BWAPI::Unit> ScoutManager::onFrame(list<BWAPI::Unit> allunits)
{
    clear();
    cout << "ScoutManager::onFrame" << endl;
    list<BWAPI::Unit>return_units = posses_units(allunits);//writes back to member unitslist
	//the return vector is the original without the scouts
	//number of required scouts is initialized as 1

	for (auto unit:unitlist)
	{
        scoutEnemyBaseLocationDirectly(unit);
	}
	
    return return_units;
}

void ScoutManager::scoutEnemyBaseLocationDirectly(BWAPI::Unit scout) 
{
	/*Moving directly to enemy baselocation
	 *From the Tutorial:
	 * https://www.youtube.com/watch?v=czhNqUxmLks&t=4281s
	 */

    if (!scout) { return; } //checking if scout pointer is valid
	for (auto tile : BWAPI::Broodwar->getStartLocations())//iterating through enemy start locations
	{
        BWAPI::Position pos(tile);
        auto command = scout->getLastCommand();
        if (command.getTargetPosition() == pos) { return; }
		//we are checking if we are giving a command multiple times to avoid lags

        scout->move(pos);
	}
	
}


void ScoutManager::ReturnToOwnBase(BWAPI::Unit scout)
{/*
	function to make the scout return to own base so it is not in danger
	*/
    if (!scout)  return;  //checking if scout pointer is valid
    auto tile = BWAPI::Broodwar->self()->getStartLocation();//get my start location
    auto command = scout->getLastCommand();
    BWAPI::Position pos(tile);
    if (command.getTargetPosition() == pos) { return; }
        //we are checking if we are giving a command multiple times to avoid lags
        scout->move(pos);
}


