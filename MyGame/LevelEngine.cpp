#include "LevelEngine.h"
#include "TilesCodes.h"

#include "Enemies\EnemyBase.h"
#include "MovingTile.h"
#include "SpringBoard.h"
#include "CoinBox.h"
#include "ItemBox.h"
#include "MutableObject.h"

#include <fstream>


#define MIN_TILE_SIZE 20
#define MAX_TILE_SIZE 120

#define MIN_OFFSET_X 0
#define MAX_OFFSET_X (float)(levelMap.mapSize.width * TILE_SIZE - wndSize.width)
#define MIN_OFFSET_Y 0
#define MAX_OFFSET_Y (float)(levelMap.mapSize.height * TILE_SIZE - wndSize.height)

#define offset levelMap.position // this is the offset of the camera frame


LevelEngine::LevelEngine()
	: player(this, levelMap.mapSize), hud(this), levelMap(this), liquidsMap(this)
{
	Clear();
#ifndef _DEBUG
	backgroundColor = 0x67aeda;
#endif
}
LevelEngine::~LevelEngine()
{
	Release();
}

void LevelEngine::LoadLevel(int levelNumber)
#define CHECK_IF_KEY_EXISTS(k) if (!lvlJson.contains(k)) throw exception(("the level json does not contains \"" + string(k) + "\" attribute").c_str());
{
	LogFile::WriteLog("load level " + to_string(levelNumber), LogFile::Information);


	ifstream lvlFile(AssetsManager::GetLevelFileName(levelNumber));
	if (!lvlFile.is_open())
	{
		char msg[32];
		sprintf_s(msg, "unable to open level %d", levelNumber);
		throw exception(msg);
	}

	json lvlJson;
	lvlFile >> lvlJson;
	lvlFile.close();

	CHECK_IF_KEY_EXISTS("player");
	CHECK_IF_KEY_EXISTS("size");
	CHECK_IF_KEY_EXISTS("tiles-map");


	D2D1_SIZE_U mapSize = D2D1::SizeU((int)lvlJson["size"]["width"], (int)lvlJson["size"]["height"]);

	player.position = GetPointF(lvlJson["player"]["initial-position"], TILE_SIZE_F);

	levelMap.AllocNew(mapSize);
	liquidsMap.AllocNew(mapSize);


	int idx = 0;
	lvlJson["tiles-map"].list_iterate([&](const json& i) {
		int x = idx % levelMap.mapSize.width, y = idx / levelMap.mapSize.width;

		if (IsCoinBox(i))
		{
			coinBoxes.push_back(CoinBox(this, x, y));
		}
		else if (IsItemBox(i))
		{
			itemBoxes.push_back(ItemBox(this, x, y));
		}
		else if (IsLiquid(i))
		{
			liquidsMap.SetValue(x, y, i);
			levelMap.tilesMap[y][x] = Empty;
		}
		else
		{
			levelMap.tilesMap[y][x] = LevelMap::TileCell(i);
		}

		idx++;
		});

	if (lvlJson.contains("foreground-tiles-map"))
	{
		idx = 0;
		lvlJson["foreground-tiles-map"].list_iterate([&](const json& i) {
			int x = idx % levelMap.mapSize.width, y = idx / levelMap.mapSize.width;

			if (isMutableObject(i))
			{
				mutableObjects.push_back(MutableObject(this, x, y, i));
			}
			else
			{
				levelMap.tilesMap[y][x].foreground_tile_value = i;
			}

			idx++;
			});
	}

	if (lvlJson.contains("enemies"))
	{
		lvlJson["enemies"].list_iterate([&](const json& enemyData) {
			EnemyBase* pEnemy = EnemyBase::CreateEnemy(this, enemyData);
			if (pEnemy != nullptr)
				enemies.push_back(pEnemy);
			});
	}

	if (lvlJson.contains("moving-tiles"))
	{
		lvlJson["moving-tiles"].list_iterate([&](const json& tileData) {
			movingTiles.push_back(MovingTile(this,
				GetPointF(tileData["initial-position"]),
				GetPointF(tileData["final-position"]),
				tileData["id"]));
			});
	}

	if (lvlJson.contains("springboards"))
	{
		lvlJson["springboards"].list_iterate([&](const json& spData) {
			springBoards.push_back(SpringBoard(this, spData["x"], spData["y"], spData["force"]));
			});
	}

#ifndef _DEBUG
	if (lvlJson.contains("background-color"))
	{
		backgroundColor = D2D1::ColorF((int)lvlJson["background-color"]);
	}
#endif


	AddUIElement(&liquidsMap);
	AddUIElement(&levelMap);
	for (MutableObject& i : mutableObjects) AddUIElement(&i);
	for (EnemyBase* i : enemies) AddUIElement(i);//todo: `push_front` for snakes and fishes
	for (MovingTile& i : movingTiles) AddUIElement(&i);
	for (SpringBoard& i : springBoards) AddUIElement(&i);
	AddUIElement(&player);
	for (CoinBox& i : coinBoxes) AddUIElement(&i);
	for (ItemBox& i : itemBoxes) AddUIElement(&i);
	AddUIElement(&hud);
}
void LevelEngine::Clear()
{
	Release();

	offset = { 0,0 };
	ctrlPressed = false;
	state = State::Run;
	StopEngine = false;

	levelMap.Clear();
	liquidsMap.Clear();
	enemies.clear();
	movingTiles.clear();
	springBoards.clear();
	player.Reset();
	coinBoxes.clear();
	itemBoxes.clear();
	mutableObjects.clear();

	RemoveAllUIElements();
}

void LevelEngine::Logic(float elapsedTime)
{
	EngineBase::Logic(elapsedTime);


	if (player.HasLives()) // all the logic below is happening only if the player is not dead
	{
		D2D1_RECT_F collDist = levelMap.PlayerCollides(); // collision distances
		if (collDist.bottom > 0) player.StopFalling(collDist.bottom);
		if (collDist.left   > 0) player.StopMovingLeft(collDist.left);
		if (collDist.right  > 0) player.StopMovingRight(collDist.right);
		if (collDist.top    > 0) player.BounceTop();


		liquidsMap.PlayerCollides();


		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i]->PlayerCollides();
			
			if (!enemies[i]->HasLives())
			{
				RemoveUIElement(enemies[i]);
				delete enemies[i];
				enemies.erase(enemies.begin() + i);
				i -= 1; // do not skip enemies after delete one of them
			}
		}


		// change the display offset according to player position, but clamp it to the limits (the player should to be in screen center)
		
		const D2D1_SIZE_U wndSize = AssetsManager::GetWindowSize();
		
		offset.x = player.position.x - wndSize.width / 2.0f;
		if (offset.x < MIN_OFFSET_X) offset.x = MIN_OFFSET_X;
		if (offset.x > MAX_OFFSET_X) offset.x = MAX_OFFSET_X;
		offset.y = player.position.y - wndSize.height / 2.0f;
		if (offset.y < MIN_OFFSET_Y) offset.y = MIN_OFFSET_Y;
		if (offset.y > MAX_OFFSET_Y) offset.y = MAX_OFFSET_Y;
	}
	else
	{
		Lose();
	}
}
void LevelEngine::OnKeyUp(int key)
{
	switch (key)
	{
	case 'A': case VK_LEFT:  player.GoesLeft  = false; break;
	case 'D': case VK_RIGHT: player.GoesRight = false; break;
	case 'W': case VK_UP:	player.GoesUp     = false; break;
	case 'S': case VK_DOWN:	player.GoesDown   = false; break;
	case VK_CONTROL: ctrlPressed = false; break;
	default: break;
	}
}
void LevelEngine::OnKeyDown(int key)
{
	switch (key)
	{
	case'A': case VK_LEFT:	player.GoesLeft  = true; break;
	case'D': case VK_RIGHT:	player.GoesRight = true; break;
	case'W': case VK_UP:	player.GoesUp    = true; break;
	case'S': case VK_DOWN:	player.GoesDown  = true; break;
	case VK_CONTROL: ctrlPressed = true; break;
	default: break;
	}

	if (ctrlPressed)
	{
		switch (key)
		{
		case VK_ADD:		ChangeZoom(10); break;
		case VK_SUBTRACT:	ChangeZoom(-10); break;
		case VK_DIVIDE:		ChangeZoom(SettingManager::GetDefaultTileSize() - TILE_SIZE); break;
		default: break;
		}
	}
}

void LevelEngine::RemoveOffset(D2D1_RECT_F& rect) const
{
	rect.left -= offset.x;
	rect.right -= offset.x;
	rect.top -= offset.y;
	rect.bottom -= offset.y;
}
bool LevelEngine::InCameraFrame(D2D1_RECT_F& rect) const
{
	D2D1_SIZE_U wndSize = AssetsManager::GetWindowSize();
	return offset.x < rect.right
		&& rect.left < wndSize.width + offset.x
		&& offset.y < rect.bottom
		&& rect.top < wndSize.height + offset.y;
}
D2D1_SIZE_F LevelEngine::GetLevelSize() const
{
	return D2D1::SizeF(levelMap.mapSize.width * TILE_SIZE_F, levelMap.mapSize.height * TILE_SIZE_F);
}
D2D1_POINT_2F LevelEngine::GetOffset() const
{
	return offset;
}
void LevelEngine::AddItemToLevelMap(D2D1_POINT_2F pos, short itemId)
{
	if (IsCollectable(itemId))
	{
		levelMap.tilesMap[(int)pos.y][(int)pos.x].tile_value = itemId;
		levelMap.tilesMap[(int)pos.y][(int)pos.x].type = TileType::Collectable;
	}
}

void LevelEngine::Win()
{
	state = State::Win;
	StopEngine = true;
}
void LevelEngine::Lose()
{
	state = State::Lose;
	StopEngine = true;
}

LevelEngine::State LevelEngine::GetState() const
{
	return state;
}

void LevelEngine::ChangeZoom(int zoomDelta)
{
	int newSize = TILE_SIZE + zoomDelta;
	if (InRange(newSize, MIN_TILE_SIZE, MAX_TILE_SIZE))
	{
		for (UIElementBase* e : elementsList)
		{
			((LevelUIElementBase*)e)->ChangeZoomRatio(TILE_SIZE, newSize);
		}
		// we don't need change `LevelEngine::offset` because it update automaticly every loop iteraation
		TILE_SIZE = newSize;
	}
}

void LevelEngine::Release()
{
	for (EnemyBase* e : enemies)
		delete e;
}
