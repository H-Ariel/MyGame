#pragma once

#include "framework.h"


class CollisionDistances
{
public:
	static void keepSmallest(D2D1_RECT_F& rect);
	static void keepLargest(D2D1_RECT_F& rect);

	// if `a` and `b` collide - return the collision distances, else - empty rectangle
	static D2D1_RECT_F GetCollision(D2D1_RECT_F a, D2D1_RECT_F b);

	// return if `a` and `b` are collide
	static bool IsCollision(D2D1_RECT_F a, D2D1_RECT_F b);

	static bool IsEmpty(D2D1_RECT_F rect);

	static bool IsTopCollision(D2D1_RECT_F collisionRect);
	static bool IsBottomCollision(D2D1_RECT_F collisionRect);
};
