#pragma once

#include "UIButtonElement.h"
#include "EngineBase.h"


class OpeningScreenEngine : public EngineBase
{
public:
	enum class State
	{
		StartGame,
		CloseApp,
		Run
	};


	OpeningScreenEngine();
	void OnResize() override;

	State state;

private:
	UITextElement tTitle;
	UIButtonElement bStartGame, bCredits, bExitApp;
};
