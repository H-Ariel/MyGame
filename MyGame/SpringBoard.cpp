#include "SpringBoard.h"
#include "LevelEngine.h"
#include "TilesCodes.h"
#include "CollisionDistances.h"


SpringBoard::SpringBoard(LevelEngine* engine, float x, float y, float force)
	: LevelUIElementBase(engine, { x * TILE_SIZE, y * TILE_SIZE }), force(force), isDown(true), timeCounter(0)
{
}

void SpringBoard::Logic(float elapsedTime)
{
	const D2D1_RECT_F colRc = CollisionDistances::GetCollision(engine->player.GetRect(), GetRect());
	if (CollisionDistances::IsBottomCollision(colRc) && engine->player.IsFalling())
	{
		engine->player.Jump(Player::JumpTypes::SpringboardJump, force);
		isDown = false;
	}

	if (!isDown)
	{
		timeCounter += elapsedTime;

		if (timeCounter >= 0.2f)
		{
			timeCounter = 0;
			isDown = true;
		}
	}
}

void SpringBoard::Draw() const
{
	D2D1_RECT_F dst = GetRect();
	if (engine->InCameraFrame(dst))
	{
		engine->RemoveOffset(dst);
		AssetsManager::DrawTile(dst, isDown ? SpringboardDown : SpringboardUp);
	}
}
