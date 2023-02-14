#include "Buildorder.h"
#include "Cryptbot.h"

 
Buildorder::Buildorder(CryptBot & bot) : mbot(bot)
{
}

size_t Buildorder::CountUnitType(sc2::UNIT_TYPEID unit_type)
{
	return mbot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(unit_type)).size();
}

bool Buildorder::Trybuilding(sc2::ABILITY_ID abilitytypestructure) {
	if (CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PROBE) > 0)
	{
		sc2::UNIT_TYPEID unit_type = sc2::UNIT_TYPEID::PROTOSS_PROBE;
		const sc2::ObservationInterface* observation = mbot.Observation();
		const sc2::Unit* unit_to_build = nullptr;
		std::vector<const sc2::Unit *> nexuses;
		sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Self);
		for (const auto& unit : units)
		{
			for (const auto& order : unit->orders)
			{
				if (order.ability_id == abilitytypestructure)
				{
					return false;
				}


			}
			if (unit->unit_type == unit_type)
			{
				unit_to_build = unit;
			}
			if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_NEXUS)
			{
				nexuses.push_back(unit);
			}
		}
		float rx;
		float ry;
		bool tooclose = true;


		rx = sc2::GetRandomScalar();
		ry = sc2::GetRandomScalar();
		tooclose = false;

		for (auto&& it : nexuses)
		{
			double xdifference = it->pos.x - (unit_to_build->pos.x + rx * 15.0f);
			double ydifference = it->pos.y - (unit_to_build->pos.y + ry * 15.0f);
			double distance = xdifference * xdifference + ydifference * ydifference;
			if (distance < 3)
			{
				tooclose = true;

			}
		}


		if (tooclose == false)
		{
			mbot.Actions()->UnitCommand(unit_to_build,
				abilitytypestructure,
				sc2::Point2D(unit_to_build->pos.x + rx * 15.0f, unit_to_build->pos.y + ry * 15.0f));

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	


}

bool Buildorder::Trybuildingpylon()
{
	int supplycap = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_NEXUS) * 15 + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PYLON) * 8;
	const sc2::ObservationInterface* observation = mbot.Observation();
	if (observation->GetFoodUsed() < 33) {
		if (observation->GetFoodUsed() <= supplycap - 2) {
			return false;
		}
		else {
			return Trybuilding(sc2::ABILITY_ID::BUILD_PYLON);
		}
	}
	else 
	{
		if (observation->GetFoodUsed() < 66)
		{
			if (observation->GetFoodUsed() <= supplycap - 6) {
				return false;
			}
			else {
				return Trybuilding(sc2::ABILITY_ID::BUILD_PYLON);
			}
		}
		else
		{
			if (observation->GetFoodUsed() <= supplycap - 16) {
				return false;
			}
			else {
				return Trybuilding(sc2::ABILITY_ID::BUILD_PYLON);
			}
		}

	}
}

bool Buildorder::Trybuildingnexus(double x, double y)
{
	if (CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PROBE) > 0)
	{
		const sc2::ObservationInterface* observation = mbot.Observation();
		const sc2::Unit* unit_to_build = nullptr;
		const sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Self);
		for (const auto& unit : units) {
			if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PROBE) {
				unit_to_build = unit;
			}
		}

		mbot.Actions()->UnitCommand(unit_to_build, sc2::ABILITY_ID::BUILD_NEXUS, sc2::Point2D(x, y));
		return true;
	}
	else
	{
		return false;
	}

}
void Buildorder::Build()
{
	int Gatecount = 0;
	int Requiredprobecount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PROBE);
	int Probecount = 0;
	int Cybercorecount = 0;
	int Forgecount = 0;
	int Nexuscount = 1;
	int Robocount = 0;
	int Robobaycount = 0;
	int Stargatecount = 0;
	int Fleetbeaconcount = 0;
	int Twilightcount = 0;
	int Archivecount = 0;
	int Shrinecount = 0;
	bool end = false;
	
	for (auto&& it : currentbuildorder)
	{
		if (end == false)
		{
			switch (it)
			{
			case sc2::UNIT_TYPEID::PROTOSS_GATEWAY:
			case sc2::UNIT_TYPEID::PROTOSS_WARPGATE:
				Gatecount++;
				if (Gatecount > (CountUnitType(sc2::UNIT_TYPEID::PROTOSS_GATEWAY) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_WARPGATE)))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_GATEWAY);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_FORGE:
				Forgecount++;
				if (Forgecount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_FORGE))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_FORGE);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE:
				Cybercorecount++;
				if (Cybercorecount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_CYBERNETICSCORE);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_STARGATE:
				Stargatecount++;
				if (Stargatecount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_STARGATE))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_STARGATE);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_FLEETBEACON :
				Fleetbeaconcount++;
				if (Fleetbeaconcount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_FLEETBEACON))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_FLEETBEACON);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL:
				Twilightcount++;
				if (Twilightcount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_TWILIGHTCOUNCIL);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_TEMPLARARCHIVE:
				Archivecount++;
				if (Archivecount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_TEMPLARARCHIVE))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_TEMPLARARCHIVE);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_DARKSHRINE:
				Shrinecount++;
				if (Shrinecount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_DARKSHRINE))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_DARKSHRINE);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY:
				Robocount++;
				if (Robocount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_ROBOTICSFACILITY);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_ROBOTICSBAY:
				Robobaycount++;
				if (Robobaycount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ROBOTICSBAY))
				{
					Trybuilding(sc2::ABILITY_ID::BUILD_ROBOTICSBAY);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_NEXUS:
				Nexuscount++;
				
				if (Nexuscount > CountUnitType(sc2::UNIT_TYPEID::PROTOSS_NEXUS))
				{
					int i = mbot.bases.getnextexpansion();
					Trybuildingnexus(mbot.bases.allbases[i].x, mbot.bases.allbases[i].y);
					end = true;
				}
				break;
			case sc2::UNIT_TYPEID::PROTOSS_PROBE:
				Probecount++;
				if (Probecount > Requiredprobecount)
				{
					end = true;
				}
				break;
			}


		}
		else
		{
			break;
		}

	}
}



void Buildorder::LoadBuildOrder(const std::string & filename)
{
	char c = ' ';
	std::string currentline;
	std::fstream buildorderfile(filename);
	
	while (std::getline(buildorderfile, currentline))
	{
		int count = 0;
		std::string name = "";
		std::vector<char> line(currentline.c_str(), currentline.c_str() + currentline.size() + 1);
		if (line.size() > 1)
		{
			for (auto&& it : line)
			{
				if (isdigit(it))
				{
					count = count * 10 + it - '0';
				}
				if (isalpha(it))
				{
					name = name + it;
				}
			}
		}

		if (name == "GATEWAY")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
			}
		}
		if (name == "CYBERCORE")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE);
			}
		}
		if (name == "FORGE")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_FORGE);
			}
		}
		if (name == "NEXUS")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_NEXUS);
			}
		}
		if (name == "STARGATE")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_STARGATE);
			}
		}
		if (name == "FLEETBEACON")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_FLEETBEACON);
			}
		}
		if (name == "ROBO")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY);
			}
		}
		if (name == "BAY")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_ROBOTICSBAY);
			}
		}
		if (name == "TWILIGHT")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL);
			}
		}
		if (name == "ARCHIVE")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_TEMPLARARCHIVE);
			}
		}
		if (name == "SHRINE")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_DARKSHRINE);
			}
		}
		if (name == "PROBE")
		{
			for (int i = 0; i < count; i++)
			{
				currentbuildorder.push_back(sc2::UNIT_TYPEID::PROTOSS_PROBE);
			}
		}

	}
}