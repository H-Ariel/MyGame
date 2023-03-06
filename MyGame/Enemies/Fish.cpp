#include "Fish.h"
#include "..\LevelEngine.h"


Fish::Fish(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxY, EnemyTypes type)
	: VerticalEnemy(engine, initialPosition, maxY, type) // there are 3 types of fishes (i used only one)
{
	forward = true;
}
void Fish::Logic(float elapsedTime)
{
	if (timeCounter <= 0)
	{
		//speed.y += 3 * TILE_SIZE * elapsedTime; // gravity (todo)
		position.y += speed.y * elapsedTime;

		if (position.y >= maxY)
		{
			speed.y = -5.0f * TILE_SIZE;
			forward = false;
			timeCounter = 1;
		}
		else if (position.y <= minY)
		{
			speed.y = 5.0f * TILE_SIZE;
			forward = true;
			//timeCounter = 0.1f; // (todo)
		}
	}
	timeCounter -= elapsedTime;
}
void Fish::Draw() const // I created a new function for drawing because the fish needs to rotate
{
	D2D1_RECT_F dst = GetRect();

	if (engine->InCameraFrame(dst))
	{
		engine->RemoveOffset(dst);
		AssetsManager::DrawEnemy(dst, GetMoveType(), false, forward ? 180.0f : 0);
	}
}
string Fish::GetMoveType() const
{
	return GetName(type);
}
