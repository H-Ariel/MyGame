#pragma once

#include "LevelUIElementBase.h"
#include <list>


// Level Head-Up Display
class LevelHUD : public LevelUIElementBase
{
public:
	LevelHUD(LevelEngine* engine);

	void Draw() const override;

private:
	void DrawNumber(float x, float y, float height, int N) const;
};
