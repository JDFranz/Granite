#pragma once
#include <BWAPI.h>
#include <Tools.h>
#include <memory>

const  bool Unit_Mapping_Debugging = true;

enum worker_detail { MINE, HARVEST, BUILD, SCOUT, ATTACK, DEFEND, IDLE, EVADE };
enum buildings {
	NEXUS = 1, PYLON, GATEWAY, ASSIMILATOR, SHIELD,
	FORGE, CANNON, CORE, STARGATE, ROBO, OBSERVATORY,
	BAY, CITADEL, ARCHIVES, BEACON, TRIBUNAL
};

class Unit_Mapping {
public:
	//Unit_Mapping& operator=(Unit_Mapping&) = delete;
	//Unit_Mapping(Unit_Mapping&) = delete;

	BWAPI::UnitType  Unit_Mapping::get_build_type();
	BWAPI::Position Unit_Mapping::get_build_loc();
	void Unit_Mapping::remove_build();
	bool Unit_Mapping::bl_empty();

	//Counting  Stuff../
	Unit_Mapping();
	~Unit_Mapping();
	void print();
	void Unit_Mapping::add_to_count(BWAPI::Unit u);
	void Unit_Mapping::remove_from_count(BWAPI::Unit u, bool warping);
	int Unit_Mapping::g_count(BWAPI::UnitType u);//granite
	int Unit_Mapping::g_total_count(BWAPI::UnitType u);
	int Unit_Mapping::g_warping_count(BWAPI::UnitType u);
	int Unit_Mapping::e_count(BWAPI::UnitType u);//enemy
	int Unit_Mapping::e_total_count(BWAPI::UnitType u);
	int Unit_Mapping::e_warping_count(BWAPI::UnitType u);
	//..

	std::vector<BWAPI::Position> e_get_potential_startpositions();
	bool Unit_Mapping::enemy_mapped(BWAPI::Unit u);
	void add_enemy_ID(BWAPI::Unit u);
	int get_enemy_ID(BWAPI::Unit u);
	void Unit_Mapping::del_enemy_ID(BWAPI::Unit u);

	bool Unit_Mapping::w_enemy_mapped(BWAPI::Unit u);
	void Unit_Mapping::w_add_enemy_ID(BWAPI::Unit u);
	void Unit_Mapping::w_del_enemy_ID(BWAPI::Unit u);

	const int Unit_Mapping::get_task(BWAPI::Unit u);
	void Unit_Mapping::set_task(BWAPI::Unit u, enum worker_detail task);

	int Unit_Mapping::get_unit_shp(BWAPI::Unit u);
	void Unit_Mapping::set_unit_shp(BWAPI::Unit u);

	bool Unit_Mapping::is_under_fire(BWAPI::Unit u);
	int Unit_Mapping::last_frame_attacked(BWAPI::Unit u);
	void Unit_Mapping::set_under_fire(BWAPI::Unit u);
	void Unit_Mapping::set_not_under_fire(BWAPI::Unit u);

	void Unit_Mapping::clear_worker_engage(BWAPI::Unitset u);
	void Unit_Mapping::set_worker_engage(BWAPI::Unitset, BWAPI::Unitset);
	BWAPI::Unitset Unit_Mapping::get_defenders(BWAPI::Unit);
	BWAPI::Unit Unit_Mapping::closest_attacker(BWAPI::Unit, BWAPI::Unitset);

	void Unit_Mapping::add_enemy_base(BWAPI::Unit u);
	void Unit_Mapping::add_enemy_base(BWAPI::Position);
	bool Unit_Mapping::enemy_base_found(BWAPI::Unit u);

private:// ../

	struct under_fire_data
	{
		bool is = false;
		int last_frame = 0;
	};

	struct build_data
	{
		BWAPI::UnitType type;
		BWAPI::Position loc;
	};

	BWAPI::Unitset e_warp;
	std::map<int, enum worker_detail> worker_task;// int is a unit->getID()
	std::list<std::pair<int, enum worker_detail>> worker_tasks_list;
	std::map<int, bool> enemy_units;
	std::map<int, bool> w_enemy_units;
	BWAPI::Position enemy_base;

	std::map<BWAPI::UnitType, int> g_unit_count; //granite
	std::map<BWAPI::UnitType, int> g_warp_count;
	std::map<BWAPI::UnitType, int> e_unit_count; //enemy
	std::map<BWAPI::UnitType, int> e_warp_count;
	std::map<BWAPI::Unit, BWAPI::Unit[2]> mineral_gather;

	std::map<int, struct build_data> build_list;

	std::map<BWAPI::Unit, int> unit_shp;
	std::map<BWAPI::Unit, struct under_fire_data> under_fire;

	std::map<BWAPI::Unit, BWAPI::Unitset> worker_engage;

	struct Build_Data {
		int type;
		BWAPI::Position loc;
	};

	std::vector<Build_Data> m_building;

	void Unit_Mapping::set_start_count();
	void Unit_Mapping::set_build_list();
	void Unit_Mapping::parse_build_data(std::string data);
	int Unit_Mapping::get_type(std::string data);
	int Unit_Mapping::get_coord(std::string data);
	std::vector<std::string> Unit_Mapping::parse_csv(std::string line);
	std::vector<std::string> Unit_Mapping::reverse_tokens(std::vector<char*> vec);
};

// bool operator ==(const BWAPI::Position a, const BWAPI::Position b)
// {
// 	return a.getDistance(b) <= 10;
// }
