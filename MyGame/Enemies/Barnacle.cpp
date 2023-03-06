#include "Barnacle.h"
#include "..\LevelEngine.h"
#include "..\CollisionDistances.h"
#include "..\TilesCodes.h"


#define MAX_FIREBALL_RADIUS			(TILE_SIZE * 8.0)
#define FIREBALL_TOTAL_SPEED		(TILE_SIZE * 2)



Barnacle::Barnacle(LevelEngine* engine, D2D1_POINT_2F position, float throwCycleTime)
	: EnemyBase(engine, position, EnemyTypes::barnacle), throwCycleTime(throwCycleTime), timeCounter(0)
{
	// put the enemy in the center of the tile
	this->position.x += (TILE_SIZE - AssetsManager::GetEnemySize(GetName(type)).width) / 2;
}
Barnacle::~Barnacle()
{
	for (auto& f : fireBalls)
		delete f;
}
void Barnacle::Logic(float elapsedTime)
{
	timeCounter += elapsedTime;

	const D2D1_RECT_F playerRc = engine->player.GetRect();

	if (timeCounter >= throwCycleTime)
	{
		// every `throwCycleTime` seconds throw fire-ball on player

		const double R = GetDistance(GetRect(), playerRc);

		if (R <= MAX_FIREBALL_RADIUS)
		{
			// throw fire ball
			fireBalls.push_back(DBG_NEW FireBall(engine, position, engine->player.position));
			timeCounter = 0;
		}
	}


	for (int i = 0; i < fireBalls.size(); i++)
	{
		D2D1_RECT_F coll = CollisionDistances::GetCollision(playerRc, fireBalls[i]->GetRect());
		if (!CollisionDistances::IsEmpty(coll))
		{
			engine->player.Disqualification(coll);
			fireBalls[i]->ArrivedToPlayer();
		}

		if (fireBalls[i]->shouldRemoved())
		{
			delete fireBalls[i];
			fireBalls.erase(fireBalls.begin() + i);
			i -= 1; // do not skip fireballs after delete one of them
		}
	}

	for (FireBall* f : fireBalls)
		f->Logic(elapsedTime);
}
void Barnacle::Draw() const
{
	EnemyBase::Draw();
	for (FireBall* f : fireBalls)
		f->Draw();
}
void Barnacle::Disqualification() { } // this enemy could not disqualify
string Barnacle::GetMoveType() const
{
	string moveType(GetName(type));
	if (timeCounter >= throwCycleTime - 0.5f)
		moveType += "_bite";
	return moveType;
}

void Barnacle::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	EnemyBase::ChangeZoomRatio(oldTileSize, newTileSize);
	for (FireBall* f : fireBalls)
	{
		f->ChangeZoomRatio(oldTileSize, newTileSize); // TODO: use `engine->AddUIElement` ?
	}
}

Barnacle::FireBall::FireBall(LevelEngine* engine, D2D1_POINT_2F barnaclePosition, D2D1_POINT_2F playerPosition)
	: LevelUIElementBase(engine), remove(false), rotAngle(0)
{
	const D2D1_SIZE_F size = GetSize();

	// put the fire-ball in the head of the barnacle
	initialPosition = D2D1::Point2F(
		barnaclePosition.x - fmodf(barnaclePosition.x, TILE_SIZE_F) + (TILE_SIZE - size.width) / 2,
		barnaclePosition.y - fmodf(barnaclePosition.y, TILE_SIZE_F) + (TILE_SIZE - size.height) / 2 - TILE_SIZE * 0.4f
	);

	// set the player's center as target
	const D2D1_POINT_2F finalPosition = D2D1::Point2F(
		playerPosition.x + (TILE_SIZE - size.width) / 2,
		playerPosition.y + (TILE_SIZE - size.height) / 2
	);

	alpha = atan((initialPosition.y - finalPosition.y) / (initialPosition.x - finalPosition.x));
	mul_minus = (finalPosition.x < initialPosition.x);

	position = initialPosition;

	speed.x = cos(alpha) * FIREBALL_TOTAL_SPEED;
	speed.y = sin(alpha) * FIREBALL_TOTAL_SPEED;

	if (mul_minus)
	{
		speed.x *= -1;
		speed.y *= -1;
	}
}
void Barnacle::FireBall::Logic(float elapsedTime)
{
	position.x += speed.x * elapsedTime;
	position.y += speed.y * elapsedTime;
	rotAngle = fmodf(rotAngle + elapsedTime * 360, 360); // causes the fireball to rotate as it moves
}
void Barnacle::FireBall::Draw() const
{
	D2D1_RECT_F dst = GetRect();
	if (engine->InCameraFrame(dst))
	{
		engine->RemoveOffset(dst);
		AssetsManager::DrawTile(dst, TilesCodes::FireBall, rotAngle);
	}
}
bool Barnacle::FireBall::shouldRemoved() const
{
	const double R = GetDistance(position, initialPosition); // the distance of the fireball from the initial-position
	const bool a = (R >= MAX_FIREBALL_RADIUS - TILE_SIZE / 2); // ball arrived to max position
	const D2D1_SIZE_F lvlSize = engine->GetLevelSize();
	const bool b = position.x < 0 || lvlSize.width < position.x || position.y < 0 || lvlSize.height < position.y; // exit from level size
	return remove || a || b;
}
void Barnacle::FireBall::ArrivedToPlayer()
{
	remove = true;
}

void Barnacle::FireBall::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	LevelUIElementBase::ChangeZoomRatio(oldTileSize, newTileSize);
	ChangePointZoomRatio(speed, oldTileSize, newTileSize);
}

D2D1_RECT_F Barnacle::FireBall::GetRect() const
{
	D2D1_SIZE_F size = GetSize();
	return D2D1::RectF(
		position.x,
		position.y - size.height,
		position.x + size.width,
		position.y
	);
}
