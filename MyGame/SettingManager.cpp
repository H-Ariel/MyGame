#include "SettingManager.h"
#include "framework.h"


std::string SettingManager::PlayerColor;
const int SettingManager::defaultTileSize = 70;
int SettingManager::TileSize = 70;
int SettingManager::numOfLevels = 0;


void SettingManager::ReadSetting()
{
	const json settingJson = json::FromFile("setting.json");

	numOfLevels = settingJson["numOfLevels"];
	PlayerColor = (std::string)settingJson["player-color"];

	const char* ValidPlayerColors[] = { "Beige", "Blue", "Green", "Pink", "Yellow" };
	if (!FIND_IN_ARRAY(ValidPlayerColors, PlayerColor))
		throw std::exception(("invalid player color: " + PlayerColor).c_str());
}

int SettingManager::GetDefaultTileSize() { return defaultTileSize; }
int SettingManager::GetNumOfLevels() { return numOfLevels; }
