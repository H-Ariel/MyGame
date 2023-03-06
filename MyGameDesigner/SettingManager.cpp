#include "SettingManager.h"
#include "framework.h"


std::string SettingManager::PlayerColor;
const int SettingManager::DefaultTileSize = 70;
int SettingManager::TileSize = 70;


void SettingManager::ReadSetting()
{
	const json settingJson = json::FromFile("setting.json");

	PlayerColor = (std::string)settingJson["player-color"];

	const char* ValidPlayerColors[] = { "Beige", "Blue", "Green", "Pink", "Yellow" };
	if (!FIND_IN_ARRAY(ValidPlayerColors, PlayerColor))
		throw std::exception(("invalid player color: " + PlayerColor).c_str());
}
