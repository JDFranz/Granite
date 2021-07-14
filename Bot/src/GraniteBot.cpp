#include "GraniteBot.h"
#include "Unit_Mapping.h"
#include "EconomyManager.h"

// Called when the bot starts!
void GraniteBot::onStart()
{
	// Set our BWAPI options here
	BWAPI::Broodwar->setLocalSpeed(3);
	BWAPI::Broodwar->setFrameSkip(1);
	// Enable the flag that tells BWAPI top let users enter input while bot plays
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

	// Call MapTools OnStart
	m_mapTools.onStart();
	m_unit_mapping = make_shared<Unit_Mapping>(Unit_Mapping());

	//export_map();
}

// Called whenever the game ends and tells you if you won or not
void GraniteBot::onEnd(bool isWinner)
{
	std::cout << "We " << (isWinner ? "won!" : "lost!") << "\n";
}

// Called on each frame of the game
void GraniteBot::onFrame()
{
	try
	{
		// Update our MapTools information
		m_mapTools.onFrame();
		m_scouting.onFrame(m_unit_mapping);
		//unitvector = get_units();
		//vector<BWAPI::Unit>workers = filter_worker();
		//ecoman.onFrame(workers);

		// Draw unit health bars, which brood war unfortunately does not do
		Tools::DrawUnitHealthBars();

		// Draw some relevent information to the screen to help us debug the bot
		drawDebugInformation();
	}
	catch (const RefreshViolation& e)
	{
		//this happens when the pointer of Unit_Maping is outdated
		//simply executing the next on Frame will probably resolve everything
		//m_unit_mapping.set_task(e.unit, e.task);
		m_mapTools.onFrame();
		m_scouting.onFrame(m_unit_mapping);
		Tools::DrawUnitHealthBars();
		drawDebugInformation();
	}
}

GraniteBot::GraniteBot() :
	ecoman(EconomyManager()),
	m_scouting(Scouting())
{
}

// Draw some relevent information to the screen to help us debug the bot
void GraniteBot::drawDebugInformation()
{
	BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 10), "Hello, World!\n");
	Tools::DrawUnitCommands();
	Tools::DrawUnitBoundingBoxes();
}

// Called whenever a unit is destroyed, with a pointer to the unit
void GraniteBot::onUnitDestroy(BWAPI::Unit unit)
{
}

// Called whenever a unit is morphed, with a pointer to the unit
// Zerg units morph when they turn into other units
void GraniteBot::onUnitMorph(BWAPI::Unit unit)
{
}

// Called whenever a text is sent to the game by a user
void GraniteBot::onSendText(std::string text)
{
	if (text == "/map")
	{
		//m_mapTools.toggleDraw();
	}
}

// Called whenever a unit is created, with a pointer to the destroyed unit
// Units are created in buildings like barracks before they are visible,
// so this will trigger when you issue the build command for most units
void GraniteBot::onUnitCreate(BWAPI::Unit unit)
{
}

// Called whenever a unit finished construction, with a pointer to the unit
void GraniteBot::onUnitComplete(BWAPI::Unit unit)
{
}

// Called whenever a unit appears, with a pointer to the destroyed unit
// This is usually triggered when units appear from fog of war and become visible
void GraniteBot::onUnitShow(BWAPI::Unit unit)
{
}

// Called whenever a unit gets hidden, with a pointer to the destroyed unit
// This is usually triggered when units enter the fog of war and are no longer visible
void GraniteBot::onUnitHide(BWAPI::Unit unit)
{
}

// Called whenever a unit switches player control
// This usually happens when a dark archon takes control of a unit
void GraniteBot::onUnitRenegade(BWAPI::Unit unit)
{
}

void GraniteBot::export_map()
{
	std::cout << "\n\n";
	std::cout << "exporting map" << std::endl;
	m_mapTools.printAndSave("export.txt");
	BWAPI::Broodwar->leaveGame();
	/* A function to export the current map.
	 * calls to the maptool class to execute print and save on its member Stardraftmap
	 * maps are saved in ...\Granite\GraniteBot\bin
	 */
}