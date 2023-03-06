#pragma once

#include "LevelUIElementBase.h"


class CoinBox : public LevelUIElementBase
{
public:
	CoinBox(LevelEngine* engine, int x, int y);

	void Logic(float elapsedTime) override;
	void Draw() const override;


private:
	float timeCounter;
	short amount, id;
};
