#pragma once
#include <exception>
#include <string>
#include "BWAPI.h"
#include "Unit_Mapping.h"

class RefreshViolation :
	public std::exception
{

	
public:
	const BWAPI::Unit unit;
	const worker_detail task;
	RefreshViolation(BWAPI::Unit u, worker_detail t) :
		unit(u), task(t)
	{}
};