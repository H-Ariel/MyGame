#include "CollisionDistances.h"
#include <cmath>


void CollisionDistances::keepSmallest(D2D1_RECT_F& rect)
{
	if (rect.top != 0 && rect.top < rect.bottom && rect.top < rect.left && rect.top < rect.right)
	{
		rect.bottom = 0;
		rect.left = 0;
		rect.right = 0;
	}
	else if (rect.bottom != 0 && rect.bottom < rect.top && rect.bottom < rect.left && rect.bottom < rect.right)
	{
		rect.top = 0;
		rect.left = 0;
		rect.right = 0;
	}
	else if (rect.left != 0 && rect.left < rect.top && rect.left < rect.bottom && rect.left < rect.right)
	{
		rect.top = 0;
		rect.bottom = 0;
		rect.right = 0;
	}
	else if (rect.right != 0 && rect.right < rect.top && rect.right < rect.bottom && rect.right < rect.left)
	{
		rect.top = 0;
		rect.bottom = 0;
		rect.left = 0;
	}
}

void CollisionDistances::keepLargest(D2D1_RECT_F& rect)
{
	if (rect.top != 0 && rect.top > rect.bottom && rect.top > rect.left && rect.top > rect.right)
	{
		rect.bottom = 0;
		rect.left = 0;
		rect.right = 0;
	}
	else if (rect.bottom != 0 && rect.bottom > rect.top && rect.bottom > rect.left && rect.bottom > rect.right)
	{
		rect.top = 0;
		rect.left = 0;
		rect.right = 0;
	}
	else if (rect.left != 0 && rect.left > rect.top && rect.left > rect.bottom && rect.left > rect.right)
	{
		rect.top = 0;
		rect.bottom = 0;
		rect.right = 0;
	}
	else if (rect.right != 0 && rect.right > rect.top && rect.right > rect.bottom && rect.right > rect.left)
	{
		rect.top = 0;
		rect.bottom = 0;
		rect.left = 0;
	}
}

D2D1_RECT_F CollisionDistances::GetCollision(D2D1_RECT_F a, D2D1_RECT_F b)
{
	D2D1_RECT_F collision = {};

	if (a.top < b.bottom && a.bottom > b.top && a.left < b.right && a.right > b.left)
	{
		collision.top = b.bottom - a.top;
		collision.bottom = a.bottom - b.top;
		collision.left = b.right - a.left;
		collision.right = a.right - b.left;
	}

	return collision;
}

bool CollisionDistances::IsCollision(D2D1_RECT_F a, D2D1_RECT_F b)
{
	return !IsEmpty(GetCollision(a, b));
}

bool CollisionDistances::IsEmpty(D2D1_RECT_F rect)
{
	return rect.top == 0
		&& rect.bottom == 0
		&& rect.right == 0
		&& rect.left == 0;
}

bool CollisionDistances::IsTopCollision(D2D1_RECT_F collisionRect)
{
	keepSmallest(collisionRect);
	return collisionRect.top > 0;
}

bool CollisionDistances::IsBottomCollision(D2D1_RECT_F collisionRect)
{
	keepSmallest(collisionRect);
	return collisionRect.bottom > 0;
}
