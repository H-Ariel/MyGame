#pragma once

#include "EnemyBase.h"


class Barnacle : public EnemyBase // this enemy can't move, but it throw fire-balls on the player
{
public:
	Barnacle(LevelEngine* engine, D2D1_POINT_2F position, float throwCycleTime); // `throwCycleTime` is the time it wait before attack
	~Barnacle();

	void Logic(float elapsedTime) override;
	void Draw() const override;
	void Disqualification() override;
	string GetMoveType() const override;
	void ChangeZoomRatio(int oldTileSize, int newTileSize) override;

private:
	class FireBall : public LevelUIElementBase
	{
		// I didn't used `public EnemyBase` because it no matter. the `Barnacle` handle its fireballs.

	public:
		FireBall(LevelEngine* engine, D2D1_POINT_2F barnaclePosition, D2D1_POINT_2F playerPosition);
		void Logic(float elapsedTime) override;
		void Draw() const override;
		void ChangeZoomRatio(int oldTileSize, int newTileSize) override;
		D2D1_RECT_F GetRect() const override;
		bool shouldRemoved() const;
		void ArrivedToPlayer();

	private:
		D2D1_POINT_2F initialPosition;
		D2D1_POINT_2F speed;
		float alpha, rotAngle;
		bool remove, mul_minus;
	};

	vector<FireBall*> fireBalls;
	const float throwCycleTime;
	float timeCounter;
};
