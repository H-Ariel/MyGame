#pragma once

#include "framework.h"
#include "FontData.h"


// Manager of the Assets and GUI window
// todo: rename it ? (but first find better name)
class AssetsManager
{
public:
	static void Initialize(const TCHAR WindowClassName[], void* lpParam);
	static void Finalize();

	static string GetLevelFileName(int levelNumber);

	static HWND GetHwnd();
	static D2D1_SIZE_U GetWindowSize();
	static void SetTitle(wstring str);

	static void ResizeRenderTarget(D2D1_SIZE_U newSize);


	static void DrawPlayer(D2D1_RECT_F dst, string moveType, bool forward);
	static D2D1_SIZE_F GetPlayerSize(string name);

	static void DrawEnemy(D2D1_RECT_F dst, string moveType, bool forward, float angle = 0);
	static D2D1_SIZE_F GetEnemySize(string name);

	static void DrawTile(D2D1_RECT_F dst, short tileId, float angle = 0);

	static void DrawHud(D2D1_RECT_F dst, short hudId);
	static D2D1_SIZE_F GetHudSize(short hudId);


	static void FillRectangle(D2D1_RECT_F rect, D2D1::ColorF color);
	static void DrawRectangle(D2D1_RECT_F rect, D2D1::ColorF color, float width = 1);
	static void DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, D2D1::ColorF color, float width = 1); // todo: delete this function ?
	static void DrawText(wstring text, FontData font, D2D1_RECT_F& layoutRect, D2D1::ColorF color);
	

private:
	// Draw a bitmap image
	// `bitmap` - the bitmap to draw.
	// `dstRect` - destination rectangle on window.
	// `srcRect` - source rectangle from the bitmap image (in pixels).
	// `reversed` - if this flag is `true` the bitmap will be drawn reversed.
	// `opacity` - the opacity of the image. this value shuold be between 0 and 1.
	// `alpha` - the angle at which the image will be drawn (the units are degrees).
	static void DrawBitmap(ID2D1Bitmap* bitmap, D2D1_RECT_F dstRect, D2D1_RECT_F srcRect, bool reversed = false, float opacity = 1.0f, float alpha = 0);

	static ID2D1Bitmap* LoadBitapImage(wstring imageFile);
	static ID2D1Bitmap* LoadBitapImage(string imageFile);


	static HWND hwnd;
	static ID2D1HwndRenderTarget* RenderTarget;
	static ID2D1Factory* D2DFactory;
	static IDWriteFactory* DWriteFactory;
	static IWICImagingFactory* WICImagingFactory;
	static IDWriteFontCollection* FontCollection;

	static ID2D1Bitmap* TilesBitmap;
	static ID2D1Bitmap* PlayerBitmap;
	static ID2D1Bitmap* EnemiesBitmap;
	static ID2D1Bitmap* HudBitmap;

	static vector<D2D1_RECT_F> TilesPositions;
	static map<string, D2D1_RECT_F> PlayerPositions;
	static map<string, D2D1_RECT_F> EnemiesPositions;
	static vector<D2D1_RECT_F> HudPositions;


	friend class EngineBase;
};
