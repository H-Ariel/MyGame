#include "Snake.h"
#include "..\LevelEngine.h"


Snake::Snake(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxY, float timeUp, float timeDown, EnemyTypes type)
	: VerticalEnemy(engine, initialPosition, maxY, type), // there are 2 types of snakes
	timeUp(timeUp), timeDown(timeDown), moveTimeCounter(0)
{
	this->minY += 0.15f * TILE_SIZE;
	this->maxY += 0.15f * TILE_SIZE;
	this->position.y += 0.15f * TILE_SIZE;
}
void Snake::Logic(float elapsedTime)
{
	if (timeCounter <= 0)
	{
		position.y += speed.y * elapsedTime;

		if (position.y >= maxY)
		{
			speed.y = -TILE_SIZE_F;
			timeCounter = timeDown;
		}
		else if (position.y <= minY)
		{
			speed.y = TILE_SIZE_F;
			timeCounter = timeUp;
		}
	}
	timeCounter -= elapsedTime;
	moveTimeCounter = moveTimeCounter - (int)moveTimeCounter + elapsedTime;
}
string Snake::GetMoveType() const
{
	string moveType(GetName(type) + "_");
	if (moveTimeCounter <= 0.5f) moveType += '1';
	else moveType += '2';
	return moveType;
}
