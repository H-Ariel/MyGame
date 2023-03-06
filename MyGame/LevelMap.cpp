#include "LevelMap.h"
#include "CollisionDistances.h"
#include "LevelEngine.h"


LevelMap::LevelMap(LevelEngine* engine)
	: LevelUIElementBase(engine), mapSize({ 0, 0 })
{
}

void LevelMap::Draw() const
{
	D2D1_RECT_F dst;
	const D2D1_SIZE_U wndSize = AssetsManager::GetWindowSize();
	const D2D1_POINT_2F offset = engine->GetOffset();
	const int startX = max(0, (int)(offset.x) / TILE_SIZE);
	const int startY = max(0, (int)(offset.y) / TILE_SIZE);
	const int endX = min((int)mapSize.width, (int)(offset.x + wndSize.width) / TILE_SIZE + 1);
	const int endY = min((int)mapSize.height, (int)(offset.y + wndSize.height) / TILE_SIZE + 1);
	int i, j;

	for (i = startY; i < endY; i++)
	{
		for (j = startX; j < endX; j++)
		{
			dst = D2D1::RectF(j * TILE_SIZE_F, i * TILE_SIZE_F, (j + 1) * TILE_SIZE_F, (i + 1) * TILE_SIZE_F);

			engine->RemoveOffset(dst);

			AssetsManager::DrawTile(dst, tilesMap[i][j].tile_value);
			AssetsManager::DrawTile(dst, tilesMap[i][j].foreground_tile_value);

#ifdef _DEBUG
			// draw net
			AssetsManager::DrawRectangle(dst, D2D1::ColorF::Red);
#endif
		}
	}
}

#define CollisionCheck(dir) if (cumulatedCollision.dir != 0) for (cumulatedCollision.dir = 0, i = 0; i < collisionsNumber; cumulatedCollision.dir = fmax(cumulatedCollision.dir, collisions[i++].dir));
D2D1_RECT_F LevelMap::PlayerCollides()
{
	D2D1_RECT_F collisions[5] = {}, cumulatedCollision = {}, collisionRect = {};

	const D2D1_RECT_F playerRect = engine->player.GetRect();
	const int startX = max((int)(playerRect.left / TILE_SIZE), 0);
	const int startY = max((int)(playerRect.top / TILE_SIZE), 0);
	const int endX = min((int)(playerRect.right / TILE_SIZE), (int)mapSize.width - 1);
	const int endY = min((int)(playerRect.bottom / TILE_SIZE), (int)mapSize.height - 1);

	int collisionsNumber = 0, i = 0, j = 0;


	for (i = startY; i <= endY; i++)
	{
		for (j = startX; j <= endX; j++)
		{
			collisionRect = CollisionDistances::GetCollision(playerRect, D2D1::RectF(j * TILE_SIZE_F, i * TILE_SIZE_F, (j + 1) * TILE_SIZE_F, (i + 1) * TILE_SIZE_F));
			if (!CollisionDistances::IsEmpty(collisionRect))
			{
				switch (tilesMap[i][j].type)
				{
				case TileType::Solid:
					if (IsLock(tilesMap[i][j].tile_value))
					{
						short key = KeyOf(tilesMap[i][j].tile_value);
						if (engine->player.HasItem(key))
						{
							tilesMap[i][j].tile_value = Empty;
							tilesMap[i][j].type = TileType::None;
							engine->player.RemoveItem(key);

							continue; // if we remove the lock, there is no collision
						}
					}

					// add this collision to the list
					collisions[collisionsNumber] = collisionRect;

					CollisionDistances::keepSmallest(collisions[collisionsNumber]);

					// add the collision details to the cummulated collision
					cumulatedCollision.top += collisions[collisionsNumber].top;
					cumulatedCollision.bottom += collisions[collisionsNumber].bottom;
					cumulatedCollision.left += collisions[collisionsNumber].left;
					cumulatedCollision.right += collisions[collisionsNumber].right;

					collisionsNumber++;
					break;


				case TileType::Ladder:
					engine->player.OnLadder = true;
					// set the player position on the ladder easily for the user (if he stop move)
					if ((engine->player.GoesUp || engine->player.GoesDown) && !(engine->player.GoesLeft || engine->player.GoesRight))
						engine->player.position.x = j * TILE_SIZE_F;
					break;


				case TileType::Door:
					engine->Win();
					return D2D1::RectF();


				case TileType::Collectable:
					engine->player.CollectItem(tilesMap[i][j].tile_value);
					tilesMap[i][j].tile_value = Empty;
					break;


				case TileType::Obstacle:
					engine->player.Disqualification(collisionRect);
					return D2D1::RectF(); // this is no collision with block, and if we continue it will stop to hurt


				default:
					break;
				}
			}
		}
	}


	// whichever side collides the most, that side is taken into consideration
	CollisionDistances::keepLargest(cumulatedCollision);

	CollisionCheck(top);
	CollisionCheck(bottom);
	CollisionCheck(left);
	CollisionCheck(right);

	return cumulatedCollision;
}

void LevelMap::AllocNew(D2D1_SIZE_U newSize)
{
	Clear();

	mapSize = newSize;
	for (UINT32 j, i = 0; i < mapSize.height; i++)
	{
		tilesMap.push_back(vector<TileCell>());
		for (j = 0; j < mapSize.width; j++)
		{
			tilesMap[i].push_back(TileCell(Empty));
		}
	}
}
void LevelMap::Clear()
{
	tilesMap.clear();
	mapSize = { 0,0 };
}


LevelMap::TileCell::TileCell(short tileId)
	: tile_value(tileId), foreground_tile_value(Empty), type(GetTileType(tileId))
{
}
