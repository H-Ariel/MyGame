#pragma once

#include "EngineBase.h"
#include "SelectLevelScreenEngine.h"
#include "LevelEngine.h"
#include <memory>



class GameMainApp
{
public:
	GameMainApp();
	~GameMainApp();

	void Run();


private:
	void RunEngine();

	void OpenningScreen();
	int SelectLevel();
	void RunLevel(int levelNumber);

	static void RegisterMyWindowClass();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


	EngineBase* pEngine;
	bool RunApp;

	// we keep the following objects because they don't change
	unique_ptr<SelectLevelScreenEngine> selectLevelScreen;
	unique_ptr<LevelEngine> levelEngine;
};
