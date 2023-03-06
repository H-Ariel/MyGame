#pragma once

#include "UIElementBase.h"


class LevelEngine;


class LevelUIElementBase : public UIElementBase
{
public:
	LevelUIElementBase(LevelEngine* const engine, D2D1_POINT_2F position = { 0,0 });

	virtual void ChangeZoomRatio(int oldTileSize, int newTileSize);

	virtual D2D1_SIZE_F GetSize() const;
	virtual D2D1_RECT_F GetRect() const;


protected:
	LevelEngine* const engine;
};
