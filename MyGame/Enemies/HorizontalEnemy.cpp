#include "HorizontalEnemy.h"
#include "..\LevelEngine.h"


HorizontalEnemy::HorizontalEnemy(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxX, EnemyTypes type)
	: EnemyBase(engine, initialPosition, type)
{
	if (initialPosition.x > maxX)
		swap(initialPosition.x, maxX);
	this->minX = initialPosition.x;
	this->maxX = maxX;

	speed.x = TILE_SIZE / 2.0f;
}
void HorizontalEnemy::Logic(float elapsedTime)
{
	position.x += (forward ? speed.x : -speed.x) * elapsedTime;
	if (position.x >= maxX) forward = false;
	else if (position.x <= minX) forward = true;
}
void HorizontalEnemy::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	EnemyBase::ChangeZoomRatio(oldTileSize, newTileSize);
	minX = minX / oldTileSize * newTileSize;
	maxX = maxX / oldTileSize * newTileSize;
}
string HorizontalEnemy::GetMoveType() const
{
	string moveType(GetName(type));
	const int stepX = GetStep(position.x);

	switch (type)
	{
	case EnemyTypes::ladyBug:
	case EnemyTypes::mouse:
	case EnemyTypes::slimePink:
	case EnemyTypes::slimeGreen:
	case EnemyTypes::slimeBlue:
	case EnemyTypes::wormGreen:
	case EnemyTypes::wormPink:
		if (stepX == 2) moveType += "_walk";
		break;

	case EnemyTypes::spider:
		moveType += "_";
		moveType += (char)(stepX + '0');
		break;

	case EnemyTypes::frog:
		if (stepX == 2) moveType += "_leap";
		break;

	default:
		break;
	}

	return moveType;
}

int HorizontalEnemy::GetStep(float z)
{
	return (int)(fmodf(z, (TILE_SIZE / 2.0f)) / (TILE_SIZE / 2.0f) * 2 + 1);
}
