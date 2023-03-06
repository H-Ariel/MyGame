#pragma once

#include "LevelUIElementBase.h"


class MovingTile : public LevelUIElementBase
{
public:
	MovingTile(LevelEngine* engine, D2D1_POINT_2F initialPosition, D2D1_POINT_2F finalPosition, short tileId);

	void Logic(float elapsedTime) override;
	void Draw() const override;
	void ChangeZoomRatio(int oldTileSize, int newTileSize) override;


private:
	D2D1_POINT_2F initPos, finPos;
	double alpha;
	short tileId;
	const bool mul_minus;
	const bool vertical_movement;
	bool forward; // used in case of vertical movement
};
