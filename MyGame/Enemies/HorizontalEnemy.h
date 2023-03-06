#pragma once

#include "EnemyBase.h"


class HorizontalEnemy : public EnemyBase // mouse, slime, etc.
{
public:
	HorizontalEnemy(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxX, EnemyTypes type);

	virtual void Logic(float elapsedTime) override;
	virtual void ChangeZoomRatio(int oldTileSize, int newTileSize) override;
	virtual string GetMoveType() const;

protected:
	static int GetStep(float z); // render `z` to 1 or 2


	float minX, maxX;
};
