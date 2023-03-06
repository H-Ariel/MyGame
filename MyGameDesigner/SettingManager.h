#pragma once

#include <string>


class SettingManager
{
public:
	static void ReadSetting();

	static std::string PlayerColor;
	static int TileSize; // width and height
	static const int DefaultTileSize;
};


#define TILE_SIZE	SettingManager::TileSize
#define TILE_SIZE_F	(float)TILE_SIZE
