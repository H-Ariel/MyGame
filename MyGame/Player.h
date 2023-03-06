#pragma once

#include "Character.h"


class MovingTile;


class Player : public Character
{
public:
	enum class JumpTypes
	{
		RegularJump,
		SmallJump,
		DisqualificationJump,
		SpringboardJump
	};


	Player(LevelEngine* engine, const D2D1_SIZE_U& mapSize);

	void Reset();

	void Logic(float elapsedTime) override;
	void Draw() const override;

	void Disqualification(D2D1_RECT_F collisionRect);

	void Jump(JumpTypes jumpType, float force = -1); // the `force` argument used in case of `SpringboardJump`
	void BounceTop();
	void StopFalling(float collisionSize);
	void StopMovingLeft(float collisionSize);
	void StopMovingRight(float collisionSize);

	bool HasItem(short itemId) const;
	void CollectItem(short itemId);
	void RemoveItem(short itemId);

	int GetLives() const;
	int GetCoins() const;
	int GetStars() const;
	float GetSpeedY() const;
	bool IsFalling() const;
	bool IsHurt() const;
	void NoLives();

	void SetSuperPower(short itemId);
	void EndSuperPower();
	bool IsImmortal() const;


	bool GoesLeft, GoesRight, GoesUp, GoesDown;
	bool OnLadder;
	MovingTile* movingTile;

private:
	D2D1_SIZE_F GetSize() const override;
	string GetMoveType() const override;


	enum class SuperPower
	{
		None,

		SuperSpeed,
		Immortal
	};
	SuperPower superPower;

	const D2D1_SIZE_U& mapSize; // When the level size changes (loading a new level), it changes here as well
	vector<short> itemsCollected;
	D2D1_POINT_2F prevPosition;
	float superPowerTimer;
	short coins, stars;
	bool hurt;
	bool leftCollision, rightCollision;
};
