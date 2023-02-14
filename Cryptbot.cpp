#include <iostream>

#include "Cryptbot.h"
#include "sc2api/sc2_api.h"
#include <time.h>


CryptBot::CryptBot() :
	rallypoint(*this),
	bases(*this),
	workers(*this),
	buildorder(*this),
	unitproduction(*this),
	micromanager(*this),
	scouting(*this)

{

}


void CryptBot::OnGameStart()
{
	srand(time(NULL));
	int randombuildorder = std::rand() % 4;

	bases.updatefields();
	for (auto& it : bases.allbases)
	{
		std::cout << it.x << ' ' << it.y << std::endl;
	}
	switch (randombuildorder)
	{
	case 0 :
		std::cout << "Car" << std::endl;
		buildorder.LoadBuildOrder("Carrier.txt");
		unitproduction.LoadStargateProduction("CarrierStargate.txt");
		unitproduction.LoadGatewayProduction("CarrierGateway.txt");
		break;
	case 1:
		std::cout << "Void" << std::endl;
		buildorder.LoadBuildOrder("Voidray.txt");
		unitproduction.LoadStargateProduction("VoidrayStargate.txt");
		unitproduction.LoadGatewayProduction("VoidrayGateway.txt");
		break;
	case 2 :
		std::cout << "Laser" << std::endl;
		buildorder.LoadBuildOrder("Colossus.txt");
		unitproduction.LoadGatewayProduction("ColossusGateway.txt");
		unitproduction.LoadRoboProduction("ColossusRobo.txt");
		scouting.strategyadaptation.AirCounterGatewayProductionName = "ColossusAirCounterGatewayProduction.txt";
		scouting.strategyadaptation.MechCounterRoboProductionName = "ColossusMechCounterRoboProduction.txt";
		attacklaunchercap = 35;
		break;
	case 3:
		std::cout << "4Gate" << std::endl;
		buildorder.LoadBuildOrder("4Gate.txt");
		unitproduction.LoadGatewayProduction("4GateGateway.txt");
		attacklaunchercap = 12;
		break;
	}
	//buildorder.Initbuildorder();
	//
	
	//unitproduction.InitUnitProduction();

	//
}

void CryptBot::OnStep() {
	Observation();
	// What your bot does every game step
	switch (attention)
	{
	case 1:
		buildorder.Trybuildingpylon();
		trybuildingassimilator();
		micromanager.UpdateArmyLocations();
		attention = 2;
		break;
	case 2:
	{
		buildorder.Build();
		attention = 3;
		break;
	}

	case 3:
		workers.GiveJobs();
		attention = 4;
		break;

	case 4:
		micromanager.SelectTarget();
		micromanager.MicroOracles();
		attention = 5;
		break;
	case 5: 
		micromanager.UpdateArmyLocations();
		attention = 1;
		break;
	}

		
	


	
}
void CryptBot::OnUnitDestroyed(const sc2::Unit *unit)
{
	if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_ORACLE || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_ZEALOT ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_STALKER || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_SENTRY || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_HIGHTEMPLAR ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_IMMORTAL || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_COLOSSUS ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_VOIDRAY || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PHOENIX ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_CARRIER || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_TEMPEST
		|| unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_DISRUPTOR)
	{
		micromanager.RemoveDeadUnit(unit);
	}
	
}

void CryptBot::OnUnitEnterVision(const sc2::Unit *unit)
{
	if(unit->unit_type == sc2::UNIT_TYPEID::TERRAN_BARRACKS || unit->unit_type == sc2::UNIT_TYPEID::TERRAN_FACTORY ||
		unit->unit_type == sc2::UNIT_TYPEID::TERRAN_STARPORT)
	{ 
		scouting.Enemystructurescheck();
	}
	else
	{
		scouting.Enemyarmycheck();
	}
}

void CryptBot::OnBuildingConstructionComplete(const sc2::Unit* unit)
{
	rallypoint.SetRallypoint(unit);
}

void CryptBot::OnUnitCreated(const sc2::Unit *unit)
{
	if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_ORACLE || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_ZEALOT ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_STALKER || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_SENTRY || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_HIGHTEMPLAR ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_IMMORTAL || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_COLOSSUS ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_VOIDRAY || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PHOENIX ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_CARRIER || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_TEMPEST
		|| unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_DISRUPTOR)
	{
		micromanager.AssignNewUnit(unit);
	}
	if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PROBE)
	{
		scouting.CheckProbeScout();
	}
		const sc2::ObservationInterface* observation = Observation();
		if (observation->GetArmyCount() > attacklaunchercap || observation->GetFoodUsed() > 190)
		{

			micromanager.LaunchAttack();
		}
}

void CryptBot::OnUnitIdle(const sc2::Unit *unit)
{
	switch (unit->unit_type.ToType()) 
	{
	 case sc2::UNIT_TYPEID::PROTOSS_NEXUS: {
		if (CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PROBE) < 23 * CountUnitType(sc2::UNIT_TYPEID::PROTOSS_NEXUS)) {
			Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_PROBE);
		}

		break;
	 }



	case sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY:
	    {
		unitproduction.ProduceFromRobo(unit);
		break;
	    }

	case sc2::UNIT_TYPEID::PROTOSS_GATEWAY:
			unitproduction.ProduceFromGate(unit);
		break;

	case sc2::UNIT_TYPEID::PROTOSS_WARPGATE:
		unitproduction.ProduceFromGate(unit);
		break;

	case sc2::UNIT_TYPEID::PROTOSS_PROBE: {

		const sc2::ObservationInterface* observation = Observation();
		sc2::Units units = observation->GetUnits();
		for (const auto& patch : units) {
			if (patch->unit_type == sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD) {
				Actions()->UnitCommand(unit, sc2::ABILITY_ID::HARVEST_GATHER, patch);
			}
		}

		break;

	}

	case sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL:
	{
		if (CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ZEALOT) > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_STALKER) &&
			CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ZEALOT) > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ADEPT))
		{
			Actions()->UnitCommand(unit, sc2::ABILITY_ID::RESEARCH_CHARGE);
		}
		else
		{
			if (CountUnitType(sc2::UNIT_TYPEID::PROTOSS_STALKER) > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ZEALOT) &&
				CountUnitType(sc2::UNIT_TYPEID::PROTOSS_STALKER) > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ADEPT))
			{
				Actions()->UnitCommand(unit, sc2::ABILITY_ID::RESEARCH_BLINK);
			}
			else
			{
				Actions()->UnitCommand(unit, sc2::ABILITY_ID::RESEARCH_ADEPTRESONATINGGLAIVES);
			}
		}

		break;
	}
	case sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE:
	{
		if (CountUnitType(sc2::UNIT_TYPEID::PROTOSS_VOIDRAY) > 3 || CountUnitType(sc2::UNIT_TYPEID::PROTOSS_CARRIER) > 1)
		{
			Actions()->UnitCommand(unit, sc2::ABILITY_ID::RESEARCH_PROTOSSAIRWEAPONS);
		}
		else
		{
			Actions()->UnitCommand(unit, sc2::ABILITY_ID::RESEARCH_WARPGATE);
		}
		break;
	}
	case sc2::UNIT_TYPEID::PROTOSS_FORGE:
	{

		Actions()->UnitCommand(unit, sc2::ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONS);
		break;
	}
	case sc2::UNIT_TYPEID::PROTOSS_ROBOTICSBAY:
	{
		Actions()->UnitCommand(unit, sc2::ABILITY_ID::RESEARCH_EXTENDEDTHERMALLANCE);
		break;
	}
	case sc2::UNIT_TYPEID::PROTOSS_STARGATE:
		unitproduction.ProduceFromStargate(unit);
		break;
	}
}

size_t CryptBot::CountUnitType(sc2::UNIT_TYPEID unit_type) {
	return Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(unit_type)).size();
}



bool CryptBot::trybuildingassimilator() {
	if ((CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR) >= CountUnitType(sc2::UNIT_TYPEID::PROTOSS_NEXUS) * 2) 
		|| CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PROBE) < 16) {
		return false;
	}

	const sc2::ObservationInterface* observation = Observation();
	const sc2::Unit* unit_to_build = nullptr;
	const sc2::Unit* location = nullptr;
	const sc2::Units placement = observation->GetUnits(sc2::Unit::Neutral);
	const sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Self);
	for (const auto& unit : units) {
		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PROBE) {
			unit_to_build = unit;
		}
	}
	double distance = 9999.9;
	for (const auto& place : placement) {
		if (place->unit_type == sc2::UNIT_TYPEID::NEUTRAL_VESPENEGEYSER || place->unit_type == sc2::UNIT_TYPEID::NEUTRAL_SPACEPLATFORMGEYSER || place->unit_type == sc2::UNIT_TYPEID::NEUTRAL_PROTOSSVESPENEGEYSER) {
			double currentdistance = sc2::Distance2D(place->pos, unit_to_build->pos);
			if (distance > currentdistance) {
				
				location = place;
				distance = currentdistance;
			}
			
		}
	}
	Actions()->UnitCommand(unit_to_build, sc2::ABILITY_ID::BUILD_ASSIMILATOR, location);
	return true;
	
}
