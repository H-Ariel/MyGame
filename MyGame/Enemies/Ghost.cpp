#include "Ghost.h"
#include "..\LevelEngine.h"


int Ghost_GetStep(float z)
{
	return (int)(fmodf(z, (TILE_SIZE / 0.5f)) / (TILE_SIZE / 0.5f) * 2);
}


Ghost::Ghost(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxX)
	: HorizontalEnemy(engine, initialPosition, maxX, EnemyTypes::ghost), initialY(initialPosition.y + TILE_SIZE), isAwake(false)
{
	speed = { TILE_SIZE * 1.5f, TILE_SIZE * 2.f };
}
void Ghost::Disqualification() { } // this enemy could not disqualify
void Ghost::Logic(float elapsedTime)
{
	const D2D1_RECT_F playerRc = engine->player.GetRect(), ghostRc = GetRect();

	if (InRange(engine->player.position.x, minX, maxX) && GetDistance(playerRc, ghostRc) <= 3.0 * TILE_SIZE)
		isAwake = true; // if the player in the ghost range (used by calculate the distance between centers)

	if (isAwake)
	{
		// the ghost can move only if it not on the player
		if (abs(engine->player.position.x - position.x) >= TILE_SIZE / 10)
		{
			bool goesUp = Ghost_GetStep(position.x);

			if (goesUp && !InRange(position.x, minX + TILE_SIZE, maxX - TILE_SIZE))
				goesUp = !goesUp;

			position.y += (goesUp ? -speed.y : speed.y) * elapsedTime;

			position.x += (forward ? speed.x : -speed.x) * elapsedTime;
		}

		if (position.x > engine->player.position.x) forward = false;
		else if (position.x < engine->player.position.x) forward = true;

		if (position.x < minX || maxX < position.x) isAwake = false; // the player escape
	}
	else
	{
		position.y = initialY;

		if (speed.y < 0)
			speed.y = -speed.y; // when the ghost starts move it first goes down to the player
	}
}
string Ghost::GetMoveType() const
{
	string moveType(GetName(type));
	if (!isAwake) moveType += "_normal";
	return moveType;
}
void Ghost::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	HorizontalEnemy::ChangeZoomRatio(oldTileSize, newTileSize);
	initialY = initialY / oldTileSize * newTileSize;
}
