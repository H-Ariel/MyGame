#pragma once

#include "SelectTile_Window.h"
#include "LevelDisplay_Window.h"
#include "SelectEnemy_Window.h"


class GameDesignerApp : public GameDesignerWindowBase
{
public:
	GameDesignerApp();
	~GameDesignerApp();

	void Logic(float elapsedTime) override;
	void Draw() const override;

	void Command(WORD command) override;

	void OnKeyUp(int key) override;
	void OnKeyDown(int key) override;

	void OnResize() override;

private:
	SelectTile_Window w_st;
	SelectEnemy_Window w_se;
	LevelDisplay_Window w_ld;

	HMENU hMenubar, hFileMenu;
};
