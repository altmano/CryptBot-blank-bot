#pragma once
#include <vector>
#include <math.h>
#include <iostream>
//#include "C:\Users\Pyroman\Documents\Visual Studio 2017\SC2API_Binary_vs2017\include\sc2api\sc2_agent.h"
#include "sc2api/sc2_agent.h"

class CryptBot;

struct singlebase 
{
	
	std::vector< const sc2::Unit*>  gasses;
	std::vector< const sc2::Unit*>  minerals;
	double x;
	double y;
};

class Bases
{
	CryptBot & mbot;
public:
	Bases(CryptBot & bot);
	std::vector<const sc2::Unit*>  mineralfields;
	std::vector<const sc2::Unit*>  vespinegeysers;
	std::vector<singlebase> allbases;
	void updatefields();
	void sorttobases();
	//sc2::Point2D getbaselocation();
	int getnextexpansion();
};

