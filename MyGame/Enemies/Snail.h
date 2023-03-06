#pragma once

#include "HorizontalEnemy.h"


class Snail : public HorizontalEnemy
{
public:
	Snail(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxX);
	bool CanDisqualify() const override;
	void Logic(float elapsedTime) override;
	string GetMoveType() const override;
};
