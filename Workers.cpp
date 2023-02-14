#include "Workers.h"
#include "Cryptbot.h"


Workers::Workers(CryptBot & bot):
	mbot(bot)

{
}

void Workers::GiveJobs()
{
	const sc2::ObservationInterface* observation = mbot.Observation();
	const sc2::Units & myunits = observation->GetUnits(sc2::Unit::Alliance::Self);
	const sc2::Units & neutralunits = observation->GetUnits(sc2::Unit::Neutral);
	std::vector<const sc2::Unit *> saturatednexuses;
	std::vector<int> nexusexcess;
	std::vector< const sc2::Unit *> unsaturatednexuses;
	std::vector<int> nexusdeficit;
	std::vector<const sc2::Unit *> unsaturatedassimilators;
	std::vector<int> assimilatordeficit;
	std::vector<const sc2::Unit *> probes;
	for (auto&& it : myunits)
	{
		if (it->unit_type == sc2::UNIT_TYPEID::PROTOSS_NEXUS)
		{
			if (it->assigned_harvesters > it->ideal_harvesters)
			{
				saturatednexuses.push_back(it);
				nexusexcess.push_back(it->assigned_harvesters - it->ideal_harvesters);


			}
			else
			{
				if (it->assigned_harvesters < it->ideal_harvesters)
				{
					unsaturatednexuses.push_back(it);
					nexusdeficit.push_back(it->ideal_harvesters - it->assigned_harvesters);
				}
			}
		}
		if (it->unit_type == sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR)
		{
			if (it->assigned_harvesters < it->ideal_harvesters)
			{
				unsaturatedassimilators.push_back(it);
				assimilatordeficit.push_back(it->ideal_harvesters - it->assigned_harvesters);
			}
		}
		if (it->unit_type == sc2::UNIT_TYPEID::PROTOSS_PROBE)
		{
			probes.push_back(it);
		}
	}

	for (int i = 0; i < saturatednexuses.size(); i++)
	{
		for (int j = 0; j < unsaturatedassimilators.size(); j++)
		{
			if (sc2::Distance2D(saturatednexuses[i]->pos, unsaturatedassimilators[j]->pos) < 10)
			{
				int failsafe = 10;
				while (nexusexcess[i] > 0 &&  assimilatordeficit[j] > 0 && failsafe > 0)
				{
					for (auto&& it : probes)
					{
						if (sc2::Distance2D(it->pos, saturatednexuses[i]->pos) < 4 && sc2::Distance2D(it->pos, unsaturatedassimilators[j]->pos) > 2)
						{
							mbot.Actions()->UnitCommand(it, sc2::ABILITY_ID::HARVEST_GATHER, unsaturatedassimilators[j]);
							nexusexcess[i]--;
							assimilatordeficit[j]--;
							break;
						}
					}
					failsafe--;
				}
			}
		}
		for (int j = 0; j < unsaturatednexuses.size(); j++)
		{
			int failsafe = 10;
			while (nexusexcess[i] > 0 && nexusdeficit[j] > 0 && failsafe > 0)
			{
				for (auto&& it : probes)
				{
					if (sc2::Distance2D(it->pos, saturatednexuses[i]->pos) < 5)
					{
						int distance = 1000;
						const sc2::Unit * closestmineral = neutralunits[0];
						for (auto&& it2 : neutralunits)
						{
							if (it2->unit_type == sc2::UNIT_TYPEID::NEUTRAL_LABMINERALFIELD ||
								it2->unit_type == sc2::UNIT_TYPEID::NEUTRAL_LABMINERALFIELD750 ||
								it2->unit_type == sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD ||
								it2->unit_type == sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD750 ||
								it->unit_type == sc2::UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD ||
								it2->unit_type == sc2::UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD750
								)
							{
								if (sc2::Distance2D(it2->pos, unsaturatednexuses[j]->pos) < distance)
								{
									distance = sc2::Distance2D(it2->pos, unsaturatednexuses[j]->pos);
									closestmineral = it2;
								}
							}
						}
						mbot.Actions()->UnitCommand(it, sc2::ABILITY_ID::HARVEST_GATHER,closestmineral);
						nexusexcess[i]--;
						nexusdeficit[j]--;
						break;
					}
				}
				failsafe--;
			}
		}
	}


}
