// global variables and constants

#pragma once

#include <string>
#include <vector>


enum TilesCodes
{
	Empty = -1,

	SpringboardDown = 64,
	SpringboardUp = 65
};


enum class Type // type of choice
{
	NONE,
	TILE,
	ENEMY
};


class GlobalVariables
{
public:
	static int SelectedId;
	static Type SelectedType;
	static std::string EnemyName;
};

static const std::initializer_list<const char* const> ENEMIES_LIST = {
	"ladyBug", "mouse", "slimePink", "slimeGreen", "slimeBlue",
	"wormGreen", "wormPink", "bat", "bee", "fly", "spider",
	"frog", "snail", "ghost", "fishBlue", "fishGreen", "fishPink",
	"snakeGreen", "snakeRed", "barnacle", "spinner",
	"grassBlock", "slimeBlock"
};
