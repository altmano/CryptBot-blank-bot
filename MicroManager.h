#pragma once
#include "sc2api/sc2_agent.h"
#include <iostream>
class CryptBot;

struct ArmyGroup
{
	std::vector<const sc2::Unit *> units;
	float x = 100;
	float y = 100;
	bool incombat = false;
	bool attacking = false;
};


class MicroManager
{
	CryptBot & mbot;
public:
	MicroManager(CryptBot & bot);
	void GroupUpAll();
	void GroupUpArmy(int);
	void LaunchAttack();
	void AssignNewUnit(const sc2::Unit  *unit);
	void RemoveDeadUnit(const sc2::Unit * unit);
	void EnenmyUnits();
	void SelectTarget();
	void UpdateArmyLocations();
	void MicroOracles();
private:
	ArmyGroup Oraclegroup;
	ArmyGroup Phoenixgroup;
	ArmyGroup Tempestgroup;
	ArmyGroup Darktemplargroup;
	float calculateavgx(const std::vector< const sc2::Unit *> &);
	float calculateavgy(const std::vector< const sc2::Unit *> &);
	std::vector<ArmyGroup> Armies;
};

