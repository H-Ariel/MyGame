#pragma once

#include "..\Player.h"


class EnemyBase : public Character
{
public:
	virtual void Logic(float elapsedTime) override = 0;
	virtual bool CanDisqualify() const;
	void Draw() const override;
	void PlayerCollides(); // check for collides of the enemy with the player and handle it

	static EnemyBase* CreateEnemy(LevelEngine* engine, const json& enemyData);

protected:
	enum class EnemyTypes : uint8_t
	{
		NONE, // use to mark unknown enemies
		ladyBug,
		mouse,
		slimePink,
		slimeGreen,
		slimeBlue,
		wormGreen,
		wormPink,
		spider,
		frog,
		snail,
		ghost,
		bat,
		bee,
		fly,
		spinner,
		slimeBlock,
		grassBlock,
		fishBlue,
		fishGreen,
		fishPink,
		snakeGreen,
		snakeRed,
		barnacle
	};


	EnemyBase(LevelEngine* engine, D2D1_POINT_2F initialPosition, EnemyTypes type);
	D2D1_SIZE_F GetSize() const override;
	virtual ::string GetMoveType() const override = 0;

	static EnemyTypes GetType(::string name);
	static ::string GetName(EnemyTypes type);

	EnemyTypes type;

private:
	static const ::map<EnemyTypes, const char*> EnemiesMap;
	static const ::vector<EnemyTypes> ENDLESS_LIVES_ENEMIES;
};
