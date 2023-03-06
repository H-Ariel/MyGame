#include "SelectEnemy_Window.h"
#include "UIButtonElement.h"


class EnemyButton : public UIButtonElement
{
public:
	EnemyButton(SelectEnemy_Window* window, const D2D1_SIZE_U& wndSize, const string& name)
		: UIButtonElement(window), _name(name), wndSize(wndSize), window(window)
	{
		OnClick = function<void(MouseButtons)>([&](MouseButtons mb) {
			if (mb == MouseButtons::Left)
			{
				GlobalVariables::EnemyName = _name;
				GlobalVariables::SelectedType = Type::ENEMY;
			}
			else if (mb == MouseButtons::Right)
				MessageBoxA(NULL, ("selected enemy name = " + _name).c_str(), "enemy name", MB_OK);
			});

		D2D1_RECT_F enImgData = window->assetsManager->EnemiesPositions.at(_name);
		size = { enImgData.right - enImgData.left, enImgData.bottom - enImgData.top };
	}

	void Draw() override
	{
		D2D1_RECT_F dst = D2D1::RectF(position.x, position.y, position.x + size.width, position.y + size.height);

		if (dst.right <= wndSize.width && dst.bottom <= wndSize.height)
		{
			window->DrawBitmap(window->assetsManager->EnemiesImageSet, dst, window->assetsManager->EnemiesPositions.at(_name));
		}

		if (GlobalVariables::EnemyName == _name && GlobalVariables::SelectedType == Type::ENEMY)
		{
			window->DrawRectangle(dst, D2D1::ColorF::Red, 5); // mark the selected tile
		}
	}


private:
	const string _name;
	const D2D1_SIZE_U& wndSize;

	SelectEnemy_Window* window;
};


SelectEnemy_Window::SelectEnemy_Window(WindowBase* parent)
	: GameDesignerWindowBase(WS_CHILDWINDOW, parent), wndSize({ 0,0 })
{
	backgroundColor = 0x67aeda;

	for (string en : ENEMIES_LIST)
	{
		if (en == "spider" || en == "snakeRed" || en == "snakeGreen") en += "_1";
		buttons.push_back(DBG_NEW EnemyButton(this, wndSize, en));
	}

	float y = 5;
	for (EnemyButton* t : buttons)
	{
		t->position = { 5, y };
		AddUIElement(t);
		y += 5 + t->size.height;
	}


	RECT wndRc = {};
	wndRc.top = 0; wndRc.bottom = 700;
	wndRc.left = 280; wndRc.right = 360;
	SetWindowRect(wndRc);
	wndSize = GetWindowSize();
}

SelectEnemy_Window::~SelectEnemy_Window() { Close(); }

void SelectEnemy_Window::OnMouseWheel(int delta)
{
	if (delta > 0)
	{
		if (buttons[0]->position.y <= 0)
			for (auto* t : buttons)
				t->position.y += 100;
	}
	else if (delta < 0)
	{
		auto& lastBtn = buttons[buttons.size() - 1];
		if (lastBtn->position.y + lastBtn->size.height * 2 > wndSize.height)
			for (auto* t : buttons)
				t->position.y -= 100;
	}
}
void SelectEnemy_Window::Close()
{
	for (auto* b : buttons)
		delete b;
	buttons.clear();

	GameDesignerWindowBase::Close();
}
