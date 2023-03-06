#include "FontData.h"


FontData::FontData()
	: size(30), family(L"Consolas"), horizontalAlignment(HorizontalAlignment::Center), verticalAlignment(VerticalAlignment::Center)
{
}
FontData::FontData(float size)
	: size(size), family(L"Consolas"), horizontalAlignment(HorizontalAlignment::Center), verticalAlignment(VerticalAlignment::Center)
{
}
FontData::FontData(float size, std::wstring family)
	: size(size), family(family), horizontalAlignment(HorizontalAlignment::Center), verticalAlignment(VerticalAlignment::Center)
{
}
FontData::FontData(float size, std::wstring family, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment)
	: size(size), family(family), horizontalAlignment(horizontalAlignment), verticalAlignment(verticalAlignment)
{
}
