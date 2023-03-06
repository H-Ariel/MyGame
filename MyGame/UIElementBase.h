#pragma once

#include "framework.h"


class UIElementBase
{
public:
	UIElementBase(D2D1_POINT_2F position = { 0,0 });

	virtual void Logic(float elapsedTime);
	virtual void Draw() const;


	D2D1_POINT_2F position;
};
