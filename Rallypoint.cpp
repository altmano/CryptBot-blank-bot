#include "Rallypoint.h"
#include "Cryptbot.h"



Rallypoint::Rallypoint(CryptBot & bot)

	:mbot (bot)
{
}

void Rallypoint::SetRallypoint(const sc2::Unit *unit)
{
	switch (unit->unit_type.ToType())
	{
	case sc2::UNIT_TYPEID::PROTOSS_NEXUS:
	{
		
	}
	case sc2::UNIT_TYPEID::PROTOSS_GATEWAY:
	{
		float rx = sc2::GetRandomScalar();
		float ry = sc2::GetRandomScalar();
		mbot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::RALLY_BUILDING, sc2::Point2D(unit->pos.x + rx * 15.0f, unit->pos.y + ry * 15.0f));
	}
	default:
		break;
	}
}


