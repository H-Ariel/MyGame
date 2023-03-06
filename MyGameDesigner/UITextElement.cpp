#include "UITextElement.h"


UITextElement::UITextElement(WindowBase* window)
	: UIElementBase(window), text(L""), size({0,0}), fgcolor(D2D1::ColorF::Black), bgcolor(D2D1::ColorF::White)
{
}

UITextElement::UITextElement(WindowBase* window, wstring text, FontData font, D2D1_SIZE_F size, D2D1_POINT_2F position, D2D1::ColorF forgroundColor, D2D1::ColorF backgroundColor)
	: UIElementBase(window, position),
	text(text), font(font), size(size), fgcolor(forgroundColor), bgcolor(backgroundColor)
{
}

void UITextElement::Draw()
{
	D2D1_RECT_F dst = D2D1::RectF(
		position.x,
		position.y,
		position.x + size.width,
		position.y + size.height
	);
	window->FillRectangle(dst, bgcolor);
	window->DrawText(text, font, dst, fgcolor);
}
