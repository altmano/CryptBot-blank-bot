#pragma once

#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_agent.h"
#include "sc2api/sc2_map_info.h"
#include "sc2lib/sc2_lib.h"
#include "Rallypoint.h"
#include "Bases.h"
#include "Workers.h"
#include "Buildorder.h"
#include "UnitProduction.h"
#include "MicroManager.h"
#include "Scouting.h"

class CryptBot : public sc2::Agent
{
public:
	CryptBot();
	Rallypoint rallypoint;
	Bases bases;
	Workers workers;
	Buildorder buildorder;
	UnitProduction unitproduction;
	MicroManager micromanager;
	Scouting scouting;
	virtual void OnGameStart() override;
	virtual void OnStep() override;
    virtual void OnUnitIdle(const sc2::Unit *unit) override;
	virtual void OnUnitDestroyed(const sc2::Unit *unit) override;
    virtual void OnUnitCreated(const sc2::Unit *unit) override;
    virtual void OnUnitEnterVision(const sc2::Unit *unit) override;
	virtual void OnBuildingConstructionComplete(const sc2::Unit *unit) override;
	int attacklaunchercap = 30;
	bool trybuildingassimilator();	
	size_t CountUnitType(sc2::UNIT_TYPEID);
	int attention = 1;
	
};

