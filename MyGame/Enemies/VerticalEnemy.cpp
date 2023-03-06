#include "VerticalEnemy.h"
#include "..\LevelEngine.h"


VerticalEnemy::VerticalEnemy(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxY, EnemyTypes type)
	: EnemyBase(engine, initialPosition, type), timeCounter(0)
{
	if (position.y > maxY)
		swap(position.y, maxY);
	this->minY = position.y;
	this->maxY = maxY;

	// put the enemy in the center of the tile
	string name = GetName(type);
	if (type == EnemyTypes::snakeGreen || type == EnemyTypes::snakeRed)
		name += "_1";
	position.x += (TILE_SIZE - AssetsManager::GetEnemySize(name).width) / 2;
}
void VerticalEnemy::Disqualification() { } // those enemies could not disqualify
void VerticalEnemy::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	EnemyBase::ChangeZoomRatio(oldTileSize, newTileSize);
	minY = minY / oldTileSize * newTileSize;
	maxY = maxY / oldTileSize * newTileSize;
}
