#include "AssetsManager.h"
#include "FontLoader.h"
#include "TilesCodes.h"


#define ASSETS_PATH			"./assets"

#define LEVELS_PATH_FORMAT	ASSETS_PATH "/maps/level_%02d.json"
#define IMAGES_PATH			ASSETS_PATH "/images"

#define TILES_IMAGE_PATH	IMAGES_PATH "/tiles.png"
#define PLAYER_IMAGE_PATH	IMAGES_PATH "/aliens.png"
#define PLAYER_JSON_PATH	IMAGES_PATH "/aliens.json"
#define ENEMIES_IMAGE_PATH	IMAGES_PATH "/enemies.png"
#define ENEMIES_JSON_PATH	IMAGES_PATH "/enemies.json"
#define HUD_IMAGE_PATH		IMAGES_PATH "/hud.png"
#define HUD_JSON_PATH		IMAGES_PATH "/hud.json"

#define FONT_FILE_PATH		ASSETS_PATH L"/fonts/Kenney Future.ttf"
#define FONT_FAMILY			L"Kenney Future"


ID2D1Bitmap* AssetsManager::TilesBitmap = nullptr;
ID2D1Bitmap* AssetsManager::PlayerBitmap = nullptr;
ID2D1Bitmap* AssetsManager::EnemiesBitmap = nullptr;
ID2D1Bitmap* AssetsManager::HudBitmap = nullptr;

map<string, D2D1_RECT_F> AssetsManager::PlayerPositions;
map<string, D2D1_RECT_F> AssetsManager::EnemiesPositions;
vector<D2D1_RECT_F> AssetsManager::TilesPositions;
vector<D2D1_RECT_F> AssetsManager::HudPositions;

HWND AssetsManager::hwnd = nullptr;
ID2D1HwndRenderTarget* AssetsManager::RenderTarget = nullptr;
ID2D1Factory* AssetsManager::D2DFactory = nullptr;
IDWriteFactory* AssetsManager::DWriteFactory = nullptr;
IWICImagingFactory* AssetsManager::WICImagingFactory = nullptr;
IDWriteFontCollection* AssetsManager::FontCollection = nullptr;



map<string, D2D1_RECT_F> GetPositionsMap(string filename)
{
	map<string, D2D1_RECT_F> m;
	json::FromFile(filename).dict_iterate([&](const string& k, const json& j) { m[k] = D2D1::RectF(j["left"], j["top"], j["right"], j["bottom"]); });
	return m;
}
vector<D2D1_RECT_F> GetPositionsVector(string filename)
{
	vector<D2D1_RECT_F> v;
	json::FromFile(filename).list_iterate([&](const json& j) { v.push_back(D2D1::RectF(j["left"], j["top"], j["right"], j["bottom"])); });
	return v;
}
vector<D2D1_RECT_F> GetTilesPositionsVector()
{
	const int IMAGES_IN_X = 32, IMAGES_IN_Y = 31;

	vector<D2D1_RECT_F> v;

	for (int j, i = 0; i < IMAGES_IN_Y; i++)
		for (j = 0; j < IMAGES_IN_X; j++)
			v.push_back(D2D1::RectF(
				j * REAL_IMAGE_SIZE,
				i * REAL_IMAGE_SIZE,
				(j + 1) * REAL_IMAGE_SIZE - 1,
				(i + 1) * REAL_IMAGE_SIZE - 1
			));

	return v;
}


void AssetsManager::Initialize(const TCHAR WindowClassName[], void* lpParam)
{
	if (hwnd != nullptr) return;

	// Create window

	THROW_IF_NULL(hwnd = CreateWindow(WindowClassName, L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, HINST_THISCOMPONENT, lpParam));

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	
	// Initializes Direct2D
	
	THROW_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory));
	THROW_IF_FAILED(D2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, GetWindowSize(), D2D1_PRESENT_OPTIONS_NONE), &RenderTarget));
	THROW_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(DWriteFactory), (IUnknown**)(&DWriteFactory)));
	THROW_IF_FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(&WICImagingFactory)));


	// Load Assets
	
	THROW_IF_NULL(TilesBitmap = LoadBitapImage(TILES_IMAGE_PATH));
	THROW_IF_NULL(PlayerBitmap = LoadBitapImage(PLAYER_IMAGE_PATH));
	THROW_IF_NULL(EnemiesBitmap = LoadBitapImage(ENEMIES_IMAGE_PATH));
	THROW_IF_NULL(HudBitmap = LoadBitapImage(HUD_IMAGE_PATH));

	PlayerPositions = GetPositionsMap(PLAYER_JSON_PATH);
	EnemiesPositions = GetPositionsMap(ENEMIES_JSON_PATH);
	TilesPositions = GetTilesPositionsVector();
	HudPositions = GetPositionsVector(HUD_JSON_PATH);

	// create custom font collection
	FontContext(DWriteFactory).CreateFontCollection(vector<wstring>({ FONT_FILE_PATH }), &FontCollection);
	

	srand((unsigned)time(nullptr)); // the random used in item-boxes
}
void AssetsManager::Finalize()
{
	SafeRelease(&TilesBitmap);
	SafeRelease(&PlayerBitmap);
	SafeRelease(&EnemiesBitmap);
	SafeRelease(&HudBitmap);

	SafeRelease(&RenderTarget);
	SafeRelease(&D2DFactory);
	SafeRelease(&DWriteFactory);
	SafeRelease(&WICImagingFactory);
	SafeRelease(&FontCollection);

	DestroyWindow(hwnd);
	hwnd = nullptr;
}

string AssetsManager::GetLevelFileName(int levelNumber)
{
	char levelFilePath[64];
	sprintf_s(levelFilePath, LEVELS_PATH_FORMAT, levelNumber);
	return levelFilePath;
}

HWND AssetsManager::GetHwnd()
{
	return hwnd;
}
D2D1_SIZE_U AssetsManager::GetWindowSize()
{
	RECT rc = {};
	THROW_IF(::GetWindowRect(hwnd, &rc) == 0);
	return D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
}
void AssetsManager::SetTitle(wstring str)
{
	SetWindowText(hwnd, str.c_str());
}

void AssetsManager::ResizeRenderTarget(D2D1_SIZE_U newSize)
{
	if (RenderTarget)
		RenderTarget->Resize(newSize);
}

void AssetsManager::DrawPlayer(D2D1_RECT_F dst, string moveType, bool forward)
{
	if (PlayerPositions.count(moveType))
		DrawBitmap(PlayerBitmap, dst, PlayerPositions.at(moveType), forward);
}
D2D1_SIZE_F AssetsManager::GetPlayerSize(string name)
{
	if (PlayerPositions.count(name))
		return GetRectSize(PlayerPositions.at(name));
	return D2D1::SizeF();
}

void AssetsManager::DrawEnemy(D2D1_RECT_F dst, string moveType, bool forward, float angle)
{
	if (EnemiesPositions.count(moveType))
		DrawBitmap(EnemiesBitmap, dst, EnemiesPositions.at(moveType), forward, 1.0f, angle);
}
D2D1_SIZE_F AssetsManager::GetEnemySize(string name)
{
	if (EnemiesPositions.count(name))
		return GetRectSize(EnemiesPositions.at(name));
	return D2D1::SizeF();
}

void AssetsManager::DrawTile(D2D1_RECT_F dst, short tileId, float angle)
{
	if (tileId != TilesCodes::Empty)
		DrawBitmap(TilesBitmap, dst, TilesPositions[tileId], false, 1.0f, angle);
}

void AssetsManager::DrawHud(D2D1_RECT_F dst, short hudId)
{
	if (0 <= hudId && hudId < HudPositions.size())
		DrawBitmap(HudBitmap, dst, HudPositions[hudId]);
}
D2D1_SIZE_F AssetsManager::GetHudSize(short hudId)
{
	if (0 <= hudId && hudId < HudPositions.size())
		return GetRectSize(HudPositions[hudId]);
	return D2D1::SizeF();
}

void AssetsManager::FillRectangle(D2D1_RECT_F rect, D2D1::ColorF color)
{
	ID2D1SolidColorBrush* brush = nullptr;
	RenderTarget->CreateSolidColorBrush(color, &brush);
	if (brush)
	{
		RenderTarget->FillRectangle(rect, brush);
		SafeRelease(&brush);
	}
}
void AssetsManager::DrawRectangle(D2D1_RECT_F rect, D2D1::ColorF color, float width)
{
	ID2D1SolidColorBrush* brush = nullptr;
	RenderTarget->CreateSolidColorBrush(color, &brush);
	if (brush)
	{
		RenderTarget->DrawRectangle(rect, brush, width);
		SafeRelease(&brush);
	}
}
void AssetsManager::DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, D2D1::ColorF color, float width)
{
	ID2D1SolidColorBrush* brush = nullptr;
	RenderTarget->CreateSolidColorBrush(color, &brush);
	if (brush)
	{
		RenderTarget->DrawLine(p1, p2, brush, width);
		SafeRelease(&brush);
	}
}
void AssetsManager::DrawText(wstring text, FontData font, D2D1_RECT_F& layoutRect, D2D1::ColorF color)
{
	IDWriteTextFormat* TextFormat = nullptr;
	DWriteFactory->CreateTextFormat(FONT_FAMILY, FontCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, font.size, L"", &TextFormat);

	if (TextFormat)
	{
		switch (font.horizontalAlignment)
		{
		case HorizontalAlignment::Right:	TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING); break;
		case HorizontalAlignment::Center:	TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); break;
		case HorizontalAlignment::Left:		TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING); break;
		}
		switch (font.verticalAlignment)
		{
		case VerticalAlignment::Top:	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR); break;
		case VerticalAlignment::Center:	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); break;
		case VerticalAlignment::Bottom:	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR); break;
		}

		ID2D1SolidColorBrush* brush = nullptr;
		RenderTarget->CreateSolidColorBrush(color, &brush);
		if (brush)
		{
			RenderTarget->DrawText(text.c_str(), (UINT32)text.length(), TextFormat, layoutRect, brush);
			SafeRelease(&brush);
		}
		SafeRelease(&TextFormat);
	}
}
void AssetsManager::DrawBitmap(ID2D1Bitmap* bitmap, D2D1_RECT_F dstRect, D2D1_RECT_F srcRect, bool reversed, float opacity, float alpha)
{
	if (reversed)
	{
		// to draw reversed we need to reverse the X-axis, so we change the sign of these points
		dstRect.left = -dstRect.left;
		dstRect.right = -dstRect.right;

		// set to draw reversed
		D2D1_MATRIX_3X2_F transformMatrix = D2D1::Matrix3x2F::Identity();
		transformMatrix._11 = -1;
		RenderTarget->SetTransform(transformMatrix);
	}
	else if (alpha != 0)
	{
		RenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(alpha,
			D2D1::Point2F(
				(dstRect.right + dstRect.left) / 2,
				(dstRect.bottom + dstRect.top) / 2
			)));
	}

	RenderTarget->DrawBitmap(bitmap, dstRect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);

	if (reversed || alpha)
	{
		// back to normal
		RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
}

ID2D1Bitmap* AssetsManager::LoadBitapImage(wstring imageFile)
#define TEST_HRESULT_FUNCTION(funcCall) if (FAILED(funcCall)) goto LoadBitapImage_end; // test function and if it faild go to end
{
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* source = nullptr;
	IWICFormatConverter* converter = nullptr;
	ID2D1Bitmap* toReturn = nullptr;

	TEST_HRESULT_FUNCTION(WICImagingFactory->CreateDecoderFromFilename(imageFile.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder));
	TEST_HRESULT_FUNCTION(decoder->GetFrame(0, &source));
	TEST_HRESULT_FUNCTION(WICImagingFactory->CreateFormatConverter(&converter));
	TEST_HRESULT_FUNCTION(converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeMedianCut));
	TEST_HRESULT_FUNCTION(RenderTarget->CreateBitmapFromWicBitmap(converter, &toReturn));

LoadBitapImage_end:
	SafeRelease(&decoder);
	SafeRelease(&source);
	SafeRelease(&converter);

	return toReturn;
}
ID2D1Bitmap* AssetsManager::LoadBitapImage(string imageFile)
{
	return LoadBitapImage(wstring(imageFile.begin(), imageFile.end()));
}
