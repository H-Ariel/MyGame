#include "WindowBase.h"
#include "UIElementBase.h"
#include <chrono>


// The maximum iteration time.
// `elapsedTime` used in `WindowBase::Logic` and its overrides.
// safety so the game doesn't go wild.
#define MAX_ITER_TIME 0.02f

#define WINDOW_CLASS_NAME L"MyGameDesignerWindow"


WindowBase::WindowBase(DWORD dwStyle, WindowBase* parent)
	: hwnd(nullptr), RunWindow(true), backgroundColor(0),
	RenderTarget(nullptr), D2DFactory(nullptr), DWriteFactory(nullptr), WICImagingFactory(nullptr)
{
	// register window class
	RegisterMyWindowClass();

	// create window
	hwnd = CreateWindow(WINDOW_CLASS_NAME, L"", dwStyle, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, parent ? parent->hwnd : nullptr, nullptr, HINST_THISCOMPONENT, this);
	THROW_IF_NULL(hwnd);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	RECT rc = GetWindowRect();
	windowSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// Initializes Direct2D
	THROW_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory));
	THROW_IF_FAILED(D2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, GetWindowSize(), D2D1_PRESENT_OPTIONS_NONE), &RenderTarget));
	THROW_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(DWriteFactory), (IUnknown**)(&DWriteFactory)));
	THROW_IF_FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(&WICImagingFactory)));
}
WindowBase::~WindowBase()
{
	Close();
}

void WindowBase::Run()
{
	MSG msg;
	chrono::steady_clock::time_point begin, end;
	float elapsedTime; // in seconds

	RunWindow = true;
	while (RunWindow && hwnd != nullptr)
	{
		end = chrono::steady_clock::now();
		elapsedTime = chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0F;
		begin = end;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT && msg.hwnd == this->hwnd)
			{
				RunWindow = false;
			}
		}

		Draw();
		if (elapsedTime > MAX_ITER_TIME)
			elapsedTime = MAX_ITER_TIME;
		Logic(elapsedTime);
	}
}
void WindowBase::Close()
{
	if (hwnd)
	{
		if (RenderTarget) RenderTarget->Release();
		if (D2DFactory) D2DFactory->Release();
		if (DWriteFactory) DWriteFactory->Release();
		if (WICImagingFactory) WICImagingFactory->Release();
		DestroyWindow(hwnd);
		hwnd = nullptr;
		RunWindow = false;
	}
}

void WindowBase::Command(WORD command)
{
}

D2D1_SIZE_U WindowBase::GetWindowSize()
{
	return windowSize;
}
RECT WindowBase::GetWindowRect()
{
	RECT rc = {};
	THROW_IF(::GetWindowRect(hwnd, &rc) == 0);
	return rc;
}
void WindowBase::SetWindowRect(RECT wndRect)
{
	if (::SetWindowPos(hwnd, NULL, wndRect.left, wndRect.top, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, SWP_NOZORDER))
	{
		RECT rc = GetWindowRect();
		windowSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	}
}
void WindowBase::SetTitle(wstring title)
{
	SetWindowText(hwnd, title.c_str());
}

void WindowBase::OnKeyUp(int key) { }
void WindowBase::OnKeyDown(int key) { }
void WindowBase::OnMouseButtonUp(MouseButtons btn) { }
void WindowBase::OnMouseButtonDown(MouseButtons btn) { }
void WindowBase::OnMouseWheel(int delta) { }
void WindowBase::OnResize()
{
	if (RenderTarget)
	{
		RenderTarget->Resize(windowSize);
	}
}

void WindowBase::Logic(float elapsedTime)
{
	if (RunWindow)
	{
		for (UIElementBase* e : elementsList)
			e->Logic(elapsedTime);
	}
}
void WindowBase::Draw() const
{
	if (RunWindow)
	{
		RenderTarget->BeginDraw();
		RenderTarget->Clear(backgroundColor);
		for (UIElementBase* e : elementsList)
			e->Draw();
		RenderTarget->EndDraw();
	}
}

void WindowBase::AddUIElement(UIElementBase* element)
{
	elementsList.push_back(element);
}

HWND WindowBase::GetHwnd() const
{
	return hwnd;
}

void WindowBase::RegisterMyWindowClass()
{
	static bool DoesClassRegistered = false;
	if (DoesClassRegistered) return;

	WNDCLASSEXW wcex = {};
	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowBase::WndProc;
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = WINDOW_CLASS_NAME;

	RegisterClassEx(&wcex);

	DoesClassRegistered = true;
}

LRESULT CALLBACK WindowBase::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowBase* pWnd = (WindowBase*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CREATE:			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams)); break;
	case WM_CLOSE:			if (pWnd) { pWnd->Close(); } PostQuitMessage(0); break;
	case WM_DISPLAYCHANGE:	InvalidateRect(hwnd, nullptr, FALSE); break;
	case WM_MOUSEMOVE:		if (pWnd) pWnd->mousePosition = { LOWORD(lParam), HIWORD(lParam) }; break;
	case WM_COMMAND:		if (pWnd) pWnd->Command(LOWORD(wParam)); break;
	case WM_KEYDOWN:		if (pWnd) pWnd->OnKeyDown((int)wParam); break;
	case WM_KEYUP:			if (pWnd) pWnd->OnKeyUp((int)wParam); break;
	case WM_LBUTTONUP:		if (pWnd) pWnd->OnMouseButtonUp(MouseButtons::Left); break;
	case WM_LBUTTONDOWN:	if (pWnd) pWnd->OnMouseButtonDown(MouseButtons::Left); break;
	case WM_MBUTTONUP:		if (pWnd) pWnd->OnMouseButtonUp(MouseButtons::Middle); break;
	case WM_MBUTTONDOWN:	if (pWnd) pWnd->OnMouseButtonDown(MouseButtons::Middle); break;
	case WM_RBUTTONUP:		if (pWnd) pWnd->OnMouseButtonUp(MouseButtons::Right); break;
	case WM_RBUTTONDOWN:	if (pWnd) pWnd->OnMouseButtonDown(MouseButtons::Right); break;
	case WM_MOUSEWHEEL:		if (pWnd) pWnd->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam)); break;
	case WM_SIZE:
		if (pWnd)
		{
			pWnd->windowSize = D2D1::SizeU(LOWORD(lParam), HIWORD(lParam));
			pWnd->OnResize();
		}
		break;
	default:				return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}


void WindowBase::FillRectangle(D2D1_RECT_F rect, D2D1::ColorF color)
{
	ID2D1SolidColorBrush* brush = nullptr;
	RenderTarget->CreateSolidColorBrush(color, &brush);
	if (brush)
	{
		RenderTarget->FillRectangle(rect, brush);
		brush->Release();
	}
}
void WindowBase::DrawRectangle(D2D1_RECT_F rect, D2D1::ColorF color, float width)
{
	ID2D1SolidColorBrush* brush = nullptr;
	RenderTarget->CreateSolidColorBrush(color, &brush);
	if (brush)
	{
		RenderTarget->DrawRectangle(rect, brush, width);
		brush->Release();
	}
}
void WindowBase::DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, D2D1::ColorF color, float width)
{
	ID2D1SolidColorBrush* brush = nullptr;
	RenderTarget->CreateSolidColorBrush(color, &brush);
	if (brush)
	{
		RenderTarget->DrawLine(p1, p2, brush, width);
		brush->Release();
	}
}
void WindowBase::DrawText(wstring text, FontData font, D2D1_RECT_F& layoutRect, D2D1::ColorF color)
{
	IDWriteTextFormat* TextFormat = nullptr;
	THROW_IF_FAILED(DWriteFactory->CreateTextFormat(font.family.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, font.size, L"", &TextFormat));

	if (TextFormat)
	{
		switch (font.horizontalAlignment)
		{
		case HorizontalAlignment::Right:	TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING); break;
		case HorizontalAlignment::Center:	TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); break;
		case HorizontalAlignment::Left:		TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING); break;
		}
		switch (font.verticalAlignment)
		{
		case VerticalAlignment::Top:	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR); break;
		case VerticalAlignment::Center:	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); break;
		case VerticalAlignment::Bottom:	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR); break;
		}

		ID2D1SolidColorBrush* brush = nullptr;
		RenderTarget->CreateSolidColorBrush(color, &brush);
		if (brush)
		{
			RenderTarget->DrawText(text.c_str(), (UINT32)text.length(), TextFormat, layoutRect, brush);
			brush->Release();
		}
		TextFormat->Release();
	}
}
void WindowBase::DrawBitmap(ID2D1Bitmap* bitmap, D2D1_RECT_F dstRect, D2D1_RECT_F srcRect, bool reversed, float opacity)
{
	if (reversed)
	{
		// to draw reversed we need to reverse the X-axis, so we change the sign of these points
		dstRect.left = -dstRect.left;
		dstRect.right = -dstRect.right;

		// set to draw reversed
		D2D1_MATRIX_3X2_F transformMatrix = D2D1::Matrix3x2F::Identity();
		transformMatrix._11 = -1;
		RenderTarget->SetTransform(transformMatrix);
	}

	RenderTarget->DrawBitmap(bitmap, dstRect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);

	if (reversed)
	{
		// back to normal
		RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
}
ID2D1Bitmap* WindowBase::LoadBitapImage(wstring imageFile)
#define TEST_HRESULT_FUNCTION(funcCall) if (FAILED(funcCall)) goto LoadBitapImage_end; // test function and if it faild go to end
{
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* source = nullptr;
	IWICFormatConverter* converter = nullptr;
	ID2D1Bitmap* toReturn = nullptr;

	TEST_HRESULT_FUNCTION(WICImagingFactory->CreateDecoderFromFilename(imageFile.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder));
	TEST_HRESULT_FUNCTION(decoder->GetFrame(0, &source));
	TEST_HRESULT_FUNCTION(WICImagingFactory->CreateFormatConverter(&converter));
	TEST_HRESULT_FUNCTION(converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeMedianCut));
	TEST_HRESULT_FUNCTION(RenderTarget->CreateBitmapFromWicBitmap(converter, &toReturn));

LoadBitapImage_end:
	if (decoder != nullptr) decoder->Release();
	if (source != nullptr) source->Release();
	if (converter != nullptr) converter->Release();

	return toReturn;
}
ID2D1Bitmap* WindowBase::LoadBitapImage(string imageFile)
{
	return LoadBitapImage(wstring(imageFile.begin(), imageFile.end()));
}
