#include "LevelDisplay_Window.h"


#define SPEED (30*TILE_SIZE) // The screen move speed


LevelDisplay_Window::LevelDisplay_Window(WindowBase* parent)
	: GameDesignerWindowBase(WS_CHILDWINDOW, parent), levelMap(this, backgroundColor)
{
	upPressed = false;
	downPressed = false;
	rightPressed = false;
	leftPressed = false;
	mouseButtonLeftPressed = false;
	ctrlPressed = false;
	backgroundColor = 0x67aeda;

	RECT wndRc = {};
	wndRc.top = 0; wndRc.bottom = 700;
	wndRc.left = 380; wndRc.right = 1250;
	SetWindowRect(wndRc);

	AddUIElement(&levelMap);

	TILE_SIZE = 50;
}
LevelDisplay_Window::~LevelDisplay_Window() { Close(); }

void LevelDisplay_Window::Logic(float elapsedTime)
{
	GameDesignerWindowBase::Logic(elapsedTime);

	if (mouseButtonLeftPressed) levelMap.ChangeTile(mousePosition);

	const D2D1_SIZE_U wndSize = GetWindowSize();

	if (leftPressed) levelMap.position.x -= elapsedTime * SPEED;
	if (rightPressed) levelMap.position.x += elapsedTime * SPEED;
	if (upPressed) levelMap.position.y -= elapsedTime * SPEED;
	if (downPressed) levelMap.position.y += elapsedTime * SPEED;

	if (levelMap.position.x < 0) levelMap.position.x = 0;
	if (levelMap.position.y < 0) levelMap.position.y = 0;
	float tmp = LEVEL_WIDTH * TILE_SIZE_F - wndSize.width;
	if (levelMap.position.x > tmp) levelMap.position.x = tmp;
	tmp = LEVEL_HEIGHT * TILE_SIZE_F - wndSize.height;
	if (levelMap.position.y > tmp) levelMap.position.y = tmp;
}

void LevelDisplay_Window::OnKeyUp(int key)
{
	switch (key)
	{
	case 'A': case VK_LEFT: leftPressed = false; break;
	case 'D': case VK_RIGHT: rightPressed = false; break;
	case 'W': case VK_UP: upPressed = false; break;
	case 'S': case VK_DOWN: downPressed = false; break;
	case VK_CONTROL: ctrlPressed = false; break;
	}

	if (ctrlPressed)
	{
		switch (key)
		{
		case 'S': Save(); break;
		case 'A': SaveAs(); break;
		case 'O': Open(); break;
		case 'N': New(); break;
		case VK_ADD: if (TILE_SIZE < 120) TILE_SIZE += 10; break;
		case VK_SUBTRACT: if (TILE_SIZE > 20) TILE_SIZE -= 10; break;
		case VK_DIVIDE: TILE_SIZE = SettingManager::DefaultTileSize; break;
		}
	}
}
void LevelDisplay_Window::OnKeyDown(int key)
{
	switch (key)
	{
	case 'A': case VK_LEFT: leftPressed = true; break;
	case 'D': case VK_RIGHT: rightPressed = true; break;
	case 'W': case VK_UP: upPressed = true; break;
	case 'S': if (!ctrlPressed) downPressed = true; break;
	case VK_DOWN: downPressed = true; break;
	case VK_CONTROL: ctrlPressed = true; break;
	case 'P': levelMap.SetPlayerInitialPosition(mousePosition); break;
	}
}
void LevelDisplay_Window::OnMouseButtonDown(MouseButtons btn)
{
	if (btn == MouseButtons::Left)
	{
		mouseButtonLeftPressed = true;
	}
	else if (btn == MouseButtons::Right)
	{
		D2D1_POINT_2U pos = levelMap.GetTilePosition(mousePosition);
		wchar_t str[20];
		swprintf_s(str, L"X = %d\nY = %d", pos.x, pos.y);
		MessageBox(NULL, str, L"tile position", MB_OK);
	}
}
void LevelDisplay_Window::OnMouseButtonUp(MouseButtons btn)
{
	if (btn == MouseButtons::Left)
		mouseButtonLeftPressed = false;
}

void LevelDisplay_Window::Save()
{
	if (filename.empty()) SaveAs();
	else levelMap.Save(filename);
}
void LevelDisplay_Window::SaveAs()
{
	OPENFILENAME ofn = { };

	wchar_t tmp[260] = {}; wcscpy_s(tmp, filename.c_str());
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = tmp;
	ofn.nMaxFile = sizeof(tmp);
	ofn.lpstrFilter = L"JSON\0*.JSON\0All File\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		filename = ofn.lpstrFile;
		if (filename.find('.') == string::npos) filename += L".json";
		levelMap.Save(filename);
	}
}
void LevelDisplay_Window::Open()
{
	OPENFILENAME ofn = { };
	wchar_t tmp[260] = {}; wcscpy_s(tmp, filename.c_str());
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = tmp;
	ofn.nMaxFile = sizeof(tmp);
	ofn.lpstrFilter = L"JSON\0*.JSON\0All File\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		filename = ofn.lpstrFile;
		levelMap.Load(filename);
	}
}

void LevelDisplay_Window::New()
{
	filename.clear();
	levelMap.Clear();
	backgroundColor = 0x67aeda;
}

void LevelDisplay_Window::SetBackgroundColor()
{
	CHOOSECOLOR cc = {};
	static COLORREF acrCustClr[16] = { 0xdaae67 }; // array of custom colors. we set it with the default background color

	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = GetHwnd();
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = 0xdaae67; // set as default background color
	cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ANYCOLOR;

	if (ChooseColor(&cc) == TRUE)
	{
		backgroundColor = D2D1::ColorF(
			GetRValue(cc.rgbResult) / 255.0f,
			GetGValue(cc.rgbResult) / 255.0f,
			GetBValue(cc.rgbResult) / 255.0f
		);
	}
}
