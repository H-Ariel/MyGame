#pragma once

#include "LevelUIElementBase.h"
#include "TilesCodes.h"


class LevelMap : public LevelUIElementBase
{
public:
	LevelMap(LevelEngine* engine);

	void Draw() const override;

	// This function returns collision details between the player and the solid-tiles.
	// In addition, we check here for collisions with obstacles/ladders/liquids/door/coins/keys/etc.
	D2D1_RECT_F PlayerCollides();

private:
	struct TileCell
	{
		TileCell(short tileId);

		TileType type;
		short foreground_tile_value; // this is only foreground tile for view, it no used in collision or collectable
		short tile_value;
	};


	void AllocNew(D2D1_SIZE_U newSize);
	void Clear(); // release the level matrix
	

	vector<vector<TileCell>> tilesMap;
	D2D1_SIZE_U mapSize;


	friend class LevelEngine;
};
