#pragma once
#include <vector>
class CryptBot;

class Workers
{
	CryptBot & mbot;
public:
	Workers(CryptBot & bot);
	void GiveJobs();
};

