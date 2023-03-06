#include "GameDesignerWindowBase.h"


bool GameDesignerWindowBase::RunApp = true;


GameDesignerWindowBase::GameDesignerWindowBase(DWORD dwStyle, WindowBase* parent)
	: WindowBase(dwStyle, parent), assetsManager(nullptr)
{
	assetsManager = DBG_NEW AssetsManager(this);
}

void GameDesignerWindowBase::Logic(float elapsedTime)
{
	RunWindow = RunApp;
	WindowBase::Logic(elapsedTime);
	RunWindow = RunApp;
}

void GameDesignerWindowBase::Close()
{
	if (assetsManager)
	{
		delete assetsManager;
		assetsManager = nullptr;
	}

	RunApp = false;

	WindowBase::Close();
}

void GameDesignerWindowBase::DrawTile(D2D1_RECT_F dst, short tileId)
{
	if (tileId != TilesCodes::Empty)
		DrawBitmap(assetsManager->TilesImageSet, dst, assetsManager->TilesPositions[tileId]);
}
