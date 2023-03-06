#include "OpeningScreenEngine.h"
#include "AssetsManager.h"


#define CREDITS_STRING \
	L"Credits:\n" \
	"\n" \
	"Kenney about assets\n" \
	"BuildSucceeded about game base\n" \
	"\n" \
	"Links:\n" \
	"https://kenney.nl/assets/platformer-art-deluxe\n" \
	"https://github.com/BuildSucceeded/2D-Platformer"


void ShowCredits()
{
	MessageBox(NULL, CREDITS_STRING, L"Credits", MB_OK | MB_ICONINFORMATION);
}


OpeningScreenEngine::OpeningScreenEngine() :
	state(State::Run),
	tTitle(L"Welcome to My Game", FontData(50), D2D1::SizeF(AssetsManager::GetWindowSize().width - 20.0f, 70), { 10,20 }, D2D1::ColorF::White, D2D1::ColorF::Green),
	bStartGame([&](MouseButtons) { StopEngine = true; state = State::StartGame; }, L"Start Game", FontData(35), D2D1::SizeF(400, 60), D2D1::Point2F((AssetsManager::GetWindowSize().width - 400) / 2.0f, 130), D2D1::ColorF::Black, D2D1::ColorF::Gray),
	bCredits([&](MouseButtons) { ShowCredits(); }, L"Credits", FontData(35), D2D1::SizeF(400, 60), D2D1::Point2F((AssetsManager::GetWindowSize().width - 400) / 2.0f, 230), D2D1::ColorF::Black, D2D1::ColorF::Gray),
	bExitApp([&](MouseButtons) { StopEngine = true; state = State::CloseApp; }, L"Exit", FontData(35), D2D1::SizeF(400, 60), D2D1::Point2F((AssetsManager::GetWindowSize().width - 400) / 2.0f, 330), D2D1::ColorF::Black, D2D1::ColorF::Gray)
{
	backgroundColor = D2D1::ColorF::CadetBlue;

	AddUIElement(&tTitle);
	AddUIElement(&bStartGame);
	AddUIElement(&bCredits);
	AddUIElement(&bExitApp);
}

void OpeningScreenEngine::OnResize()
{
	EngineBase::OnResize();

	D2D1_SIZE_U wndSize = AssetsManager::GetWindowSize();
	tTitle.size = { wndSize.width - 20.0f,70 };
	bStartGame.position = { (wndSize.width - 400) / 2.0f, 130 };
	bCredits.position = { (wndSize.width - 400) / 2.0f, 230 };
	bExitApp.position = { (wndSize.width - 400) / 2.0f, 330 };
}
