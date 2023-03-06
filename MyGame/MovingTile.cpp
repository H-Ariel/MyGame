#include "MovingTile.h"
#include "LevelEngine.h"
#include "TilesCodes.h"
#include "CollisionDistances.h"


#define MOVING_TILE_SPEED (TILE_SIZE * 1.5f)


MovingTile::MovingTile(LevelEngine* engine, D2D1_POINT_2F initialPosition, D2D1_POINT_2F finalPosition, short tileId)
	: LevelUIElementBase(engine), tileId(tileId), alpha(0), mul_minus(finalPosition.x < initialPosition.x),
	vertical_movement(finalPosition.x == initialPosition.x), forward(false)
{
	position = initPos = { initialPosition.x * TILE_SIZE, initialPosition.y * TILE_SIZE };
	finPos = { finalPosition.x * TILE_SIZE, finalPosition.y * TILE_SIZE };
	alpha = atan((initPos.y - finPos.y) / (initPos.x - finPos.x));
}

void MovingTile::Logic(float elapsedTime)
{
	static D2D1_POINT_2F speed = {};


	if (vertical_movement)
	{
		/*
		in case of `initPos.x == finPos.x` there is a little problem: the result of
		`(initialPosition.y - finalPosition.y) / (initialPosition.x - finalPosition.x)`
		is unknown, so the moving-tiles stuck and don't move. So in case of vertical
		movement we don't need to stay with unknown behavior of moving-tiles and we
		handle this specific case
		*/

		float maxY = max(initPos.y, finPos.y), minY = min(initPos.y, finPos.y);

		speed.x = 0;
		speed.y = MOVING_TILE_SPEED;
		if (forward) speed.y *= -1;
		
		position.y += speed.y * elapsedTime;
		
		if (position.y >= maxY) forward = true;
		else if (position.y <= minY) forward = false;
	}
	else
	{
		speed.x = (float)cos(alpha) * MOVING_TILE_SPEED;
		speed.y = (float)sin(alpha) * MOVING_TILE_SPEED;

		if (mul_minus)
		{
			speed.x *= -1;
			speed.y *= -1;
		}

		position.x += speed.x * elapsedTime;
		position.y += speed.y * elapsedTime;

		if (!InRange(position.x, initPos.x, finPos.x) || !InRange(position.y, initPos.y, finPos.y))
		{
			// change direction and careful from overload
			alpha += M_PI;
			static double PI_2 = 2 * M_PI;
			if (alpha > PI_2) alpha -= PI_2;
			else if (alpha < -PI_2) alpha += PI_2;
		}
	}


	const D2D1_RECT_F playerRc = engine->player.GetRect();
	const D2D1_RECT_F tileRc = GetRect();
	const D2D1_RECT_F colRc = CollisionDistances::GetCollision(playerRc, tileRc);

	if (engine->player.movingTile == this)
	{
		engine->player.position.x += speed.x * elapsedTime;
		engine->player.position.y += speed.y * elapsedTime;

		// if no collision - disable the moving tile mode for player
		if (playerRc.right < tileRc.left || tileRc.right < playerRc.left)
		{
			engine->player.movingTile = nullptr;
		}
	}
	else
	{
		D2D1_RECT_F smallest(colRc);
		CollisionDistances::keepSmallest(smallest);
		if (smallest.top > 0)
		{
			engine->player.BounceTop();
		}
		else if (smallest.bottom > 0 && (colRc.right > 0 || colRc.left > 0) && engine->player.GetSpeedY() >= 0)
		{
			// TODO: use `player.IsFalling` instead of `player.GetSpeedY`?
			// if the player fall or go to this tile - catch him
			engine->player.StopFalling(colRc.bottom);
			engine->player.movingTile = this;
		}
	}
}

void MovingTile::Draw() const
{
	D2D1_RECT_F dst = GetRect();
	if (engine->InCameraFrame(dst))
	{
		engine->RemoveOffset(dst);
		AssetsManager::DrawTile(dst, tileId);
	}
}

void MovingTile::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	LevelUIElementBase::ChangeZoomRatio(oldTileSize, newTileSize);

	ChangePointZoomRatio(initPos, oldTileSize, newTileSize);
	ChangePointZoomRatio(finPos, oldTileSize, newTileSize);
}
