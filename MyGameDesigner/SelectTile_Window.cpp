#include "SelectTile_Window.h"
#include "UIButtonElement.h"


#define BUTTON_PADDING		5
#define BUTTON_SIZE			60 // width and height
#define MAX_BUTTONS_IN_LINE	4


const initializer_list<short> IGNORED_TILES_IDS = {
	4, 6, 8, 19, 21, 23, 25, 29, 30, 31, 32, 43,
	65, 67, 69, 586, 589, 590, 593, 596, 597, 600,
	603, 604, 607, 610, 611, 624, 905, 906, 907,
	908, 909, 910, 911, 912, 913, 914, 915, 916,
	922, 924, 926, 928, 975, 976, 977, 978, 979,
	980, 981, 982, 983, 984, 985, 986, 987, 988,
	989, 990, 991
};


class TileButton : public UIButtonElement
{
public:
	TileButton(SelectTile_Window* window, const D2D1_SIZE_U& wndSize, int tileId, Type type)
		: UIButtonElement(window), _id(tileId), _type(type), wndSize(wndSize), window(window)
	{
		OnClick = function<void(MouseButtons)>([&](MouseButtons mb) {
			if (mb == MouseButtons::Left)
			{
				GlobalVariables::SelectedId = _id;
				GlobalVariables::SelectedType = _type;
			}
			else if (mb == MouseButtons::Right)
				MessageBoxA(NULL, ("selected tile id = " + to_string(_id)).c_str(), "tile id", MB_OK);
			});

		size = { BUTTON_SIZE,BUTTON_SIZE };
	}

	void Draw() override
	{
		D2D1_RECT_F dst = D2D1::RectF(position.x, position.y, position.x + BUTTON_SIZE, position.y + BUTTON_SIZE);

		if (dst.right <= wndSize.width && dst.bottom <= wndSize.height)
		{
			if (_id != TilesCodes::Empty) // if not empty, draw image
			{
				if (_type == Type::TILE)
				{
					window->DrawTile(dst, _id);
				}
			}

			if (GlobalVariables::SelectedId == _id && GlobalVariables::SelectedType == _type)
			{
				window->DrawRectangle(dst, D2D1::ColorF::Red, 5); // mark the selected tile
			}
		}
	}


private:
	const int _id;
	const Type _type;
	const D2D1_SIZE_U& wndSize;

	SelectTile_Window* window;
};


SelectTile_Window::SelectTile_Window(WindowBase* parent)
	: GameDesignerWindowBase(WS_CHILDWINDOW, parent), wndSize({ 0,0 })
{
	backgroundColor = 0x67aeda;

	int i = 0;
	
	for (auto& j : assetsManager->TilesPositions)
	{
		if (i % 40 == 0)
			buttons.push_back(DBG_NEW TileButton(this, wndSize, Empty, Type::NONE)); // put empty button
		if (!FIND_IN_ARRAY(IGNORED_TILES_IDS, i))
			buttons.push_back(DBG_NEW TileButton(this, wndSize, i, Type::TILE));
		i += 1;
	}

	i = 1;
	float x = BUTTON_PADDING, y = BUTTON_PADDING;
	for (TileButton* t : buttons)
	{
		t->position = { x, y };

		if (i % MAX_BUTTONS_IN_LINE == 0)
		{
			x = BUTTON_PADDING;
			y += BUTTON_SIZE + BUTTON_PADDING;
		}
		else
		{
			x += BUTTON_SIZE + BUTTON_PADDING;
		}
		i++;

		AddUIElement(t);
	}


	RECT wndRc = {};
	wndRc.top = 0; wndRc.bottom = 700;
	wndRc.left = 0; wndRc.right = 280;
	SetWindowRect(wndRc);
	wndSize = GetWindowSize();
}
SelectTile_Window::~SelectTile_Window() { Close(); }
void SelectTile_Window::OnMouseWheel(int delta)
{
	if (delta > 0)
	{
		if (buttons[0]->position.y <= 0)
			for (TileButton* t : buttons)
				t->position.y += 550;
	}
	else if (delta < 0)
	{
		if (buttons[buttons.size() - 1]->position.y + BUTTON_SIZE * 2 > wndSize.height)
			for (TileButton* t : buttons)
				t->position.y -= 550;
	}
}
void SelectTile_Window::Close()
{
	for (TileButton* b : buttons)
		delete b;
	buttons.clear();

	GameDesignerWindowBase::Close();
}
