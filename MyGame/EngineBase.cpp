#include "EngineBase.h"
#include "AssetsManager.h"


EngineBase::EngineBase()
	: backgroundColor(0), StopEngine(false), mousePosition({ 0,0 })
{
}
EngineBase::~EngineBase() { }

void EngineBase::OnKeyUp(int key) { }
void EngineBase::OnKeyDown(int key) { }
void EngineBase::OnMouseButtonUp(MouseButtons btn) { }
void EngineBase::OnMouseButtonDown(MouseButtons btn) { }
void EngineBase::OnMouseWheel(int delta) { }
void EngineBase::OnResize() { }

void EngineBase::Logic(float elapsedTime)
{
	for (UIElementBase* e : elementsList)
		e->Logic(elapsedTime);
}
void EngineBase::Draw() const
{
	AssetsManager::RenderTarget->BeginDraw();
	AssetsManager::RenderTarget->Clear(backgroundColor);
	for (UIElementBase* e : elementsList)
		e->Draw();
	AssetsManager::RenderTarget->EndDraw();
}

void EngineBase::AddUIElement(UIElementBase* element)
{
	elementsList.push_back(element);
}
void EngineBase::RemoveUIElement(UIElementBase* element)
{
	elementsList.remove(element);
}
void EngineBase::RemoveAllUIElements()
{
	elementsList.clear();
}
