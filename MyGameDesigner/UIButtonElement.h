#pragma once

#include "UITextElement.h"
#include <functional>


class UIButtonElement : public UITextElement
{
public:
	UIButtonElement(WindowBase* window);

	UIButtonElement(WindowBase* window,
		function<void(MouseButtons)> OnClick,
		wstring text,
		FontData font,
		D2D1_SIZE_F size,
		D2D1_POINT_2F position,
		D2D1::ColorF forgroundColor,
		D2D1::ColorF backgroundColor
	);

	void Logic(float elapsedTime);

	function<void(MouseButtons)> OnClick;

private:
	HWND btnWnd;
	void CreateWnd();
};
