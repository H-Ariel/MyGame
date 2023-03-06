#pragma once

#include "framework.h"
#include "FontData.h"
#include <list>


class UIElementBase;


enum class MouseButtons
{
	Left,
	Middle,
	Right
};


class WindowBase
{
public:
	WindowBase(DWORD dwStyle = WS_OVERLAPPEDWINDOW, WindowBase* parent = nullptr);
	virtual ~WindowBase();

	void Run();
	virtual void Close();

	virtual void Command(WORD command); // called at `WM_COMMAND`

	D2D1_SIZE_U GetWindowSize();
	RECT GetWindowRect();
	void SetWindowRect(RECT wndRect);
	void SetTitle(wstring title);

	// Events:

	virtual void OnKeyUp(int key);
	virtual void OnKeyDown(int key);
	virtual void OnMouseButtonUp(MouseButtons btn);
	virtual void OnMouseButtonDown(MouseButtons btn);
	virtual void OnMouseWheel(int delta);
	virtual void OnResize();

	/*
	This is the logic part of the window. Add your game logic here.
	It runs every frame and it receives the elapsed time between the
	last frame and this one, in seconds. Use this value for a smooth
	and consistent movement, regardless of the CPU or graphics speed
	*/
	virtual void Logic(float elapsedTime);
	virtual void Draw() const;

	void AddUIElement(UIElementBase* element);

	void FillRectangle(D2D1_RECT_F rect, D2D1::ColorF color);
	void DrawRectangle(D2D1_RECT_F rect, D2D1::ColorF color, float width = 1);
	void DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, D2D1::ColorF color, float width = 1);
	void DrawText(wstring text, FontData font, D2D1_RECT_F& layoutRect, D2D1::ColorF color);
	void DrawBitmap(ID2D1Bitmap* bitmap, D2D1_RECT_F dstRect, D2D1_RECT_F srcRect, bool reversed = false, float opacity = 1.0f);
	ID2D1Bitmap* LoadBitapImage(wstring imageFile);
	ID2D1Bitmap* LoadBitapImage(string imageFile);

	
	D2D1_POINT_2U mousePosition;
	bool RunWindow;


protected:
	D2D1::ColorF backgroundColor;
	vector<UIElementBase*> elementsList;

	HWND GetHwnd() const;


private:
	static void RegisterMyWindowClass();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


	HWND hwnd;
	ID2D1HwndRenderTarget* RenderTarget;
	ID2D1Factory* D2DFactory;
	IDWriteFactory* DWriteFactory;
	IWICImagingFactory* WICImagingFactory;
	D2D1_SIZE_U windowSize;


	friend class UIElementBase;
};
