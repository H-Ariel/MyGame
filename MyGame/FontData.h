#pragma once

#include <string>


enum class HorizontalAlignment
{
	Right,
	Center,
	Left
};

enum class VerticalAlignment
{
	Top,
	Center,
	Bottom
};


class FontData
{
public:
	FontData();
	FontData(float size);
	FontData(float size, std::wstring family);
	FontData(float size, std::wstring family, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment);

	float size; // font size
	std::wstring family; // font family
	HorizontalAlignment horizontalAlignment;
	VerticalAlignment verticalAlignment;
};
