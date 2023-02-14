#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

//#include "C:\Users\Pyroman\Documents\Visual Studio 2017\SC2API_Binary_vs2017\include\sc2api\sc2_agent.h"
#include "sc2api/sc2_agent.h"
class CryptBot;

struct Gatewayproduction
{
	sc2::UNIT_TYPEID Transitionunit;
	int Transitionunitcount;
	float Zealotratio;
	float Stalkerratio; 
	float Sentryratio;
	float Hightemplarrratio;
	float Darktemplarratio;
};

struct Stargateproduction
{
	sc2::UNIT_TYPEID Transitionunit;
	int Transitionunitcount;
	float Phoenixratio;
	float Oracleratio;
	float Voidrayratio;
	float Tempestratio;
	float Carrierratio;
};

struct Roboticsproduction
{
	sc2::UNIT_TYPEID Transitionunit;
	int Transitionunitcount;
	float Observerratio;
	float Warpprismratio;
	float Immortalratio;
	float Colossusratio;
	float Disruptorratio;
};

class UnitProduction
{
	CryptBot & mbot;
public:
	UnitProduction(CryptBot &);
	void ProduceFromGate(const sc2::Unit  *unit);
	void ProduceFromRobo(const sc2::Unit *unit);
	void ProduceFromStargate(const sc2::Unit *unit);
	void LoadGatewayProduction(const std::string &);
	void LoadRoboProduction(const std::string &);
	void LoadStargateProduction(const std::string &);
private:
	std::vector<Gatewayproduction> Gatewayproductionratios;
	std::vector<Roboticsproduction> Roboproductionratios;
	std::vector<Stargateproduction> Stargateproductionratios;
	sc2::UNIT_TYPEID Identifyunit(const std::string &);
	int Currentgatewayproduction = 0;
	int Currentroboproduction = 0;
	int Currentstargateproduction = 0;
	size_t CountUnitType(sc2::UNIT_TYPEID unit_type);
	void CreateGatewayUnit(sc2::UNIT_TYPEID, const sc2::Unit *unit);
};

