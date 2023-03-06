#pragma once

#include "VerticalEnemy.h"


class Snake : public VerticalEnemy
{
public:
	Snake(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxY, float timeUp, float timeDown, EnemyTypes type);
	void Logic(float elapsedTime) override;
	string GetMoveType() const override;

private:
	float moveTimeCounter;
	float timeUp, timeDown;
};
