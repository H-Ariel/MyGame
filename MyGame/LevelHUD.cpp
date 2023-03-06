#include "LevelHUD.h"
#include "TilesCodes.h"
#include "LevelEngine.h"


LevelHUD::LevelHUD(LevelEngine* engine)
	: LevelUIElementBase(engine)
{
}

void LevelHUD::Draw() const
{
	// draw red frame (alert for user)
	if (engine->player.IsHurt())
	{
		D2D1_SIZE_U wndSz = AssetsManager::GetWindowSize();
		AssetsManager::DrawRectangle(D2D1::RectF(0, 0, wndSz.width - 1.0f, wndSz.height - 1.0f), D2D1::ColorF::Red, 30);
	}

	// draw number of coins
	AssetsManager::DrawTile(D2D1::RectF(0, 0, TILE_SIZE_F, TILE_SIZE_F), CoinGold);
	DrawNumber(TILE_SIZE_F, TILE_SIZE * 0.3f, TILE_SIZE * 0.4f, engine->player.GetCoins());

	// draw number of lives
	AssetsManager::DrawTile(D2D1::RectF(TILE_SIZE * 0.15f, TILE_SIZE_F, TILE_SIZE * 0.85f, TILE_SIZE * 1.7f), Heart);
	DrawNumber(TILE_SIZE_F, TILE_SIZE * 1.1f, TILE_SIZE * 0.4f, engine->player.GetLives());

	// draw keys
	list<short> items;
	if (engine->player.HasItem(KeyBlue  )) items.push_back(KeyBlue  );
	if (engine->player.HasItem(KeyGreen )) items.push_back(KeyGreen );
	if (engine->player.HasItem(KeyRed   )) items.push_back(KeyRed   );
	if (engine->player.HasItem(KeyYellow)) items.push_back(KeyYellow);
	D2D1_RECT_F dst = D2D1::RectF(TILE_SIZE * 0.25f, TILE_SIZE * 1.7f, TILE_SIZE * 0.75f, TILE_SIZE * 2.2f);
	for (const short& i : items)
	{
		AssetsManager::DrawTile(dst, i);
		dst.right += TILE_SIZE / 2;
		dst.left += TILE_SIZE / 2;
	}

	// draw stars
	int starsNum = engine->player.GetStars();
	if (starsNum)
	{
		const float Y = TILE_SIZE / 7.0f;
		const float ADD_TO_X = TILE_SIZE + Y;
		const float x = (AssetsManager::GetWindowSize().width - ADD_TO_X * (starsNum + 1)) / 2;

		D2D1_RECT_F dstRc = D2D1::RectF(x, Y, x + TILE_SIZE, Y + TILE_SIZE);

		while (starsNum--)
		{
			dstRc.left += ADD_TO_X;
			dstRc.right += ADD_TO_X;
			AssetsManager::DrawTile(dstRc, Star);
		}
	}
}

void LevelHUD::DrawNumber(float x, float y, float dstHeight, int N) const
{
	list<int8_t> list;

	// split N to list of digits
	do {
		list.push_front(N % 10);
		N /= 10;
	} while (N);

	list.push_front(10); // push the X

	for (const int8_t& i : list)
	{
		D2D1_SIZE_F srcSize = AssetsManager::GetHudSize(i);
		float dstWidth = srcSize.width / srcSize.height * dstHeight;
		AssetsManager::DrawHud(D2D1::RectF(x, y, x + dstWidth, y + dstHeight), i);
		x += dstWidth;
	}
}
