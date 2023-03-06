#pragma once

#include "GameDesignerWindowBase.h"
#include "LevelMap.h"


class LevelDisplay_Window : public GameDesignerWindowBase
{
public:
	LevelDisplay_Window(WindowBase* parent);
	~LevelDisplay_Window();

	void Logic(float elapsedTime) override;

	void OnKeyUp(int key) override;
	void OnKeyDown(int key) override;
	void OnMouseButtonDown(MouseButtons btn) override;
	void OnMouseButtonUp(MouseButtons btn) override;

	void Save();
	void SaveAs();
	void Open();
	void New();

	void SetBackgroundColor();


private:
	LevelMap levelMap;
	wstring filename;
	bool upPressed, downPressed, rightPressed, leftPressed;
	bool mouseButtonLeftPressed;
	bool ctrlPressed;

	friend class LevelMap;
	friend class GameDesignerApp;
};
