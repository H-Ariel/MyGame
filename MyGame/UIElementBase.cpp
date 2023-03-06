#include "UIElementBase.h"


UIElementBase::UIElementBase(D2D1_POINT_2F position)
	: position(position)
{
}

void UIElementBase::Logic(float elapsedTime) { }
void UIElementBase::Draw() const { }
