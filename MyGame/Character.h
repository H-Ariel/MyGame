#pragma once

#include "LevelUIElementBase.h"


class Character : public LevelUIElementBase
{
public:
	Character(LevelEngine* engine, D2D1_POINT_2F initialPosition, int lives);
	virtual ~Character();

	virtual void Logic(float elapsedTime) override = 0;
	virtual void Draw() const override = 0;

	void ChangeZoomRatio(int oldTileSize, int newTileSize) override;
	D2D1_RECT_F GetRect() const override;

	virtual void Disqualification();

	bool HasLives();


protected:
	virtual D2D1_SIZE_F GetSize() const override = 0;
	virtual string GetMoveType() const = 0;


	int lives;
	bool forward;
	// speed.x -> speed of X-axis
	// speed.y -> speed of Y-axis
	// units: tiles per seconds
	// for example: `position.x = speed.x * elapsedTime`
	D2D1_POINT_2F speed;
};
