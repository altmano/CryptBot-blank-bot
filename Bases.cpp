#include "Bases.h"

#include "Cryptbot.h"
Bases::Bases(CryptBot & bot)
	:mbot (bot)
	
{
}

void Bases::updatefields() 
{
	const sc2::ObservationInterface* observation = mbot.Observation();
	const sc2::Units units = observation->GetUnits(sc2::Unit::Neutral);
	for (const auto& neutralunit : units) 
	{
		if (neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD ||
			neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_LABMINERALFIELD ||
			neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD ||
			neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_LABMINERALFIELD750 ||
			neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD750 ||
			neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD750
			)
		{
			mineralfields.push_back(neutralunit);
		}
		else 
		{
			if (neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_SPACEPLATFORMGEYSER ||
				neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_PROTOSSVESPENEGEYSER ||
				neutralunit->unit_type == sc2::UNIT_TYPEID::NEUTRAL_VESPENEGEYSER)
			{
				vespinegeysers.push_back(neutralunit);
			}
			else 
			{

			}
		}
	}
	sorttobases();
}

void Bases::sorttobases()
{
	
	while (vespinegeysers.size() > 0)
	{
		singlebase newbase;
		int lowest;
		double shortestdistance = 9999.9;
		for (int i = 1; i < vespinegeysers.size(); i++)
		{
			if (sc2::Distance2D(vespinegeysers[0]->pos, vespinegeysers[i]->pos) < shortestdistance)
			{
				shortestdistance = sc2::Distance2D(vespinegeysers[0]->pos, vespinegeysers[i]->pos);
				lowest = i;
			}
		}
		newbase.gasses.push_back(vespinegeysers[0]);
		newbase.gasses.push_back(vespinegeysers[lowest]);
		vespinegeysers.erase(vespinegeysers.begin() + lowest);
		vespinegeysers.erase(vespinegeysers.begin());
		
		for (int i = 0; i < 8; i++)
		{
			double shortestdistance = 9999.9;
			int j = 0;
			int k = 0;
			for (auto& it : mineralfields)
			{
				if ((sc2::Distance2D(newbase.gasses[0]->pos, it->pos) + sc2::Distance2D(newbase.gasses[1]->pos, it->pos)) / 2 < shortestdistance)
				{
					shortestdistance = (sc2::Distance2D(newbase.gasses[0]->pos, it->pos) + sc2::Distance2D(newbase.gasses[1]->pos, it->pos)) / 2;
					k = j;
				}
				j++;
			}
			newbase.minerals.push_back(mineralfields[k]);
			mineralfields.erase(mineralfields.begin() + k);
		}
		double sumx = 0;
		double sumy = 0;
		for (auto& it : newbase.minerals)
		{
			sumx += it->pos.x;
			sumy += it->pos.y;
		}
		sumx = sumx / 8;
		sumy = sumy / 8;
		double gasx = (newbase.gasses[0]->pos.x + newbase.gasses[1]->pos.x) / 2;
		double gasy = (newbase.gasses[0]->pos.y + newbase.gasses[1]->pos.y) / 2;
		std::cout << "Base: " << gasx << " "<< gasy << " " << sc2::Distance2D(newbase.gasses[0]->pos, newbase.gasses[1]->pos) << std::endl;
		if (std::abs(newbase.gasses[0]->pos.x - newbase.gasses[1]->pos.x) < 2 || std::abs(newbase.gasses[0]->pos.y - newbase.gasses[1]->pos.y) < 2)
		{
			if (std::abs(newbase.gasses[0]->pos.x - newbase.gasses[1]->pos.x) < 2)
			{
				if (gasx > sumx)
				{
					newbase.y = gasy ;
					newbase.x = gasx + 2;
				}
				else
				{
					newbase.y = gasy ;
					newbase.x = gasx - 2;
				}
			}
			else
			{
				if (gasy > sumy)
				{
					newbase.x = gasx;
					newbase.y = gasy + 2;
				}
				else
				{
					newbase.x = gasx ;
					newbase.y = gasy - 2;
				}
			}
		}
		else 
		{
			if (gasx > sumx && gasy > sumy)
			{
				newbase.x = gasx + 2;
				newbase.y = gasy + 2;
			}
			else
			{
				if (gasx > sumx && gasy < sumy)
				{
					if (sc2::Distance2D(newbase.gasses[0]->pos, newbase.gasses[1]->pos) < 14.2)
					{
						newbase.x = gasx + 2;
						newbase.y = gasy - 2;
					}
					else
					{
						newbase.x = gasx + 2;
						newbase.y = gasy - 3;
					}

				}
				else
				{
					if (gasy < sumy)
					{
						if (sc2::Distance2D(newbase.gasses[0]->pos, newbase.gasses[1]->pos) < 14.2)
						{
							newbase.x = gasx - 2;
							newbase.y = gasy - 2;
						}
						else
						{
							newbase.x = gasx - 2;
							newbase.y = gasy - 3;
						}
					
					}
					else
					{
						newbase.x = gasx - 3;
						newbase.y = gasy + 2;
					}
				}
			}
		}
		
		

		allbases.push_back(newbase);
	}

}

int Bases::getnextexpansion()
{
	double distance = 999.9;
	int result;
	const sc2::ObservationInterface* observation = mbot.Observation();
	const sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Self);
	std::vector<const sc2::Unit*> currentbases;

	for (auto& unit : units)
	{
		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_NEXUS)
		{
			currentbases.push_back(unit);
		}
	}
	for (int i = 0; i < allbases.size(); i++)
	{
		bool free = true;
		for (auto& it : currentbases)
		{
			if (sc2::Distance2D(sc2::Point2D(allbases[i].x, allbases[i].y), it->pos) < 4)
			{
				free = false;
			}

		}
		if (currentbases.size() > 0)
		{
			if (sc2::Distance2D(sc2::Point2D(allbases[i].x, allbases[i].y), currentbases[0]->pos) < distance && free)
			{
				distance = sc2::Distance2D(sc2::Point2D(allbases[i].x, allbases[i].y), currentbases[0]->pos);
				result = i;
			}
		}
		else
		{
			result = 0;
		}
		

	}
	return result;

}

