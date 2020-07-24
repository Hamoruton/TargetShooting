#ifndef __PAUSEMENU_H_INCLUDED__
#define __PAUSEMENU_H_INCLUDED__

#pragma once
#include "UIScreen.h"

class CPauseMenu :
	public CUIScreen
{
public:
	CPauseMenu(class CGame* pGame);
	~CPauseMenu();

	void HandleKeyPress(int key) override;
};

#endif // !__PAUSEMENU_H_INCLUDED__
