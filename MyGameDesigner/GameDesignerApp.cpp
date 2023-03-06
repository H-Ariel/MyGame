#include "GameDesignerApp.h"


#define IDM_FILE_NEW		101
#define IDM_FILE_OPEN		102
#define IDM_FILE_SAVE		103
#define IDM_FILE_SAVE_AS	104
#define IDM_CHANGE_BG_COLOR	105


GameDesignerApp::GameDesignerApp()
	: w_st(this), w_se(this), w_ld(this)
{
	backgroundColor = D2D1::ColorF::DarkGray;
	OnResize();

	hMenubar = CreateMenu();
	hFileMenu = CreateMenu();

	AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, L"&New");
	AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
	AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, L"&Save");
	AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE_AS, L"&Save As ...");

	AppendMenu(hMenubar, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
	AppendMenu(hMenubar, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenubar, MF_STRING, IDM_CHANGE_BG_COLOR, L"&Change Background Color");

	SetMenu(GetHwnd(), hMenubar);
}
GameDesignerApp::~GameDesignerApp()
{
	DestroyMenu(hFileMenu);
	DestroyMenu(hMenubar);
}

void GameDesignerApp::Logic(float elapsedTime)
{
	if (RunWindow)
	{
		SetTitle(L"Game Designer App Window - " + w_ld.filename);

		w_st.Logic(elapsedTime);
		w_se.Logic(elapsedTime);
		w_ld.Logic(elapsedTime);
	}
}
void GameDesignerApp::Draw() const
{
	if (RunWindow)
	{
		w_st.Draw();
		w_se.Draw();
		w_ld.Draw();
	}
}

void GameDesignerApp::Command(WORD command)
{
	switch (command)
	{
	case IDM_FILE_NEW:		w_ld.New(); break;
	case IDM_FILE_OPEN:		w_ld.Open(); break;
	case IDM_FILE_SAVE:		w_ld.Save(); break;
	case IDM_FILE_SAVE_AS:	w_ld.SaveAs(); break;
	
	case IDM_CHANGE_BG_COLOR:	w_ld.SetBackgroundColor(); break;

	default:
		break;
	}
}

void GameDesignerApp::OnKeyUp(int key)
{
	w_ld.OnKeyUp(key);
}
void GameDesignerApp::OnKeyDown(int key)
{
	w_ld.OnKeyDown(key);
}

void GameDesignerApp::OnResize()
{
	D2D1_SIZE_U size = this->GetWindowSize();
	RECT rc = {};

	rc = w_st.GetWindowRect();
	rc.top = 0; rc.bottom = size.height;
	rc.left = 0; rc.right = 280;
	w_st.SetWindowRect(rc);

	rc = w_se.GetWindowRect();
	rc.top = 0; rc.bottom = size.height;
	rc.left = 280; rc.right = 360;
	w_se.SetWindowRect(rc);

	rc = w_ld.GetWindowRect();
	rc.top = 0; rc.bottom = size.height;
	rc.left = 380; rc.right = size.width - 10;
	w_ld.SetWindowRect(rc);

	GameDesignerWindowBase::Draw();
}
