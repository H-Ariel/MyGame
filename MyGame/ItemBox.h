#pragma once

#include "LevelUIElementBase.h"


class ItemBox : public LevelUIElementBase
{
public:
	ItemBox(LevelEngine* engine, int x, int y);

	void Logic(float elapsedTime) override;
	void Draw() const override;


private:
	short id;
	bool collected;
};
