#pragma once

// here we name common tiles (so there are no "magic numbers" in the code)
enum TilesCodes
{
	Empty = -1,

	CoinBox_enabled = 3,
	CoinBox_disabled,

	ItemBox_enabled = 7,
	ItemBox_disabled,

	CoinBronze = 26,
	CoinGold,
	CoinSilver,

	GemBlue,
	GemGreen,
	GemRed,
	GemYellow,

	KeyBlue,
	KeyGreen,
	KeyRed,
	KeyYellow,

	LockBlue,
	LockGreen,
	LockRed,
	LockYellow,

	FireBall = 43,

	SpringboardDown = 64,
	SpringboardUp,

	Star,

	ShieldBronze = 981,
	ShieldGold,
	ShieldSilver,
	SwordBronze,
	SwordGold,
	SwordSilver,

	Heart = 990
};

enum class TileType
{
	None,

	Solid,
	Ladder,
	Door,
	Collectable,
	Obstacle
};

TileType GetTileType(short tileId);


short KeyOf(short lock);

bool IsSolid(short tileId);
bool IsCollectable(short tileId);
bool IsDoor(short tileId);
bool IsLiquid(short tileId);
bool IsLiquidTop(short tileId);
bool IsLadder(short tileId);
bool IsObstacles(short tileId);
bool IsLock(short tileId);
bool IsCoinBox(short tileId);
bool IsItemBox(short tileId);

bool isMutableObject(short tileId);
void changeMutableObject(short& flagId);
