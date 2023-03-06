#pragma once

#include "GameDesignerWindowBase.h"
#include "AssetsManager.h"


class EnemyButton;


class SelectEnemy_Window : public GameDesignerWindowBase
{
public:
	SelectEnemy_Window(WindowBase* parent);
	~SelectEnemy_Window();
	void Close() override;
	void OnMouseWheel(int delta) override;

private:
	vector<EnemyButton*> buttons;
	D2D1_SIZE_U wndSize;

	friend class EnemyButton;
};
