#pragma once

#include "EngineBase.h"
#include "UIButtonElement.h"


class SelectLevelScreenEngine : public EngineBase
{
public:
	SelectLevelScreenEngine();
	~SelectLevelScreenEngine() override;

	void Logic(float elapsedTime) override;

	void OnMouseWheel(int delta) override;
	void OnResize() override;

	int GetLevelNumber();

private:
	UITextElement title;
	vector<UIButtonElement*> btns;
};
