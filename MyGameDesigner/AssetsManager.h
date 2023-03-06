#pragma once

#include "framework.h"


class WindowBase;


class AssetsManager
{
public:
	AssetsManager(WindowBase* window);
	~AssetsManager();


	ID2D1Bitmap* TilesImageSet;
	ID2D1Bitmap* EnemiesImageSet;

	static const vector<D2D1_RECT_F> TilesPositions;
	static const map<string, D2D1_RECT_F> EnemiesPositions;
};
