#include "StrategyAdaptation.h"
#include "Cryptbot.h"


StrategyAdaptation::StrategyAdaptation(CryptBot & bot) : mbot(bot)
{
}


void StrategyAdaptation::DetectedBio()
{
	std::cout << "Detected bio strategy" << std::endl;
	mbot.unitproduction.LoadGatewayProduction(BioCounterGatewayProductionName);
	mbot.unitproduction.LoadRoboProduction(BioCounterRoboProductionName);
	mbot.unitproduction.LoadStargateProduction(BioCounterStargateProductionName);
}

void StrategyAdaptation::DetectedMech()
{
	std::cout << "Detected mech strategy" << std::endl;
	mbot.unitproduction.LoadGatewayProduction(MechCounterGatewayProductionName);
	mbot.unitproduction.LoadRoboProduction(MechCounterRoboProductionName);
	mbot.unitproduction.LoadStargateProduction(MechCounterStargateProductionName);
}

void StrategyAdaptation::DetectedAir()
{
	std::cout << "Detected air strategy" << std::endl;
	mbot.unitproduction.LoadGatewayProduction(AirCounterGatewayProductionName);
	mbot.unitproduction.LoadRoboProduction(AirCounterRoboProductionName);
	mbot.unitproduction.LoadStargateProduction(AirCounterStargateProductionName);
}

void StrategyAdaptation::DetectedAggro()
{
	std::cout << "Detected aggro strategy" << std::endl;
	mbot.unitproduction.LoadGatewayProduction(AggroCounterGatewayProduction);
	mbot.unitproduction.LoadRoboProduction(AggroCounterRoboProduction);
	mbot.unitproduction.LoadStargateProduction(AggroCounterStargateProduction);
}