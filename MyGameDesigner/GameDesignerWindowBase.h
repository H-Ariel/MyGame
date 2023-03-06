#pragma once

#include "WindowBase.h"
#include "AssetsManager.h"


class GameDesignerWindowBase : public WindowBase
{
public:
	GameDesignerWindowBase(DWORD dwStyle = WS_OVERLAPPEDWINDOW, WindowBase* parent = nullptr);

	void Logic(float elapsedTime) override;
	virtual void Close() override;

	void DrawTile(D2D1_RECT_F dst, short tileId);

	static bool RunApp; // the main app, no just this window

protected:
	AssetsManager* assetsManager;
};
