#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"
#include "sc2utils/sc2_manage_process.h"
#include "sc2utils/sc2_arg_parser.h"

#include "Cryptbot.h"
#include <iostream>

#ifdef DEBUG
int main(int argc, char* argv[])
{
	class Human : public sc2::Agent {
	public:
		void OnGameStart() final {
			Debug()->DebugTextOut("Human");
			Debug()->SendDebug();

		}

		void OnStep()
		{
			Control()->GetObservation();
		}

	};
	Human human;
	CryptBot bot;
	sc2::Coordinator coordinator;
	if (!coordinator.LoadSettings(argc, argv))
	{
		std::cout << "Unable to find or parse settings." << std::endl;
		return 1;
	}
	coordinator.SetStepSize(1);
	coordinator.SetRealtime(false);
	coordinator.SetMultithreaded(true);
	coordinator.SetParticipants({
		CreateParticipant(sc2::Race::Protoss, &bot),
		//sc2::PlayerSetup(sc2::PlayerType::Observer,Util::GetRaceFromString(enemyRaceString)),
		CreateComputer(sc2::Race::Terran, sc2::Difficulty::VeryHard)

		
	});
	// Start the game.
	coordinator.LaunchStarcraft();
	coordinator.StartGame("C:/Program Files (x86)/StarCraft II/Maps/InterloperLE.SC2Map");
	//coordinator.StartGame("Interloper LE");


	// Step forward the game simulation.
	while (coordinator.Update())
	{
	}
}
#else
#include "LadderInterface.h"
int main(int argc, char* argv[])
{
	RunBot(argc, argv, new CryptBot(), sc2::Race::Protoss);

	return 0;
}
#endif
