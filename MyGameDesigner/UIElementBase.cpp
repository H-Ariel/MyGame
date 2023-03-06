#include "UIElementBase.h"


UIElementBase::UIElementBase(WindowBase* window, D2D1_POINT_2F position)
	: window(window), position(position)
{
}

void UIElementBase::Logic(float elapsedTime) { }
void UIElementBase::Draw() { }

void UIElementBase::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	// we convert the position to be relative to the tile size, and then return to the new tile size.
	position.x = position.x / oldTileSize * newTileSize;
	position.y = position.y / oldTileSize * newTileSize;
}

D2D1_RECT_F UIElementBase::GetRect() const
{
	D2D1_SIZE_F size = GetSize();

	D2D1_RECT_F rect = D2D1::RectF(
		position.x,
		position.y - size.height,
		position.x + size.width,
		position.y
	);

	return rect;
}
D2D1_SIZE_F UIElementBase::GetSize() const
{
	return D2D1::SizeF();
}

HWND UIElementBase::GetWnd() const
{
	return window->hwnd;
}
