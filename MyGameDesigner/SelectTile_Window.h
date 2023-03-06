#pragma once

#include "GameDesignerWindowBase.h"
#include "AssetsManager.h"


class TileButton;


class SelectTile_Window : public GameDesignerWindowBase
{
public:
	SelectTile_Window(WindowBase* parent);
	~SelectTile_Window();
	void Close() override;
	void OnMouseWheel(int delta) override;

private:
	vector<TileButton*> buttons;
	D2D1_SIZE_U wndSize;

	friend class TileButton;
};
