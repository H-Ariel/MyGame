#pragma once

#include "LevelUIElementBase.h"


class SpringBoard : public LevelUIElementBase
{
public:
	SpringBoard(LevelEngine* engine, float x, float y, float force);

	void Logic(float elapsedTime) override;
	void Draw() const override;


private:
	float force, timeCounter;
	bool isDown;
};
