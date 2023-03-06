#pragma once

#include "EnemyBase.h"


class VerticalEnemy : public EnemyBase // fish, snake, etc.
{
public:
	VerticalEnemy(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxY, EnemyTypes type);

	virtual void Logic(float elapsedTime) override = 0;

	void Disqualification() override;
	void ChangeZoomRatio(int oldTileSize, int newTileSize) override;

protected:
	float minY, maxY;
	float timeCounter; // used to make delay before the enemy goes up/down again
};
