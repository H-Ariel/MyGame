#include "SelectLevelScreenEngine.h"
#include "AssetsManager.h"


#define LEVEL_BUTTON_SIZE	 100
#define LEVEL_BUTTON_PADDING 20


static int choosenLevel = -1;


class ChooseLevelButton : public UIButtonElement
{
public:
	ChooseLevelButton(int lvlId)
		: _lvlId(lvlId),
		UIButtonElement(
			function<void(MouseButtons)>([&](MouseButtons mb) { choosenLevel = _lvlId; }),
			to_wstring(lvlId), FontData(50),
			D2D1::SizeF(LEVEL_BUTTON_SIZE, LEVEL_BUTTON_SIZE), { 0,0 },
			D2D1::ColorF::Black, D2D1::ColorF(0xd0d0d0))
	{
	}

private:
	int _lvlId;
};


SelectLevelScreenEngine::SelectLevelScreenEngine()
	: title(L"Choose the level you want to play", FontData(40), D2D1::SizeF(AssetsManager::GetWindowSize().width - 20.0f, 50), {10,10}, D2D1::ColorF::Black, D2D1::ColorF::LightPink)
{
	backgroundColor = D2D1::ColorF(D2D1::ColorF::ForestGreen);

	const int numOfLevels = SettingManager::GetNumOfLevels();
	for (int i = 1; i <= numOfLevels; i++)
		btns.push_back(DBG_NEW ChooseLevelButton(i));

	for (UIButtonElement* b : btns)
		AddUIElement(b);
	AddUIElement(&title);
}

SelectLevelScreenEngine::~SelectLevelScreenEngine()
{
	for (UIButtonElement* b : btns)
	{
		RemoveUIElement(b);
		delete b;
	}
}

void SelectLevelScreenEngine::Logic(float elapsedTime)
{
	EngineBase::Logic(elapsedTime);

	StopEngine = (choosenLevel != -1);
}

void SelectLevelScreenEngine::OnMouseWheel(int delta)
{
	if (delta > 0)
	{
		if (btns[0]->position.y <= 0)
		{
			for (UIButtonElement* b : btns)
				b->position.y += LEVEL_BUTTON_SIZE;
		}
	}
	else if (delta < 0)
	{
		D2D1_SIZE_U wndSize = AssetsManager::GetWindowSize();
		if (btns[btns.size() - 1]->position.y + LEVEL_BUTTON_SIZE + LEVEL_BUTTON_PADDING > wndSize.height)
		{
			for (UIButtonElement* b : btns)
				b->position.y -= LEVEL_BUTTON_SIZE;
		}
	}
}

void SelectLevelScreenEngine::OnResize()
{
	EngineBase::OnResize();

	D2D1_SIZE_U wndSize = AssetsManager::GetWindowSize();

	title.size = { wndSize.width - 20.0f, 50 };

	// set buttons position
	const int MaxButtonsInLine = (int)((wndSize.width - LEVEL_BUTTON_PADDING) / (LEVEL_BUTTON_SIZE + LEVEL_BUTTON_PADDING));
	float x = LEVEL_BUTTON_PADDING, y = 100;
	for (int i = 0; i < btns.size(); i++)
	{
		btns[i]->position = { x, y };

		x += LEVEL_BUTTON_SIZE + LEVEL_BUTTON_PADDING;
		if ((i + 1) % MaxButtonsInLine == 0)
		{
			x = LEVEL_BUTTON_PADDING;
			y += LEVEL_BUTTON_SIZE + LEVEL_BUTTON_PADDING;
		}
	}
}

int SelectLevelScreenEngine::GetLevelNumber()
{
	int retVal = choosenLevel;
	
	// do not "recycle" this values
	choosenLevel = -1;
	StopEngine = false;

	return retVal;
}
