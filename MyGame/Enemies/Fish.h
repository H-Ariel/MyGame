#pragma once

#include "VerticalEnemy.h"


class Fish : public VerticalEnemy
{
public:
	Fish(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxY, EnemyTypes type);
	void Logic(float elapsedTime) override;
	void Draw() const override;
	string GetMoveType() const override;
};
