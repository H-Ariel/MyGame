#pragma once

#include "LevelUIElementBase.h"


class LiquidsMap : public LevelUIElementBase
{
public:
	LiquidsMap(LevelEngine* engine);

	void Logic(float elapsedTime) override;
	void Draw() const override;

	void PlayerCollides();

	void SetValue(int x, int y, short tileId);

	void AllocNew(D2D1_SIZE_U newSize);
	void Clear();


private:
	vector<vector<short>> tilesMap;
	D2D1_SIZE_U mapSize;
};
