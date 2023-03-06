#include "CoinBox.h"
#include "LevelEngine.h"
#include "TilesCodes.h"
#include "CollisionDistances.h"


#define TIME_COIN_DISPLAYED 0.35f


CoinBox::CoinBox(LevelEngine* engine, int x, int y)
	: LevelUIElementBase(engine, { x * TILE_SIZE_F, y * TILE_SIZE_F }),
	amount(3), timeCounter(0), id(CoinBox_enabled)
{
}

void CoinBox::Logic(float elapsedTime)
{
	id = amount > 0 ? CoinBox_enabled : CoinBox_disabled;

	D2D1_RECT_F colRect = CollisionDistances::GetCollision(engine->player.GetRect(), GetRect());

	if (!CollisionDistances::IsEmpty(colRect))
	{
		CollisionDistances::keepSmallest(colRect);

		if (colRect.left > 0)
			engine->player.StopMovingLeft(colRect.left);
		else if (colRect.right > 0)
			engine->player.StopMovingRight(colRect.right);
		else if (colRect.bottom > 0)
			engine->player.StopFalling(colRect.bottom);

		else if (colRect.top > 0)
		{
			engine->player.BounceTop();
			if (amount > 0)
			{
				engine->player.CollectItem(CoinBronze);
				amount -= 1;
				timeCounter = TIME_COIN_DISPLAYED;
			}
		}
	}

	if (timeCounter > 0)
		timeCounter -= elapsedTime;
}

void CoinBox::Draw() const
{
	D2D1_RECT_F dst = GetRect();
	if (engine->InCameraFrame(dst))
	{
		engine->RemoveOffset(dst);
		AssetsManager::DrawTile(dst, id);

		// draw coin above the coin-box
		if (0 < timeCounter && timeCounter <= TIME_COIN_DISPLAYED)
		{
			dst.top -= TILE_SIZE;
			dst.bottom -= TILE_SIZE;
			AssetsManager::DrawTile(dst, CoinGold);
		}
	}
}
