#pragma once

#include "EnemyBase.h"


class FlyingEnemy : public EnemyBase // fly, bee, spinner, etc.
{
public:
	FlyingEnemy(LevelEngine* engine, D2D1_POINT_2F initialPosition, D2D1_POINT_2F finalPosition, EnemyTypes type);

	void Logic(float elapsedTime) override;
	void Disqualification() override;
	void ChangeZoomRatio(int oldTileSize, int newTileSize) override;
	string GetMoveType() const override;


private:
	D2D1_POINT_2F initPos, finPos;
	double alpha;
	float moveTimeCounter;
	const bool mul_minus;
	const bool vertical_movement;
};
