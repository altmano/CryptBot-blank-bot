#include "UnitProduction.h"
#include "Cryptbot.h"


UnitProduction::UnitProduction(CryptBot & bot) : mbot(bot)
{
}

size_t UnitProduction::CountUnitType(sc2::UNIT_TYPEID unit_type)
{
	return mbot.Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(unit_type)).size();

}

void UnitProduction::CreateGatewayUnit(sc2::UNIT_TYPEID which_unit, const sc2::Unit *unit)
{
	if (unit->is_alive)
	{
		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_GATEWAY)
		{
			if (which_unit == sc2::UNIT_TYPEID::PROTOSS_ZEALOT)
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_ZEALOT);

			}
			if (which_unit == sc2::UNIT_TYPEID::PROTOSS_STALKER)
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_STALKER);
			}
			if (which_unit == sc2::UNIT_TYPEID::PROTOSS_SENTRY)
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_SENTRY);
			}
			if (which_unit == sc2::UNIT_TYPEID::PROTOSS_HIGHTEMPLAR)
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_HIGHTEMPLAR);
			}
			if (which_unit == sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR)
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_DARKTEMPLAR);
			}
		}
		if (unit->unit_type == sc2::UNIT_TYPEID::PROTOSS_WARPGATE)
		{
			//std::cout << "Warping in" << std::endl;
			auto Powersources = mbot.Observation()->GetPowerSources();
			const sc2::GameInfo& gameinfo = mbot.Observation()->GetGameInfo();
			if (Powersources.size() > 0)
			{

				float distance = 1000;
				int chosenpowersource = 0;
				for (int i = 0; i < Powersources.size(); i++)
				{
					if (distance > sc2::Distance2D(Powersources[i].position, gameinfo.enemy_start_locations.front()))
					{
						distance = sc2::Distance2D(Powersources[i].position, gameinfo.enemy_start_locations.front());
						chosenpowersource = i;
					}
				}
				float rx = sc2::GetRandomScalar();
				float ry = sc2::GetRandomScalar();
				if (which_unit == sc2::UNIT_TYPEID::PROTOSS_ZEALOT)
				{
					mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAINWARP_ZEALOT, sc2::Point2D(Powersources[chosenpowersource].position.x + 2 * rx, Powersources[chosenpowersource].position.y + 2 * ry));

				}
				if (which_unit == sc2::UNIT_TYPEID::PROTOSS_STALKER)
				{
					mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAINWARP_STALKER, sc2::Point2D(Powersources[chosenpowersource].position.x + 2 * rx, Powersources[chosenpowersource].position.y + 2 * ry));
				}
				if (which_unit == sc2::UNIT_TYPEID::PROTOSS_SENTRY)
				{
					mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAINWARP_SENTRY, sc2::Point2D(Powersources[chosenpowersource].position.x + 2 * rx, Powersources[chosenpowersource].position.y + 2 * ry));
				}
				if (which_unit == sc2::UNIT_TYPEID::PROTOSS_HIGHTEMPLAR)
				{
					mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAINWARP_HIGHTEMPLAR, sc2::Point2D(Powersources[chosenpowersource].position.x + 2 * rx, Powersources[chosenpowersource].position.y + 2 * ry));
				}
				if (which_unit == sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR)
				{
					mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAINWARP_DARKTEMPLAR, sc2::Point2D(Powersources[chosenpowersource].position.x + 2 * rx, Powersources[chosenpowersource].position.y + 2 * rx));
				}
			}

		}
	}
	
}
void UnitProduction::ProduceFromGate(const sc2::Unit *unit)
{
	if (unit->is_alive)
	{
		if (Currentgatewayproduction < Gatewayproductionratios.size())
		{
			if (Currentgatewayproduction < Gatewayproductionratios.size() - 1)
			{
				if (Gatewayproductionratios[Currentgatewayproduction].Transitionunitcount <= CountUnitType(Gatewayproductionratios[Currentgatewayproduction].Transitionunit))
				{
					//std::cout << "Changing production plan" << std::endl;
					std::cout << Gatewayproductionratios[Currentgatewayproduction].Transitionunitcount << CountUnitType(Gatewayproductionratios[Currentgatewayproduction].Transitionunit) << std::endl;


					Currentgatewayproduction++;
				}
			}
			float gatewayunitcount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ZEALOT) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_STALKER) +
				CountUnitType(sc2::UNIT_TYPEID::PROTOSS_SENTRY) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_HIGHTEMPLAR) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR);

			if (gatewayunitcount == 0)
			{
				gatewayunitcount = 1;
			}

			float realstalkerratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_STALKER) / gatewayunitcount;
			float realzealotratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ZEALOT) / gatewayunitcount;
			float realsentryratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_SENTRY) / gatewayunitcount;
			float realhightemplarratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_HIGHTEMPLAR) / gatewayunitcount;
			float realdarktemplarratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR) / gatewayunitcount;
			//std::cout << "Real zealot ratio: " << realzealotratio << "Desired zealot ratio: " << Gatewayproductionratios[Currentgatewayproduction].Zealotratio << std::endl;
			//std::cout << "Real stalker ratio: " << realstalkerratio << "Desired stalker ratio: " << Gatewayproductionratios[Currentgatewayproduction].Stalkerratio << std::endl;
			if (realzealotratio <= Gatewayproductionratios[Currentgatewayproduction].Zealotratio)
			{
				CreateGatewayUnit(sc2::UNIT_TYPEID::PROTOSS_ZEALOT, unit);
				//	std::cout << "Producting zealot" << std::endl;

			}
			else
			{
				if (realstalkerratio <= Gatewayproductionratios[Currentgatewayproduction].Stalkerratio)
				{
					CreateGatewayUnit(sc2::UNIT_TYPEID::PROTOSS_STALKER, unit);
					//		std::cout << "Producing stalker" << std::endl;
				}
				else
				{
					if (realsentryratio <= Gatewayproductionratios[Currentgatewayproduction].Sentryratio)
					{
						CreateGatewayUnit(sc2::UNIT_TYPEID::PROTOSS_SENTRY, unit);
						//		std::cout << "Producing sentry" << std::endl;
					}
					else
					{
						if (realsentryratio <= Gatewayproductionratios[Currentgatewayproduction].Hightemplarrratio)
						{
							CreateGatewayUnit(sc2::UNIT_TYPEID::PROTOSS_HIGHTEMPLAR, unit);
							//		std::cout << "Producting High Templar" << std::endl;
						}
						else
						{
							CreateGatewayUnit(sc2::UNIT_TYPEID::PROTOSS_DARKTEMPLAR, unit);
							//		std::cout << "Producing Dark Templar" << std::endl;
						}
					}
				}
			}

		}
	}

}


void UnitProduction::ProduceFromRobo(const sc2::Unit *unit)
{
	if (unit->is_alive)
	{
		if (Currentroboproduction < Roboproductionratios.size())
		{
			if (Currentroboproduction < Roboproductionratios.size() - 1)
			{
				if (Roboproductionratios[Currentroboproduction].Transitionunitcount <= CountUnitType(Roboproductionratios[Currentroboproduction].Transitionunit))
				{
					Currentroboproduction++;
				}
			}
			float robounitcount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_OBSERVER) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_WARPPRISM) +
				CountUnitType(sc2::UNIT_TYPEID::PROTOSS_IMMORTAL) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_COLOSSUS) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_DISRUPTOR);
			if (robounitcount == 0)
			{
				robounitcount = 1;
			}
			float realobservercount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_OBSERVER) / robounitcount;
			float realwarpprismcount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_WARPPRISM) / robounitcount;
			float realimmortalcount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_IMMORTAL) / robounitcount;
			float realcolossuscount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_COLOSSUS) / robounitcount;
			float realdisruptorcount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_DISRUPTOR) / robounitcount;

			if (realobservercount < Roboproductionratios[Currentroboproduction].Observerratio
				|| Roboproductionratios[Currentroboproduction].Observerratio == 1)
			{
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_OBSERVER);
			}
			else
			{
				if (realwarpprismcount < Roboproductionratios[Currentroboproduction].Warpprismratio
					|| Roboproductionratios[Currentroboproduction].Warpprismratio == 1)
				{
					mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_WARPPRISM);
				}
				else
				{
					if (realimmortalcount < Roboproductionratios[Currentroboproduction].Immortalratio ||
						Roboproductionratios[Currentroboproduction].Immortalratio == 1)
					{
						mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_IMMORTAL);
					}
					else
					{
						if (realcolossuscount < Roboproductionratios[Currentroboproduction].Colossusratio ||
							Roboproductionratios[Currentroboproduction].Colossusratio == 1)
						{
							mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_COLOSSUS);
						}
						else
						{
							mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_DISRUPTOR);
						}

					}

				}
			}
				

			
		}
	}
}


void UnitProduction::ProduceFromStargate(const sc2::Unit * unit)
{
	if (true)
	{
		if (Currentstargateproduction < Stargateproductionratios.size())
		{
			if (Currentstargateproduction < Stargateproductionratios.size() - 1)
			{
				if (Stargateproductionratios[Currentstargateproduction].Transitionunitcount <= CountUnitType(Stargateproductionratios[Currentstargateproduction].Transitionunit))
				{
					Currentstargateproduction++;
				}
			}
			float stargateunitcount = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PHOENIX) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ORACLE) +
				CountUnitType(sc2::UNIT_TYPEID::PROTOSS_VOIDRAY) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_CARRIER) + CountUnitType(sc2::UNIT_TYPEID::PROTOSS_TEMPEST);
			if (stargateunitcount == 0)
			{
				stargateunitcount = 1;
			}
			float realphoenixratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_PHOENIX) / stargateunitcount;
			float realoracleratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_ORACLE) / stargateunitcount;
			float realvoidrayratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_VOIDRAY) / stargateunitcount;
			float realcarrierratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_CARRIER) / stargateunitcount;
			float realtemperstratio = CountUnitType(sc2::UNIT_TYPEID::PROTOSS_TEMPEST) / stargateunitcount;
			if (realphoenixratio < Stargateproductionratios[Currentstargateproduction].Phoenixratio ||
				Stargateproductionratios[Currentstargateproduction].Phoenixratio == 1)
			{
				//std::cout << "Producing phoenix" << std::endl;
				mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_PHOENIX);
			}
			else
			{
				if (realoracleratio < Stargateproductionratios[Currentstargateproduction].Oracleratio ||
					Stargateproductionratios[Currentstargateproduction].Oracleratio == 1)
				{
					//std::cout << "Producing Oracle" << std::endl;
					mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_ORACLE);
				}
				else
				{
					if (realvoidrayratio < Stargateproductionratios[Currentstargateproduction].Voidrayratio ||
						Stargateproductionratios[Currentstargateproduction].Voidrayratio == 1)
					{
						mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_VOIDRAY);
					}
					else
					{
						if (realcarrierratio < Stargateproductionratios[Currentstargateproduction].Carrierratio ||
							Stargateproductionratios[Currentstargateproduction].Carrierratio == 1 )
						{
							mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_CARRIER);
						}
						else
						{
							mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_TEMPEST);
						}
					}
				}
			}
				
			
		}
	}
}


void UnitProduction::LoadGatewayProduction(const std::string & filename)
{
	std::string currentline;
	std::ifstream Gatewayproductionfile(filename);
	if (Gatewayproductionfile.is_open())
	{
		Gatewayproductionratios.clear();
		while (std::getline(Gatewayproductionfile, currentline))
		{
			std::vector<char> line(currentline.c_str(), currentline.c_str() + currentline.size() + 1);
			std::vector<int> counts;
			std::string name = "";
			int number = 0;
			bool readingnumber = false;
			bool readingname = false;

			for (auto && it : line)
			{
				if (isspace(it) && readingnumber)
				{

					counts.push_back(number);
					number = 0;
					readingnumber = false;

				}
				if (isdigit(it))
				{
					readingnumber = true;
					number = number * 10 + it - '0';

				}
				if (isalpha(it))
				{
					readingname = true;
					name = name + it;

				}
			}

			if (readingname == false)
			{
				std::cout << "Missing name of gateway transition unit" << std::endl;
			}
			else
			{


				Gatewayproduction addedgatewayproduction;
				std::cout << name << std::endl;
				addedgatewayproduction.Transitionunit = Identifyunit(name);
				float total = 0;
				for (int i = 0; i < counts.size() - 1; i++)
				{
					total = total + counts[i];
				}
				if (counts.size() > 5)
				{
					addedgatewayproduction.Zealotratio = counts[0] / total;
					addedgatewayproduction.Stalkerratio = counts[1] / total;
					addedgatewayproduction.Sentryratio = counts[2] / total;
					addedgatewayproduction.Hightemplarrratio = counts[3] / total;
					addedgatewayproduction.Darktemplarratio = counts[4] / total;
					addedgatewayproduction.Transitionunitcount = counts[5];
					Gatewayproductionratios.push_back(addedgatewayproduction);
				}
				else
				{
					std::cout << "Not enough numbers entered to gateway production" << std::endl;
				}



			}
		}
	}
	

}

void UnitProduction::LoadRoboProduction(const std::string & filename)
{
	std::string currentline;
	std::ifstream Roboproductionfile(filename);
	if (Roboproductionfile.is_open())
	{
		Roboproductionratios.clear();
		while (std::getline(Roboproductionfile, currentline))
		{
			std::vector<char> line(currentline.c_str(), currentline.c_str() + currentline.size() + 1);
			std::vector<int> counts;
			std::string name;
			int number = 0;
			bool readingnumber = false;
			bool readingname = false;

			for (auto && it : line)
			{
				if (isspace(it) && readingnumber)
				{

					counts.push_back(number);
					number = 0;
					readingnumber = false;

				}
				if (isdigit(it))
				{
					readingnumber = true;
					number = number * 10 + it - '0';

				}
				if (isalpha(it))
				{
					readingname = true;
					name = name + it;
				}
			}

			if (readingname == false)
			{
				std::cout << "Missing name of gateway transition unit" << std::endl;
			}
			else
			{
				Roboticsproduction addedroboproduction;
				addedroboproduction.Transitionunit = Identifyunit(name);
				float total = 0;
				for (int i = 0; i < counts.size() - 1; i++)
				{
					total = total + counts[i];
				}
				if (counts.size() > 5)
				{
					//std::cout << "New production plan" << std::endl;
					addedroboproduction.Observerratio = counts[0] / total;
					//std::cout << "Observer ratio " << addedroboproduction.Observerratio << std::endl;
					addedroboproduction.Warpprismratio = counts[1] / total;
					//std::cout << "Warp prism ratio " << addedroboproduction.Warpprismratio << std::endl;
					addedroboproduction.Immortalratio = counts[2] / total;
					//std::cout << "Immortal ratio " << addedroboproduction.Immortalratio << std::endl;
					addedroboproduction.Colossusratio = counts[3] / total;
					//std::cout << "Colossus ratio " << addedroboproduction.Colossusratio << std::endl;
					addedroboproduction.Disruptorratio = counts[4] / total;
					//std::cout << "Dissruptor ratio " << addedroboproduction.Observerratio << std::endl;
					addedroboproduction.Transitionunitcount = counts[5];
					Roboproductionratios.push_back(addedroboproduction);
				}
				else
				{
					std::cout << "Not enough numbers entered to robo production" << std::endl;
				}
			}
		}
	}
	


}

void UnitProduction::LoadStargateProduction(const std::string & filename)
{
	std::string currentline;
	std::ifstream Stargateproductionfile(filename);
	if (Stargateproductionfile.is_open())
	{
		while (std::getline(Stargateproductionfile, currentline))
		{
			std::vector<char> line(currentline.c_str(), currentline.c_str() + currentline.size() + 1);
			std::vector<int> counts;
			std::string name;
			int number = 0;
			bool readingnumber = false;
			bool readingname = false;

			for (auto && it : line)
			{
				if (isspace(it) && readingnumber)
				{

					counts.push_back(number);
					number = 0;
					readingnumber = false;

				}
				if (isdigit(it))
				{
					readingnumber = true;
					number = number * 10 + it - '0';

				}
				if (isalpha(it))
				{
					readingname = true;
					name = name + it;
				}
			}


			if (readingname == false)
			{
				std::cout << "Missing name of gateway transition unit" << std::endl;
			}
			else
			{
				Stargateproduction addedstargateproduction;
				addedstargateproduction.Transitionunit = Identifyunit(name);
				float total = 0;
				for (int i = 0; i < counts.size() - 1; i++)
				{
					total = total + counts[i];
				}
				if (counts.size() > 5)
				{
					addedstargateproduction.Phoenixratio = counts[0] / total;
					addedstargateproduction.Oracleratio = counts[1] / total;
					addedstargateproduction.Voidrayratio = counts[2] / total;
					addedstargateproduction.Carrierratio = counts[3] / total;
					addedstargateproduction.Tempestratio = counts[4] / total;
					addedstargateproduction.Transitionunitcount = counts[5];
					Stargateproductionratios.push_back(addedstargateproduction);

				}
				else
				{
					std::cout << "Not enough parameters for stargate production" << std::endl;
				}

			}
		}
	}
	

}

sc2::UNIT_TYPEID UnitProduction::Identifyunit(const std::string & name)
{
	
	//std::cout << name << std::endl;
	if (name == "STARGATE" || name == "Stargate")
	{
		//std::cout << "Transition unit identified as Stargate" << std::endl;
		return sc2::UNIT_TYPEID::PROTOSS_STARGATE;
		
	}
	if (name == "GATEWAY" || name == "Gateway")
	{
		//std::cout << "Transition unit identified as Gateway" << std::endl;
		return sc2::UNIT_TYPEID::PROTOSS_GATEWAY;
	}
	if (name == "CYBERCORE" || name == "Cybercore" || name == "CYBERNETICSCORE" || name == "Cyberneticscore")
	{
		//std::cout << "Transition unit identified as Cybercore" << std::endl;
		return sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE;
	}
	if (name == "NEXUS" || name == "Nexus")
	{
		//std::cout << "Transition unit identified as Nexus" << std::endl;
		return sc2::UNIT_TYPEID::PROTOSS_NEXUS;
	}
	if (name == "FLEETBEACON" || name == "Fleetbeacon")
	{
		
		return sc2::UNIT_TYPEID::PROTOSS_FLEETBEACON;
	}
	if (name == "TWILIGHT" || name == "Twilight" || name == "TWILIGHTCOUNCIL" || name == "Twilightcouncil")
	{
		
		return sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL;
	}
	if (name == "DARKSHRINE" || name == "Darkshrine")
	{
		
		return sc2::UNIT_TYPEID::PROTOSS_DARKSHRINE;
	}
	if (name == "ARCHIVE" || name == "Archive" || name == "TEMPLARARCHIVE" || name == "Templararchive")
	{
		
		return sc2::UNIT_TYPEID::PROTOSS_TEMPLARARCHIVE;
	}
	if (name == "ROBO" || name == "Robo" || name == "ROBOTICSFACILITY" || name == "Roboticsfacility")
	{
		
		return sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY;
	}
	if (name == "BAY" || name == "Bay" || name == "Roboticsbay" || name == "ROBOTICSBAY")
	{
		
		return sc2::UNIT_TYPEID::PROTOSS_ROBOTICSBAY;
	}
	if (name == "FORGE" || name == "Forge")
	{
		
		return sc2::UNIT_TYPEID::PROTOSS_FORGE;
	}
	return sc2::UNIT_TYPEID::PROTOSS_PROBE;
}