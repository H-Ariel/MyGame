#include "FlyingEnemy.h"
#include "..\AssetsManager.h"


#define FLYING_ENEMY_TOTAL_SPEED (TILE_SIZE / 2.0f)


FlyingEnemy::FlyingEnemy(LevelEngine* engine, D2D1_POINT_2F initialPosition, D2D1_POINT_2F finalPosition, EnemyTypes type)
	: EnemyBase(engine, initialPosition, type), initPos(initialPosition), finPos(finalPosition),
	alpha(0), mul_minus(finalPosition.x < initialPosition.x), moveTimeCounter(0),
	vertical_movement(finalPosition.x == initialPosition.x)
{
	initPos.y += TILE_SIZE;
	finPos.y += TILE_SIZE;

	alpha = atan((initPos.y - finPos.y) / (initPos.x - finPos.x));

	if (type == EnemyTypes::slimeBlock)
	{
		// center the enemy:
		const D2D1_SIZE_F enemySize = AssetsManager::GetEnemySize(GetName(type));
		position.x += (TILE_SIZE - enemySize.width) / 2;
		position.y += (TILE_SIZE - enemySize.height) / 2;
	}
}

void FlyingEnemy::Logic(float elapsedTime)
{
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
		speed.y = FLYING_ENEMY_TOTAL_SPEED;
		if (forward) speed.y *= -1;

		position.y += speed.y * elapsedTime;

		if (position.y >= maxY) forward = true;
		else if (position.y <= minY) forward = false;
	}
	else
	{
		speed.x = (float)cos(alpha) * FLYING_ENEMY_TOTAL_SPEED;
		speed.y = (float)sin(alpha) * FLYING_ENEMY_TOTAL_SPEED;

		if (mul_minus)
		{
			speed.x *= -1;
			speed.y *= -1;
		}

		position.x += speed.x * elapsedTime;
		position.y += speed.y * elapsedTime;

		if (speed.x < 0) forward = false;
		else if (speed.x > 0) forward = true;

		if (!InRange(position.x, initPos.x, finPos.x) || !InRange(position.y, initPos.y, finPos.y))
		{
			// change direction and careful from overload
			alpha += M_PI;
			static double PI_2 = 2 * M_PI;
			if (alpha > PI_2) alpha -= PI_2;
			else if (alpha < -PI_2) alpha += PI_2;
		}
	}

	if (type == EnemyTypes::spinner)
	{
		moveTimeCounter += elapsedTime;
		if (moveTimeCounter >= FLT_MAX / 2)
			moveTimeCounter = 0;
	}
	else
	{
		moveTimeCounter = moveTimeCounter - (int)moveTimeCounter + elapsedTime;
	}
}

void FlyingEnemy::Disqualification() { }

string FlyingEnemy::GetMoveType() const
{
	string moveType(GetName(type));

	switch (type)
	{
	case EnemyTypes::bat:
	case EnemyTypes::bee:
	case EnemyTypes::fly:
		if (moveTimeCounter <= 0.5f)
			moveType += "_fly";
		break;

	case EnemyTypes::spinner:
		if (fmod(moveTimeCounter, 0.3) >= 0.15)
			moveType += "_spin";
		break;

	default:
		break;
	}

	return moveType;
}

void FlyingEnemy::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	EnemyBase::ChangeZoomRatio(oldTileSize, newTileSize);
	ChangePointZoomRatio(initPos, oldTileSize, newTileSize);
	ChangePointZoomRatio(finPos, oldTileSize, newTileSize);
}
