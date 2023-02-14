#include "Scouting.h"
#include "Cryptbot.h"


Scouting::Scouting(CryptBot & bot) : mbot(bot),
    strategyadaptation(bot)
{
}

void Scouting::Enemystructurescheck()
{
	auto barracks = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Enemy, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_BARRACKS));
	auto factories = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Enemy, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_FACTORY));
	auto starports = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Enemy, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_STARPORT));
	if (barracks.size() > 2 || factories.size() > 2 || starports.size() > 1)
	{
		if (barracks.size() > factories.size() + starports.size())
		{
			if (bioproduction == false)
			{
				mechproduction = false;
				airproduction = false;
				bioproduction = true;
				strategyadaptation.DetectedBio();
			}
		}

		if (factories.size() > 2 && barracks.size() < factories.size())
		{
			if (mechproduction == false)
			{
				bioproduction = false;
				airproduction = false;
				mechproduction = true;
				strategyadaptation.DetectedMech();
			}
		}

		if (starports.size() > barracks.size() + factories.size() || starports.size() > 3)
		{
			if (airproduction == false)
			{
				bioproduction = false;
				mechproduction = false;
				airproduction = true;
				strategyadaptation.DetectedAir();
			}

		}
	}
}

void Scouting::Enemyarmycheck()
{
	auto enemyunits = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Enemy);
	int biounitcount = mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_MARINE) + mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_MARAUDER)
		+ mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_GHOST) + mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_MEDIVAC);
	int mechunitcount = mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_HELLION) + mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_WIDOWMINE)
		+ mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_SIEGETANK) + mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_CYCLONE)
		+ mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_THOR);
	int airunitcount = mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_VIKINGFIGHTER) + mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_VIKINGASSAULT)
		+ mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_RAVEN) + mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_LIBERATOR)
		+ mbot.CountUnitType(sc2::UNIT_TYPEID::TERRAN_BATTLECRUISER);

	if ((mechunitcount + biounitcount + airunitcount) > 20)
	{
		if (biounitcount > mechunitcount + airunitcount)
		{
			if (bioproduction == false)
			{
				mechproduction = false;
				airproduction = false;
				bioproduction = true;
				aggroproduction = false;
				strategyadaptation.DetectedBio();
			}
		}
		else
		{
			if (mechunitcount > biounitcount + airunitcount)
			{
				if (mechproduction == false)
				{
					bioproduction = false;
					airproduction = false;
					mechproduction = true;
					aggroproduction = false;
					strategyadaptation.DetectedMech();
				}
			}
			else
			{
				if (airunitcount > biounitcount + mechunitcount)
				{
					if (airproduction == false)
					{
						bioproduction = false;
						mechproduction = false;
						airproduction = true;
						aggroproduction = false;
						strategyadaptation.DetectedAir();
					}
				}
			}
		}
	}
	if ((mechunitcount + biounitcount + airunitcount) > 12 && mbot.CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PROBE) < 24)
	{
		if (aggroproduction == false)
		{
			bioproduction = false;
			mechproduction = false;
			airproduction = false;
			aggroproduction = true;
			strategyadaptation.DetectedAggro();
		}

	}
}

void Scouting::CheckProbeScout()
{
	if (mbot.CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PROBE) == firstprobescouttiming && firstprobescoutsent == false)
	{
		firstprobescoutsent = true;
		SendProbeScout();
	}
	if (mbot.CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PROBE) == secondprobescouttiming)
	{
		secondprobescoutsent = true;
		SendProbeScout();
	}
}

void Scouting::SendProbeScout()
{
	auto& probes = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::PROTOSS_PROBE));
	const sc2::GameInfo& gameinfo = mbot.Observation()->GetGameInfo();
	mbot.Actions()->UnitCommand(probes.front(), sc2::ABILITY_ID::MOVE, gameinfo.enemy_start_locations.front());
}