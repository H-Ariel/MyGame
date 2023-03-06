#pragma once

#include "LevelUIElementBase.h"


class MutableObject : public LevelUIElementBase // flag, torch, etc.
{
public:
	MutableObject(LevelEngine* const engine, int x, int y, short tileId);

	void Logic(float elapsedTime) override;
	void Draw() const override;


private:
	float timeCounter;
	short tileId;
};
