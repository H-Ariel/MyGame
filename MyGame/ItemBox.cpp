#include "ItemBox.h"
#include "LevelEngine.h"
#include "TilesCodes.h"
#include "CollisionDistances.h"


#define TIME_COIN_DISPLAYED 0.35f

static const vector<short> SECRETS_ITEMS = { // todo: add more items
	GemBlue, // super speed
	ShieldSilver, // immortal
	Heart // get more lives
};


ItemBox::ItemBox(LevelEngine* engine, int x, int y)
	: LevelUIElementBase(engine, { x * TILE_SIZE_F, y * TILE_SIZE_F }),
	collected(false), id(ItemBox_disabled)
{
}

void ItemBox::Logic(float elapsedTime)
{
	id = collected ? ItemBox_disabled : ItemBox_enabled;

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
			if (!collected)
			{
				collected = true;
				// put cool item above the item-box
				engine->AddItemToLevelMap({ position.x / TILE_SIZE, position.y / TILE_SIZE - 1 },
					SECRETS_ITEMS[rand() % SECRETS_ITEMS.size()]
				);
			}
		}
	}
}

void ItemBox::Draw() const
{
	D2D1_RECT_F dst = GetRect();
	if (engine->InCameraFrame(dst))
	{
		engine->RemoveOffset(dst);
		AssetsManager::DrawTile(dst, id);
	}
}
