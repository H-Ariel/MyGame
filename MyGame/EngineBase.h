#pragma once

#include "UIElementBase.h"
#include <list>


class EngineBase
{
public:
	EngineBase();
	virtual ~EngineBase();

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
	void Draw() const;

	
	D2D1_POINT_2U mousePosition;
	bool StopEngine;


protected:
	void AddUIElement(UIElementBase* element);
	void RemoveUIElement(UIElementBase* element);
	void RemoveAllUIElements();


	D2D1::ColorF backgroundColor;
	list<UIElementBase*> elementsList;
};
