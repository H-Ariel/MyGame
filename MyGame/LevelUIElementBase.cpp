#include "LevelUIElementBase.h"


LevelUIElementBase::LevelUIElementBase(LevelEngine* const engine, D2D1_POINT_2F position)
	: UIElementBase(position), engine(engine)
{
}

void LevelUIElementBase::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	ChangePointZoomRatio(position, oldTileSize, newTileSize);
}

D2D1_RECT_F LevelUIElementBase::GetRect() const
{
	D2D1_SIZE_F size = GetSize();
	return D2D1::RectF(
		position.x,
		position.y,
		position.x + size.width,
		position.y + size.height
	);
}

D2D1_SIZE_F LevelUIElementBase::GetSize() const
{
	return D2D1::SizeF(TILE_SIZE_F, TILE_SIZE_F);
}
