#pragma once

#include "UIElementBase.h"


#define LEVEL_WIDTH 100
#define LEVEL_HEIGHT 50


class LevelDisplay_Window;


struct EnemyData
{
	string name;
	D2D1_POINT_2L initPos, finPos;
	float timeUp = 1, timeDown = 3;
	bool used;
};
struct SpringboardData
{
	D2D1_POINT_2F position;
	float force;
	bool used;
};
struct MovingTileData
{
	D2D1_POINT_2L initPos, finPos;
	short tile_id;
	bool used;
};


class LevelMap : public UIElementBase
{
public:
	LevelMap(LevelDisplay_Window* window, D2D1::ColorF& dstBgColor);

	void Logic(float elapsedTime) override;
	void Draw() override;

	void ChangeTile(D2D1_POINT_2U mousePos);
	D2D1_POINT_2U GetTilePosition(D2D1_POINT_2U mousePos);

	void SetPlayerInitialPosition(D2D1_POINT_2U mousePos);

	void Load(wstring filename);
	void Save(wstring filename);

	void Clear();
	
private:
	void AddMovingTile(D2D1_POINT_2U tilePos);
	void AddEnemy(D2D1_POINT_2U tilePos);

	bool InCameraFrame(D2D1_RECT_F rc);
	void RemoveOffset(D2D1_RECT_F& rc);


	json lvlJson;
	vector<EnemyData> enemies;
	vector<SpringboardData> springs;
	vector<MovingTileData> movingTiles;
	LevelDisplay_Window* const window;
	D2D1::ColorF& bgColor;
	D2D1_SIZE_U wndSize;
	D2D1_POINT_2U playerInitPos;
	bool chooseFinalPosition;
};
