#pragma once
#include "StarDraftMap.hpp"
#include "MapTools.h"
#include "BWAPI.h"
#include "Scouting.h"
#include <vector>

class Unit_Mapping;
class EconomyManager;

class GraniteBot
{
	MapTools m_mapTools;
	StarDraftMap m_map;
	Unit_Mapping& m_unit_mapping;
	Scouting m_scouting;
	EconomyManager& ecoman;

public:
	GraniteBot();
	// helper functions to get you started with bot programming and learn the API
	void sendIdleWorkersToMinerals();
	void trainAdditionalWorkers();
	void buildAdditionalSupply();
	void drawDebugInformation();

	// functions that are triggered by various BWAPI events from main.cpp
	void onStart();
	void onFrame();
	void onEnd(bool isWinner);
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);
	void onSendText(std::string text);
	void onUnitCreate(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onUnitShow(BWAPI::Unit unit);
	void onUnitHide(BWAPI::Unit unit);
	void onUnitRenegade(BWAPI::Unit unit);
	void export_map();

	//TODO these need to be removed they belong into Unit::Mapping

		//vectors of units per task

	std::vector<BWAPI::Unit> unitvector;
	std::vector<BWAPI::Unit> workervector;
	std::vector<BWAPI::Unit> buildingvector;
	std::vector<BWAPI::Unit> militaryvector;

	std::vector<int> worker_ids; //used to compare which unit was managed last frame

	//how many units this manager wants to manage

	std::vector<BWAPI::Unit>  get_units()
	{
		std::vector<BWAPI::Unit> returnvector = std::vector<BWAPI::Unit>();
		const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
		for (auto unit : myUnits)
		{
			returnvector.push_back(unit);
		}
		return returnvector;
	}

	std::vector<BWAPI::Unit> filter_worker()
	{
		int num_got = 0;
		workervector = std::vector<BWAPI::Unit>();
		for (int ID : worker_ids) // takes old workers by Id
		{
			for (auto it = unitvector.cbegin(); it != unitvector.cend(); it = it)
			{
				if (ID == (*it)->getID())
				{
					workervector.push_back(*it);
					it = unitvector.erase(it);
					// delete in common manager vector for return
					//append own vector for managment
					num_got++;
				}
				else it++;
			}
		}

		std::vector<BWAPI::Unit> notworker(unitvector);

		//Adding all further workers to the workervector
		for (auto it = unitvector.begin(); it != unitvector.end(); it = it)
		{
			if ((*it)->getType().isWorker())
			{
				workervector.push_back(*it);
				it = unitvector.erase(it);
			}
			else it++;
		}

		worker_ids = std::vector<int>(); //update IDs
		for (auto unit : workervector)
		{
			worker_ids.push_back(unit->getID());
		}
		std::cout << size(worker_ids) << " working units" << std::endl;

		return workervector;
	}
};