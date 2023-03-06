#include "UIButtonElement.h"
#include "AssetsManager.h"


#define MY_BUTTON_CLASS_NAME L"my_button"


UIButtonElement::UIButtonElement()
{
	CreateWnd();
}

UIButtonElement::UIButtonElement(function<void(MouseButtons)> OnClick,
	wstring text, FontData font, D2D1_SIZE_F size, D2D1_POINT_2F position,
	D2D1::ColorF forgroundColor, D2D1::ColorF backgroundColor)
	: UITextElement(text, font, size, position, forgroundColor, backgroundColor), OnClick(OnClick)
{
	CreateWnd();
}

void UIButtonElement::Logic(float elapsedTime)
{
	// ensure that the drawn button is also the virtual button
	SetWindowPos(btnWnd, 0, (int)position.x, (int)position.y, (int)size.width, (int)size.height, SWP_NOZORDER);
}

void UIButtonElement::CreateWnd()
{
	RegisterMyButtonClass();
	THROW_IF_NULL(btnWnd = CreateWindow(MY_BUTTON_CLASS_NAME, L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, AssetsManager::GetHwnd(), nullptr, HINST_THISCOMPONENT, this));
	ShowWindow(btnWnd, SW_NORMAL);
}

void UIButtonElement::RegisterMyButtonClass()
{
	static bool DoesClassRegistered = false;
	if (DoesClassRegistered) return;

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = UIButtonElement::UIButtonElement_WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
	wcex.lpszClassName = MY_BUTTON_CLASS_NAME;

	RegisterClassEx(&wcex);

	DoesClassRegistered = true;
}

LRESULT UIButtonElement::UIButtonElement_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const UIButtonElement* btn = (UIButtonElement*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CREATE:			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams)); break;
	case WM_DESTROY:		PostQuitMessage(0); break;
	case WM_DISPLAYCHANGE:	InvalidateRect(hwnd, nullptr, FALSE); break;
	case WM_LBUTTONUP:		if (btn->btnWnd == hwnd) btn->OnClick(MouseButtons::Left);	break;
	case WM_MBUTTONUP:		if (btn->btnWnd == hwnd) btn->OnClick(MouseButtons::Middle);	break;
	case WM_RBUTTONUP:		if (btn->btnWnd == hwnd) btn->OnClick(MouseButtons::Right);	break;
	default:				return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
