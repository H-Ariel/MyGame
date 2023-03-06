#pragma once

#include "UIElementBase.h"
#include "FontData.h"


class UITextElement : public UIElementBase
{
public:
	UITextElement();
	UITextElement(wstring text,
		FontData font,
		D2D1_SIZE_F size,
		D2D1_POINT_2F position,
		D2D1::ColorF forgroundColor,
		D2D1::ColorF backgroundColor
	);

	void Draw() const override;


	wstring text;
	D2D1_SIZE_F size;
	FontData font;
	D2D1::ColorF fgcolor; // forground color
	D2D1::ColorF bgcolor; // background color
};
