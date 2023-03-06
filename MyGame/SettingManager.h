#pragma once

#include <string>


class SettingManager
{
public:
	static void ReadSetting();

	static int GetDefaultTileSize();
	static int GetNumOfLevels();

	static std::string PlayerColor;
	static int TileSize; // width and height

private:
	static const int defaultTileSize;
	static int numOfLevels;
};


#define TILE_SIZE	SettingManager::TileSize
#define TILE_SIZE_F	(float)TILE_SIZE
