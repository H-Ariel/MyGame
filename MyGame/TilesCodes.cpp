#include "TilesCodes.h"
#include "framework.h"


// Tiles IDs

const initializer_list<short> SOLIDS = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 37, 38, 39, 40, 70, 71, 72, 73, 74, 75,
	76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 97, 98,
	99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
	116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
	133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 150, 151,
	152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
	169, 170, 171, 172, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
	188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
	205, 206, 207, 208, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
	224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
	241, 244, 245, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314,
	315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331,
	332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 355, 356, 413,
	414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430,
	431, 432, 433, 434, 435, 436, 437, 440, 441, 859, 860, 861, 862, 865, 866, 867, 868,
	869, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882, 883, 890, 891, 892, 893,
	894, 895, 896, 897, 898, 899, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939,
	940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956,
	957, 958, 959, 960
};
const initializer_list<short> COLLECTABLES = {
	CoinBronze, CoinGold, CoinSilver, Star,
	KeyBlue, KeyGreen, KeyRed, KeyYellow,
	GemBlue, GemGreen, GemRed, GemYellow,
	ShieldBronze, ShieldGold, ShieldSilver, 
	SwordBronze, SwordGold, SwordSilver, Heart };
const initializer_list<short> DOOR = { 709, 710, 711, 712 };
const initializer_list<short> LIQUIDS = { 47, 49, 357, 358};
const initializer_list<short> LIQUIDS_TOP = { 48, 50, 359 };
const initializer_list<short> LADDER = { 41, 42, 58, 794, 795, 796, 797 }; // ladders and ropes
const initializer_list<short> OBSTACLES = { 63, 382, 383, 384, 385, 386, 387, 388, 798, 799 };
const initializer_list<short> LOCKS = { LockBlue, LockGreen, LockRed, LockYellow }; // this values are also in SOLIDS array
const initializer_list<short> COIN_BOX = { CoinBox_enabled, CoinBox_disabled };
const initializer_list<short> ITEM_BOX = { ItemBox_enabled, ItemBox_disabled };
const initializer_list<short> FLAGS = {
	585, 586, 587, 588, 589, 590, 592, 593, 594, 595, 596, 597, 599,
	600, 601, 602, 603, 604, 606, 607, 608, 609, 610, 611, 624, 625
};


TileType GetTileType(short tileId)
{
	if (IsSolid			(tileId)) return TileType::Solid;
	if (IsCollectable	(tileId)) return TileType::Collectable;
	if (IsDoor			(tileId)) return TileType::Door;
	if (IsLadder		(tileId)) return TileType::Ladder;
	if (IsObstacles		(tileId)) return TileType::Obstacle;
	return TileType::None;
}

short KeyOf(short lock)
{
	switch (lock)
	{
	case LockBlue:	return KeyBlue;
	case LockGreen:	return KeyGreen;
	case LockRed:	return KeyRed;
	case LockYellow:return KeyYellow;
	default: break;
	}
	return Empty;
}

bool IsSolid		(short tileId) { return FIND_IN_ARRAY(SOLIDS, tileId); }
bool IsCollectable	(short tileId) { return FIND_IN_ARRAY(COLLECTABLES, tileId); }
bool IsDoor			(short tileId) { return FIND_IN_ARRAY(DOOR, tileId); }
bool IsLiquid		(short tileId) { return FIND_IN_ARRAY(LIQUIDS, tileId) || IsLiquidTop(tileId); }
bool IsLiquidTop    (short tileId) { return FIND_IN_ARRAY(LIQUIDS_TOP, tileId); }
bool IsLadder		(short tileId) { return FIND_IN_ARRAY(LADDER, tileId); }
bool IsObstacles	(short tileId) { return FIND_IN_ARRAY(OBSTACLES, tileId); }
bool IsLock			(short tileId) { return FIND_IN_ARRAY(LOCKS, tileId); }
bool IsCoinBox		(short tileId) { return FIND_IN_ARRAY(COIN_BOX, tileId); }
bool IsItemBox		(short tileId) { return FIND_IN_ARRAY(ITEM_BOX, tileId); }

bool isMutableObject(short tileId) { return FIND_IN_ARRAY(FLAGS, tileId); }

void changeMutableObject(short& flagId)
{
	switch (flagId)
	{
		// red flags
	case 585: flagId = 586; break;
	case 586: flagId = 585; break;
	case 592: flagId = 593; break;
	case 593: flagId = 592; break;
	case 587: flagId = 589; break;
	case 589: flagId = 587; break;
	case 595: flagId = 597; break;
	case 597: flagId = 595; break;
	case 588: flagId = 590; break;
	case 590: flagId = 588; break;
	case 594: flagId = 596; break;
	case 596: flagId = 594; break;

		// green flags
	case 599: flagId = 600; break;
	case 600: flagId = 599; break;
	case 601: flagId = 603; break;
	case 603: flagId = 601; break;
	case 602: flagId = 604; break;
	case 604: flagId = 602; break;
	case 606: flagId = 607; break;
	case 607: flagId = 606; break;
	case 608: flagId = 610; break;
	case 610: flagId = 608; break;
	case 609: flagId = 611; break;
	case 611: flagId = 609; break;

		// torches
	case 624: flagId = 625; break;
	case 625: flagId = 624; break;

	default: break;
	}
}
