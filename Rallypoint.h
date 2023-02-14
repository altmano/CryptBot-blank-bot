#pragma once
//#include "C:\Users\Pyroman\Documents\Visual Studio 2017\SC2API_Binary_vs2017\include\sc2api\sc2_agent.h"
#include "sc2api/sc2_agent.h"
class CryptBot;

class Rallypoint
{
	CryptBot & mbot;
public:

	Rallypoint(CryptBot & bot);
	void SetRallypoint(const sc2::Unit *);
};

