#pragma once

#include "sc2api/sc2_agent.h"
#include "StrategyAdaptation.h"

class CryptBot;

class Scouting
{

	CryptBot & mbot;
public:
	StrategyAdaptation strategyadaptation;
	Scouting(CryptBot &);
	void Enemystructurescheck();
	void Enemyarmycheck();
	bool bioproduction = false;
	bool mechproduction = false;
	bool airproduction = false;
	bool aggroproduction = false;
	void SendProbeScout();
	void CheckProbeScout();
	int firstprobescouttiming = 17;
	int secondprobescouttiming = 32;
	bool firstprobescoutsent = false;
	bool secondprobescoutsent = false;
};

