#include "LiquidsMap.h"
#include "LevelEngine.h"
#include "TilesCodes.h"
#include "CollisionDistances.h"


#define LIQUIDS_SPEED	(TILE_SIZE/2.0f)


LiquidsMap::LiquidsMap(LevelEngine* engine)
	: LevelUIElementBase(engine), mapSize({ 0, 0 })
{
}

void LiquidsMap::Logic(float elapsedTime)
{
	position.x += LIQUIDS_SPEED * elapsedTime;

	if (position.x > TILE_SIZE)
		position.x = 0;
}

void LiquidsMap::Draw() const
{
	D2D1_RECT_F dst;
	const D2D1_SIZE_U wndSize = AssetsManager::GetWindowSize();
	const D2D1_POINT_2F offset = engine->GetOffset();
	const int startX = max(0, (int)(offset.x) / TILE_SIZE);
	const int startY = max(0, (int)(offset.y) / TILE_SIZE);
	const int endX = min((int)mapSize.width, (int)(offset.x + wndSize.width) / TILE_SIZE + 2);
	const int endY = min((int)mapSize.height, (int)(offset.y + wndSize.height) / TILE_SIZE + 1);
	int i, j;

	for (i = startY; i < endY; i++)
	{
		for (j = startX; j < endX; j++)
		{
			dst = D2D1::RectF(
				j * TILE_SIZE_F - position.x,
				i * TILE_SIZE_F,
				(j + 1) * TILE_SIZE_F - position.x,
				(i + 1) * TILE_SIZE_F);

			engine->RemoveOffset(dst);

			AssetsManager::DrawTile(dst, tilesMap[i][j]);
		}
	}
}

void LiquidsMap::PlayerCollides()
{
	D2D1_RECT_F collisionRect = {};
	const D2D1_RECT_F playerRect = engine->player.GetRect();
	const int startX = max((int)(playerRect.left / TILE_SIZE), 0);
	const int startY = max((int)(playerRect.top / TILE_SIZE), 0);
	const int endX = min((int)(playerRect.right / TILE_SIZE), (int)mapSize.width - 1);
	const int endY = min((int)(playerRect.bottom / TILE_SIZE), (int)mapSize.height - 1);
	int i = 0, j = 0;
	for (i = startY; i <= endY; i++)
	{
		for (j = startX; j <= endX; j++)
		{
			if (tilesMap[i][j] != Empty)
			{
				collisionRect = CollisionDistances::GetCollision(playerRect, D2D1::RectF(j * TILE_SIZE_F, i * TILE_SIZE_F, (j + 1) * TILE_SIZE_F, (i + 1) * TILE_SIZE_F));
				if (!CollisionDistances::IsEmpty(collisionRect))
				{
					// check if the liquid covers the player
					if (CollisionDistances::IsTopCollision(collisionRect))
					{
						engine->player.NoLives();
						goto end_PlayerCollides;
					}
				}
			}
		}
	}

end_PlayerCollides:;
}

void LiquidsMap::SetValue(int x, int y, short tileId)
{
	tilesMap[y][x] = tileId;
	x += 1;
	tilesMap[y][x] = tileId;
}


void LiquidsMap::AllocNew(D2D1_SIZE_U newSize)
{
	Clear();

	mapSize = newSize;
	mapSize.width += 1;
	for (UINT32 j, i = 0; i < mapSize.height; i++)
	{
		tilesMap.push_back(vector<short>());
		for (j = 0; j < mapSize.width; j++)
		{
			tilesMap[i].push_back(Empty);
		}
	}
}
void LiquidsMap::Clear()
{
	tilesMap.clear();
	mapSize = { 0,0 };
}
