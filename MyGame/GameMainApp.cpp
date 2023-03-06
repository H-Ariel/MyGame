#include "GameMainApp.h"
#include "OpeningScreenEngine.h"
#include <chrono>


#define WINDOW_CLASS_NAME L"MyGameWindow"

// The maximum iteration time.
// `elapsedTime` used in `EngineBase::Logic` and its overrides.
// safety so the game doesn't go wild.
#define MAX_ITER_TIME 0.02f


GameMainApp::GameMainApp()
	: pEngine(nullptr), RunApp(true)
{
	SettingManager::ReadSetting();

	// register window class
	RegisterMyWindowClass();

	AssetsManager::Initialize(WINDOW_CLASS_NAME, this);

	selectLevelScreen = unique_ptr<SelectLevelScreenEngine>(DBG_NEW SelectLevelScreenEngine());
	levelEngine = unique_ptr<LevelEngine>(DBG_NEW LevelEngine());
}
GameMainApp::~GameMainApp()
{
	AssetsManager::Finalize();
	RunApp = false;
}

void GameMainApp::Run()
{
//	{ RunLevel(100); return; }

	OpenningScreen();

	int lvl;
	while (RunApp)
	{
#ifdef CATCH_EXCEPTIONS
		try
		{
#endif
			lvl = SelectLevel();
			if (lvl == -1)
				RunApp = false;
			if (RunApp)
				RunLevel(lvl);
#ifdef CATCH_EXCEPTIONS
		}
		catch (exception& ex)
		{
			string msg = ex.what();
			LogFile::WriteLog(msg, LogFile::Error);
			MessageBox(NULL, wstring(msg.begin(), msg.end()).c_str(), L"Error", MB_OK | MB_ICONERROR);
			pEngine = nullptr;
		}
#endif
	}
}
void GameMainApp::RunEngine()
{
	MSG msg;
	chrono::steady_clock::time_point begin, end;
	float elapsedTime; // in seconds

#ifdef _DEBUG
	// for FPS
	float framesTime = 0;
	int frames = 0;
#endif

	RunApp = true;
	while (RunApp && pEngine && !pEngine->StopEngine)
	{
		end = chrono::steady_clock::now();
		elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - begin).count() / 1000.0f; // milliseconds to seconds
		begin = end;

#ifdef _DEBUG
		// display FPS
		framesTime += elapsedTime;
		frames++;
		if (framesTime > 1)
		{
			WCHAR fpsText[14] = {};
			swprintf(fpsText, 14, L"Game: %d FPS", frames);
			AssetsManager::SetTitle(fpsText);
			frames = 0;
			framesTime = 0;
		}
#endif

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				RunApp = false;
			}
		}

		if (pEngine)
		{
			pEngine->Draw();
			if (elapsedTime > MAX_ITER_TIME)
				elapsedTime = MAX_ITER_TIME;
			pEngine->Logic(elapsedTime);
		}
	}
}

void GameMainApp::OpenningScreen()
{
	OpeningScreenEngine OpeningScreen;

	AssetsManager::SetTitle(L"Game");
	pEngine = &OpeningScreen;
	RunEngine();

	if (OpeningScreen.state == OpeningScreenEngine::State::CloseApp)
		RunApp = false;

	pEngine = nullptr;
}
int GameMainApp::SelectLevel()
{
	AssetsManager::SetTitle(L"Game - Select Level");
	selectLevelScreen->OnResize();
	pEngine = selectLevelScreen.get();
	RunEngine();
	int lvlNum = selectLevelScreen->GetLevelNumber();
	pEngine = nullptr;
	return lvlNum;
}
void GameMainApp::RunLevel(int levelNumber)
{
	{
		wchar_t title[17];
		swprintf_s(title, L"Game - Level %d", levelNumber);
		AssetsManager::SetTitle(title);
	}

	levelEngine->LoadLevel(levelNumber);
	pEngine = levelEngine.get();
	RunEngine();

	switch (levelEngine->GetState())
	{
	case LevelEngine::State::Lose:
		MessageBox(nullptr, L"Sorry, you lose", L"Game Over", MB_OK);
		break;

	case LevelEngine::State::Win:
		MessageBox(nullptr, L"Well done!\nYou finished the level!", L"Congratulations", MB_OK);
		break;

	case LevelEngine::State::Run:
		// TODO: check if level has loaded and them: ask if sure / save game state
		break;
	}

	levelEngine->Clear();
	pEngine = nullptr;
}

void GameMainApp::RegisterMyWindowClass()
{
//	static bool DoesClassRegistered = false;
//	if (DoesClassRegistered) return;

	WNDCLASSEXW wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GameMainApp::WndProc;
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = WINDOW_CLASS_NAME;

	RegisterClassEx(&wcex);

//	DoesClassRegistered = true;
}
LRESULT CALLBACK GameMainApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GameMainApp* app = (GameMainApp*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CREATE:			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams)); break;
	case WM_GETMINMAXINFO:	((LPMINMAXINFO)lParam)->ptMinTrackSize = { 600,400 }; break;
	case WM_DISPLAYCHANGE:	InvalidateRect(hwnd, nullptr, FALSE); break;
	case WM_CLOSE:			PostQuitMessage(0); break;
	case WM_MOUSEMOVE:		if (app && app->pEngine) app->pEngine->mousePosition = { LOWORD(lParam), HIWORD(lParam) }; break;
	case WM_KEYDOWN:		if (app && app->pEngine) app->pEngine->OnKeyDown((int)wParam); break;
	case WM_KEYUP:			if (app && app->pEngine) app->pEngine->OnKeyUp((int)wParam); break;
	case WM_LBUTTONUP:		if (app && app->pEngine) app->pEngine->OnMouseButtonUp(MouseButtons::Left); break;
	case WM_LBUTTONDOWN:	if (app && app->pEngine) app->pEngine->OnMouseButtonDown(MouseButtons::Left); break;
	case WM_MBUTTONUP:		if (app && app->pEngine) app->pEngine->OnMouseButtonUp(MouseButtons::Middle); break;
	case WM_MBUTTONDOWN:	if (app && app->pEngine) app->pEngine->OnMouseButtonDown(MouseButtons::Middle); break;
	case WM_RBUTTONUP:		if (app && app->pEngine) app->pEngine->OnMouseButtonUp(MouseButtons::Right); break;
	case WM_RBUTTONDOWN:	if (app && app->pEngine) app->pEngine->OnMouseButtonDown(MouseButtons::Right); break;
	case WM_MOUSEWHEEL:		if (app && app->pEngine) app->pEngine->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam)); break;
	case WM_SIZE:
		if (app && app->pEngine)
		{
			AssetsManager::ResizeRenderTarget(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
			app->pEngine->OnResize();
		}
		break;
	default:				return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
