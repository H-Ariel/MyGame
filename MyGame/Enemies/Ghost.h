#pragma once

#include "HorizontalEnemy.h"


class Ghost : public HorizontalEnemy
{
public:
	Ghost(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxX);
	void Disqualification() override;
	void Logic(float elapsedTime) override;
	string GetMoveType() const override;
	void ChangeZoomRatio(int oldTileSize, int newTileSize) override;

private:
	float initialY;
	bool isAwake;
};