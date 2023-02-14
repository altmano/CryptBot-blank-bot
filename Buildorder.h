#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
//#include "C:\Users\Pyroman\Documents\Visual Studio 2017\SC2API_Binary_vs2017\include\sc2api\sc2_agent.h"
#include "sc2api/sc2_agent.h"
class CryptBot;

class Buildorder
{
	CryptBot & mbot;
public:
	Buildorder(CryptBot & bot);
	void Build();
	bool Trybuilding(sc2::ABILITY_ID);
	bool Trybuildingnexus(double, double);
	bool Trybuildingpylon();
	void LoadBuildOrder(const std::string &);
	size_t CountUnitType(sc2::UNIT_TYPEID unit_type);
private:
	std::vector<sc2::UNIT_TYPEID> currentbuildorder;
};

