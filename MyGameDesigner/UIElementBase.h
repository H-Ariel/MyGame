#pragma once

#include "FontData.h"
#include "WindowBase.h"


class UIElementBase
{
public:
	UIElementBase(WindowBase* window, D2D1_POINT_2F position = { 0,0 });

	virtual void Logic(float elapsedTime);
	virtual void Draw();

	virtual void ChangeZoomRatio(int oldTileSize, int newTileSize);

	virtual D2D1_RECT_F GetRect() const;
	virtual D2D1_SIZE_F GetSize() const;


	D2D1_POINT_2F position; // by default its the left-bottom corner (acoording to `GetRect`), but we can change it.

protected:
	HWND GetWnd() const;

	WindowBase* window;
};
