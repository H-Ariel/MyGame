#pragma once

#include "EngineBase.h"
#include "AssetsManager.h"
#include "LevelMap.h"
#include "Player.h"
#include "LevelHUD.h"
#include "LiquidsMap.h"


class EnemyBase;
class MovingTile;
class SpringBoard;
class CoinBox;
class ItemBox;
class MutableObject;


// window for running single level
class LevelEngine : public EngineBase
{
public:
	enum class State
	{
		Lose, // player has no more lives
		Win, // arrive to the exit door
		Run // the game is still running
	};


	LevelEngine();
	~LevelEngine() override;

	void LoadLevel(int levelNumber); // must call it after defination
	void Clear();

	void Logic(float elapsedTime) override;

	void OnKeyUp(int key) override;
	void OnKeyDown(int key) override;

	void RemoveOffset(D2D1_RECT_F& rect) const;
	bool InCameraFrame(D2D1_RECT_F& rect) const;

	D2D1_SIZE_F GetLevelSize() const; // return the level map real size
	D2D1_POINT_2F GetOffset() const;

	void AddItemToLevelMap(D2D1_POINT_2F pos, short itemId); // used in ItemBox. `pos` is in `levelMap` units.

	void Win();
	void Lose();

	State GetState() const;

	
	Player player;
	

private:
	void ChangeZoom(int zoomDelta); // change the zoom ratio
	void Release(); // release allocated memory


	LevelHUD hud;
	LevelMap levelMap;
	LiquidsMap liquidsMap;
	vector<EnemyBase*> enemies;
	vector<MovingTile> movingTiles;
	vector<SpringBoard> springBoards;
	vector<CoinBox> coinBoxes;
	vector<ItemBox> itemBoxes;
	vector<MutableObject> mutableObjects;
	State state;
	bool ctrlPressed;
};
