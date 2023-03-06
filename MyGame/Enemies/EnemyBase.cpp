#include "EnemyBase.h"
#include "..\LevelEngine.h"
#include "..\CollisionDistances.h"

// include all enemies headers:
#include "HorizontalEnemy.h"
#include "FlyingEnemy.h"
#include "Snail.h"
#include "Ghost.h"
#include "Fish.h"
#include "Snake.h"
#include "Barnacle.h"



#define CREATE_ENEMY_PAIR(x) { EnemyTypes:: x, #x }
const map<EnemyBase::EnemyTypes, const char*> EnemyBase::EnemiesMap = {
	CREATE_ENEMY_PAIR(ladyBug),
	CREATE_ENEMY_PAIR(mouse),
	CREATE_ENEMY_PAIR(slimePink),
	CREATE_ENEMY_PAIR(slimeGreen),
	CREATE_ENEMY_PAIR(slimeBlue),
	CREATE_ENEMY_PAIR(wormGreen),
	CREATE_ENEMY_PAIR(wormPink),
	CREATE_ENEMY_PAIR(spider),
	CREATE_ENEMY_PAIR(frog),
	CREATE_ENEMY_PAIR(snail),
	CREATE_ENEMY_PAIR(ghost),
	CREATE_ENEMY_PAIR(bat),
	CREATE_ENEMY_PAIR(bee),
	CREATE_ENEMY_PAIR(fly),
	CREATE_ENEMY_PAIR(spinner),
	CREATE_ENEMY_PAIR(slimeBlock),
	CREATE_ENEMY_PAIR(grassBlock),
	CREATE_ENEMY_PAIR(fishBlue),
	CREATE_ENEMY_PAIR(fishGreen),
	CREATE_ENEMY_PAIR(fishPink),
	CREATE_ENEMY_PAIR(snakeGreen),
	CREATE_ENEMY_PAIR(snakeRed),
	CREATE_ENEMY_PAIR(barnacle)
};
#undef CREATE_ENEMY_PAIR

const vector<EnemyBase::EnemyTypes> EnemyBase::ENDLESS_LIVES_ENEMIES = {
	EnemyTypes::snakeGreen,
	EnemyTypes::snakeRed,
	EnemyTypes::fishBlue,
	EnemyTypes::fishGreen,
	EnemyTypes::fishPink,
	EnemyTypes::ghost,
	EnemyTypes::barnacle,
	EnemyTypes::bat,
	EnemyTypes::bee,
	EnemyTypes::fly,
	EnemyTypes::spinner,
	EnemyTypes::slimeBlock,
	EnemyTypes::grassBlock
};


EnemyBase::EnemyBase(LevelEngine* engine, D2D1_POINT_2F initialPosition, EnemyTypes type)
	: Character(engine, initialPosition, 1), type(type)
{
	position.y += TILE_SIZE;
}
bool EnemyBase::CanDisqualify() const
{
	return true;
}
void EnemyBase::Draw() const
{
	D2D1_RECT_F dst = GetRect();

	if (engine->InCameraFrame(dst))
	{
		engine->RemoveOffset(dst);
		AssetsManager::DrawEnemy(dst, GetMoveType(), forward);
	}
}
void EnemyBase::PlayerCollides()
{
	D2D1_RECT_F collision = CollisionDistances::GetCollision(engine->player.GetRect(), GetRect()); // the collision according to the player
	CollisionDistances::keepSmallest(collision);

	if (!CollisionDistances::IsEmpty(collision) && engine->player.IsImmortal())
	{
		lives = 0;
	}

	if (collision.bottom > 0 && engine->player.IsFalling())
	{
		if (FIND_IN_ARRAY(ENDLESS_LIVES_ENEMIES, type))
		{
			// the player's bottom collides the enemy's top, and for the above enemies do not die from it
			engine->player.Disqualification(collision);
		}
		else
		{
			// if we hit the enemy from top, disqualify that enemy
			Disqualification();
			engine->player.Jump(Player::JumpTypes::SmallJump);
		}
	}
	else if (collision.left > 0 || collision.right > 0 || collision.top > 0)
	{
		// if we hit the enemy from any other direction, disqualification
		if (CanDisqualify())
		{
			engine->player.Disqualification(collision);
		}
		else
		{
			// if we can't disqualify the player we stop him

			if (collision.left > 0)
			{
				engine->player.StopMovingLeft(collision.left);
			}
			if (collision.right > 0)
			{
				engine->player.StopMovingRight(collision.right);
			}
		}
	}
}
D2D1_SIZE_F EnemyBase::GetSize() const
{
	return AssetsManager::GetEnemySize(GetMoveType());
}

EnemyBase* EnemyBase::CreateEnemy(LevelEngine* engine, const json& enemyData)
{
	const string name = enemyData["name"];
	const D2D1_POINT_2F initialPosition = GetPointF(enemyData["initial-position"], TILE_SIZE_F);
	const D2D1_POINT_2F finalPosition = GetPointF(enemyData["final-position"], TILE_SIZE_F);
	const EnemyTypes type = GetType(name);

	EnemyBase* enemy = nullptr;
	switch (type)
	{
	case EnemyTypes::ladyBug:
	case EnemyTypes::mouse:
	case EnemyTypes::slimePink:
	case EnemyTypes::slimeGreen:
	case EnemyTypes::slimeBlue:
	case EnemyTypes::wormGreen:
	case EnemyTypes::wormPink:
	case EnemyTypes::spider:
	case EnemyTypes::frog:
		enemy = DBG_NEW HorizontalEnemy(engine, initialPosition, finalPosition.x, type);
		break;

	case EnemyTypes::snail:
		enemy = DBG_NEW Snail(engine, initialPosition, finalPosition.x);
		break;

	case EnemyTypes::ghost:
		enemy = DBG_NEW Ghost(engine, initialPosition, finalPosition.x);
		break;

	case EnemyTypes::bat:
	case EnemyTypes::bee:
	case EnemyTypes::fly:
	case EnemyTypes::spinner:
	case EnemyTypes::slimeBlock:
	case EnemyTypes::grassBlock:
		enemy = DBG_NEW FlyingEnemy(engine, initialPosition, finalPosition, type);
		break;

	case EnemyTypes::fishBlue:
	case EnemyTypes::fishGreen:
	case EnemyTypes::fishPink:
		enemy = DBG_NEW Fish(engine, initialPosition, finalPosition.y, type);
		break;

	case EnemyTypes::snakeGreen:
	case EnemyTypes::snakeRed: {
		float timeUp = 1, timeDown = 3;
		if (enemyData.contains("time-up")) timeUp = enemyData["time-up"];
		if (enemyData.contains("time-down")) timeDown = enemyData["time-down"];
		enemy = DBG_NEW Snake(engine, initialPosition, finalPosition.y, timeUp, timeDown, type);
	}	break;

	case EnemyTypes::barnacle: {
		float timeDown = 5;
		if (enemyData.contains("time-down")) timeDown = enemyData["time-down"];
		enemy = DBG_NEW Barnacle(engine, initialPosition, timeDown);
	}	break;

	default:
		LogFile::WriteLog("unknown enemy: " + name, LogFile::Warning);
	}

	return enemy;
}

EnemyBase::EnemyTypes EnemyBase::GetType(string name)
{
	for (const auto& i : EnemiesMap)
	{
		if (i.second == name)
			return i.first;
	}
	return EnemyTypes::NONE;
}
string EnemyBase::GetName(EnemyTypes type)
{
	return EnemiesMap.at(type);
}
