#pragma once
#include "sc2api/sc2_agent.h"
#include <iostream>
#include <string>
class CryptBot;

class StrategyAdaptation
{
	CryptBot & mbot;
public:
	StrategyAdaptation(CryptBot & bot);
//Files with unit productions to counter bio
	std::string BioCounterGatewayProductionName = "";
	std::string BioCounterRoboProductionName = "";
	std::string BioCounterStargateProductionName = "";
//
	std::string MechCounterGatewayProductionName = "";
	std::string MechCounterRoboProductionName = "";
	std::string MechCounterStargateProductionName = "";
//
	std::string AirCounterGatewayProductionName = "";
	std::string AirCounterRoboProductionName = "";
	std::string AirCounterStargateProductionName = "";
//
	std::string AggroCounterGatewayProduction = "";
	std::string AggroCounterRoboProduction = "";
	std::string AggroCounterStargateProduction = "";

	void DetectedBio();
	void DetectedMech();
	void DetectedAir();
	void DetectedAggro();
};

