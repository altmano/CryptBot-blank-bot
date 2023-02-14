#include "MicroManager.h"
#include "Cryptbot.h"



MicroManager::MicroManager(CryptBot & bot) : mbot(bot)
{
}


float MicroManager::calculateavgx(const std::vector< const sc2::Unit *> & units)
{
	float x = 0;
	for (auto&& it : units)
	{
		x = x + it->pos.x;
	}
	x = x / units.size();
	return x;
}

float MicroManager::calculateavgy(const std::vector<const sc2::Unit *> & units)
{
	float y = 0;
	for (auto&& it : units)
	{
		y = y + it->pos.y;
	}
	y = y / units.size();
	return y;
}
void MicroManager::GroupUpAll()
{
	const sc2::ObservationInterface* observation = mbot.Observation();
	const sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Self);
	float x = 0;
	float y = 0;
	int count = 0;

	std::vector<const sc2::Unit *> army;
	for (auto&& it : units)
	{
		if (it->unit_type == sc2::UNIT_TYPEID::PROTOSS_ZEALOT || it->unit_type == sc2::UNIT_TYPEID::PROTOSS_STALKER ||
			it->unit_type == sc2::UNIT_TYPEID::PROTOSS_SENTRY || it->unit_type == sc2::UNIT_TYPEID::PROTOSS_HIGHTEMPLAR ||
			it->unit_type == sc2::UNIT_TYPEID::PROTOSS_IMMORTAL || it->unit_type == sc2::UNIT_TYPEID::PROTOSS_COLOSSUS ||
			it->unit_type == sc2::UNIT_TYPEID::PROTOSS_VOIDRAY || it->unit_type == sc2::UNIT_TYPEID::PROTOSS_PHOENIX ||
			it->unit_type == sc2::UNIT_TYPEID::PROTOSS_CARRIER || it->unit_type == sc2::UNIT_TYPEID::PROTOSS_TEMPEST)
		{
			x = x + it->pos.x;
			y = y + it->pos.y;
			count++;
			army.push_back(it);
		}
	}
	if (count > 1)
	{
		x = x / count;
		y = y / count;
		mbot.Actions()->UnitCommand(army, sc2::ABILITY_ID::ATTACK_ATTACK, sc2::Point2D(x, y));
	}

}

void MicroManager::GroupUpArmy(int i)
{
	if (Armies.size() > i)
	{
		float x = 0;
		float y = 0;
		for (auto&& it : Armies[i].units)
		{
			x = x + it->pos.x;
			y = y + it->pos.y;
		}
		x = x / Armies[i].units.size();
		y = y / Armies[i].units.size();
		Armies[i].x = x;
		Armies[i].y = y;
		mbot.Actions()->UnitCommand(Armies[i].units, sc2::ABILITY_ID::ATTACK, sc2::Point2D(Armies[i].x, Armies[i].y));
	}
	else
	{
		std::cout << "This army does not exist" << std::endl;
	}
}


void MicroManager::AssignNewUnit(const sc2::Unit * unit)
{
	if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_ORACLE || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PHOENIX || 
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_TEMPEST || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR)
	{

		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_ORACLE)
		{
			//std::cout << "Adding oracle" << std::endl;
			if (Oraclegroup.units.size() == 0)
			{
				Oraclegroup.x = unit->pos.x;
				Oraclegroup.y = unit->pos.y;
			}
			else
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::MOVE, sc2::Point2D(Oraclegroup.x,Oraclegroup.y));
			}
			Oraclegroup.units.push_back(unit);
		}
		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PHOENIX)
		{
			//std::cout << "Adding phoenix" << std::endl;
			if (Phoenixgroup.units.size() == 0)
			{
				Phoenixgroup.x = unit->pos.x;
				Phoenixgroup.y = unit->pos.y;
			}
			else
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::MOVE, sc2::Point2D(Phoenixgroup.x, Phoenixgroup.y));
			}
			Phoenixgroup.units.push_back(unit);
		}
		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_TEMPEST)
		{
			if (Tempestgroup.units.size() == 0)
			{
				Tempestgroup.x = unit->pos.x;
				Tempestgroup.y = unit->pos.y;
			}
			else
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::ATTACK, sc2::Point2D(Tempestgroup.x,Tempestgroup.y));
			}
			Tempestgroup.units.push_back(unit);
		}
		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR)
		{
			if (Darktemplargroup.units.size() == 0)
			{
				Darktemplargroup.x = unit->pos.x;
				Darktemplargroup.y = unit->pos.y;
			}
			else
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::MOVE, sc2::Point2D(Darktemplargroup.x, Darktemplargroup.y));
			}
			Darktemplargroup.units.push_back(unit);
		}
	}
	else
	{
		if (Armies.size() == 0)
		{
			ArmyGroup newarmygroup;
			newarmygroup.x = unit->pos.x;
			newarmygroup.y = unit->pos.y;
			newarmygroup.units.push_back(unit);
			Armies.push_back(newarmygroup);
		}
		else
		{
			bool found = false;
			float distance = 1000;

			int chosenarmy = 0;
			for (int i = 0; i < Armies.size(); ++i)
			{
				if (distance > sc2::Distance2D(sc2::Point2D(Armies[i].x, Armies[i].y), unit->pos))
				{
					distance = sc2::Distance2D(sc2::Point2D(Armies[i].x, Armies[i].y), unit->pos);
					chosenarmy = i;
				}
			}
			//std::cout << "Adding unit to army group" << chosenarmy << std::endl;
			Armies[chosenarmy].units.push_back(unit);
			mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::ATTACK_ATTACK, sc2::Point2D(Armies[chosenarmy].x, Armies[chosenarmy].y));
		}
	}
	
}

void MicroManager::RemoveDeadUnit(const sc2::Unit * unit)
{
	if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_ORACLE || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PHOENIX ||
		unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_TEMPEST || unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR)
	{
		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_ORACLE)
		{
			int toremove = 0;
			for (int i = 0; i <  Oraclegroup.units.size(); i++)
			{
				if (Oraclegroup.units[i]->tag == unit->tag)
				{
					toremove = i;
				}
			}
			Oraclegroup.units.erase(Oraclegroup.units.begin() + toremove);
		}

		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_PHOENIX)
		{
			int toremove = 0;
			for (int i = 0; i < Phoenixgroup.units.size(); i++)
			{
				if (Phoenixgroup.units[i]->tag == unit->tag)
				{
					toremove = i;
				}
			}
			Phoenixgroup.units.erase(Phoenixgroup.units.begin() + toremove);
		}

		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR)
		{
			int toremove = 0;
			for (int i = 0; i < Darktemplargroup.units.size(); i++)
			{
				if (Darktemplargroup.units[i]->tag == unit->tag)
				{
					toremove = i;
				}
			}
			Darktemplargroup.units.erase(Darktemplargroup.units.begin() + toremove);
		}

		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_TEMPEST)
		{
			int toremove = 0;
			for (int i = 0; i < Tempestgroup.units.size(); i++)
			{
				if (Tempestgroup.units[i]->tag == unit->tag)
				{
					toremove = i;
				}
			}
			Tempestgroup.units.erase(Tempestgroup.units.begin() + toremove);
		}
	}
	else
	{
		for (auto&& it : Armies)
		{
			bool unitfound = false;
			int toremove = 0;
			for (int i = 0; i < it.units.size(); i++)
			{
				if (it.units[i]->tag == unit->tag)
				{
					unitfound = true;
					toremove = i;
					
				}
			}
			if (unitfound)
			{
				it.units.erase(it.units.begin() + toremove);
				break;
			}
		}
	}
}

void MicroManager::LaunchAttack()
{
	const sc2::GameInfo& gameinfo = mbot.Observation()->GetGameInfo();

	for (auto&& it : Armies)
	{
		if (it.attacking == false)
		{
			it.attacking = true;
			mbot.Actions()->UnitCommand(it.units, sc2::ABILITY_ID::ATTACK_ATTACK, gameinfo.enemy_start_locations.front());

		}
	}
}

void MicroManager::UpdateArmyLocations()
{
	for (auto&& it : Armies)
	{

		it.x = calculateavgx(it.units);
		it.y = calculateavgy(it.units);
	}
	if (Phoenixgroup.units.size() > 0)
	{

		Phoenixgroup.x = calculateavgx(Phoenixgroup.units);
		Phoenixgroup.y = calculateavgy(Phoenixgroup.units);
	}
	if (Oraclegroup.units.size() > 0)
	{
		Oraclegroup.x = calculateavgx(Oraclegroup.units);
		Oraclegroup.y = calculateavgy(Oraclegroup.units);
	}
	if (Darktemplargroup.units.size() > 0)
	{
		Darktemplargroup.x = calculateavgx(Darktemplargroup.units);
		Darktemplargroup.y = calculateavgy(Darktemplargroup.units);
	}
	if (Tempestgroup.units.size() > 0)
	{
		Tempestgroup.x = calculateavgx(Tempestgroup.units);
		Tempestgroup.y = calculateavgx(Tempestgroup.units);
	}
}

void MicroManager::SelectTarget()
{
	const sc2::Units enemyunits = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Enemy);
	if (enemyunits.size() > 0)
	{
		//std::cout << "Number of enemy units:" << enemyunits.size() << std::endl;
		for (auto&& it : Armies)
		{
			float distance = 1000;
			int selectedtarget = 0;
			if (it.attacking)
			{
				for (int i = 0; i < enemyunits.size(); i++)
				{
					if (distance > sc2::Distance2D(enemyunits[i]->pos, sc2::Point2D(it.x, it.y)))
					{
						selectedtarget = i;
						distance = sc2::Distance2D(enemyunits[i]->pos, sc2::Point2D(it.x, it.y));
					}
				}
				mbot.Actions()->UnitCommand(it.units, sc2::ABILITY_ID::ATTACK_ATTACK, enemyunits[selectedtarget]->pos);
			}
			else
			{
				const sc2::Units nexuses = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(sc2::UNIT_TYPEID::PROTOSS_NEXUS));
				float x = 100;
				float y = 100;
				if (nexuses.size() > 0)
				{
					x = calculateavgx(nexuses);
					y = calculateavgy(nexuses);
				}
				bool foundvalidtarget = false;
				int target = 0;
				float distance = 1000;
				for (int i = 0; i < enemyunits.size(); i++)
				{
					if ((distance > sc2::Distance2D(enemyunits[i]->pos, sc2::Point2D(it.x, it.y))) &&
						(sc2::Distance2D(sc2::Point2D(x, y), enemyunits[i]->pos) < 80))
					{
						foundvalidtarget = true;
						distance = sc2::Distance2D(enemyunits[i]->pos, sc2::Point2D(it.x, it.y));
						//std::cout << "Found valid target at a distance of " << sc2::Distance2D(sc2::Point2D(x, y), enemyunits[i]->pos) << std::endl;
						target = i;
					}
				}
				if (foundvalidtarget)
				{
					mbot.Actions()->UnitCommand(it.units, sc2::ABILITY_ID::ATTACK_ATTACK, enemyunits[target]->pos);
				}
			}
		}
	}
	else
	{
		for (auto && it : Armies)
		{
			if (it.attacking)
			{
				//std::cout << "Attacking enemy base" << std::endl;
				const sc2::GameInfo& gameinfo = mbot.Observation()->GetGameInfo();
				mbot.Actions()->UnitCommand(it.units, sc2::ABILITY_ID::ATTACK_ATTACK, gameinfo.enemy_start_locations.front());
			}
			else
			{
				mbot.Actions()->UnitCommand(it.units, sc2::ABILITY_ID::MOVE, sc2::Point2D(it.x, it.y));
			}
		}
	}
}

void MicroManager::MicroOracles()
{
	if (Oraclegroup.units.size() > 0)
	{
		for (auto && it : Oraclegroup.units)
		{
			if (it->shield > 40 && it->energy > 30)
			{
				bool tooclose = false;
				auto missileturrets = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Enemy, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_MISSILETURRET));
				for (auto&& it2 : missileturrets)
				{
					if (sc2::Distance3D(it2->pos, it->pos) < 7)
					{
						tooclose = true;
						
						float xdiff = it2->pos.x - it->pos.x;
						float ydiff = it2->pos.y - it->pos.y;
						mbot.Actions()->UnitCommand(it, sc2::ABILITY_ID::MOVE, sc2::Point2D((it->pos.x - xdiff), (it->pos.y - ydiff)));
					}

				}
				if (tooclose == false)
				{
					auto enemyworkers = mbot.Observation()->GetUnits(sc2::Unit::Alliance::Enemy, sc2::IsUnit(sc2::UNIT_TYPEID::TERRAN_SCV));
					float mindistance = 1000;
					int targetSCV = 0;
					for (int i = 0; i < enemyworkers.size(); i++)
					{
						if (sc2::Distance2D(enemyworkers[i]->pos, it->pos) < mindistance)
						{
							mindistance = sc2::Distance2D(enemyworkers[i]->pos, it->pos);
							targetSCV = i;
						}
					}
					//std::cout << "MIN distance is " << mindistance << std::endl;
					if (mindistance < 7)
					{
						//std::cout << "Attacking SCV" << std::endl;
						auto buf = it->buffs;
						bool weaponactive = false;
						for (auto && it3 : buf)
						{
							if (it3 == sc2::BUFF_ID::ORACLEWEAPON)
							{
								weaponactive = true;
							}
						}
						if (weaponactive)
						{
							
							mbot.Actions()->UnitCommand(it, sc2::ABILITY_ID::ATTACK, enemyworkers[targetSCV]);
						}
						else
						{
							mbot.Actions()->UnitCommand(it, sc2::ABILITY_ID::BEHAVIOR_PULSARBEAMON);
						}
						
						
					}
					else
					{
						//std::cout << "Moving to harras" << std::endl;
						const sc2::GameInfo& gameinfo = mbot.Observation()->GetGameInfo();
						mbot.Actions()->UnitCommand(it, sc2::ABILITY_ID::MOVE, gameinfo.enemy_start_locations.front());
					}
				
				}
			}
		}
	}
}
void MicroManager::EnenmyUnits()
{
	const sc2::ObservationInterface* observation = mbot.Observation();
	const sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Enemy);
	std::cout << "Listing enemy units" << std::endl;
	for (auto&& it : units)
	{
		if (it->unit_type == sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER)
		{
			std::cout << "Command center" << std::endl;
		}
		if (it->unit_type == sc2::UNIT_TYPEID::TERRAN_BARRACKS)
		{
			std::cout << " Barracks" << std::endl;
		}
		if (it->unit_type == sc2::UNIT_TYPEID::TERRAN_SCV)
		{
			std::cout << " SCV" << std::endl;
		}
		if (it->unit_type == sc2::UNIT_TYPEID::TERRAN_MARINE)
		{
			std::cout << "Marine" << std::endl;
		}
	}
}