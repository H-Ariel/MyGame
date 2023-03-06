#include "MutableObject.h"
#include "LevelEngine.h"


MutableObject::MutableObject(LevelEngine* const engine, int x, int y, short tileId)
	: LevelUIElementBase(engine, { x * TILE_SIZE_F, y * TILE_SIZE_F }), tileId(tileId), timeCounter(0)
{
}

void MutableObject::Logic(float elapsedTime)
{
	timeCounter += elapsedTime;

	if (timeCounter >= 0.5f)
	{
		timeCounter = 0;
		changeMutableObject(tileId);
	}
}

void MutableObject::Draw() const
{
	D2D1_RECT_F dst = GetRect();
	if (engine->InCameraFrame(dst))
	{
		engine->RemoveOffset(dst);
		AssetsManager::DrawTile(dst, tileId);
	}
}
