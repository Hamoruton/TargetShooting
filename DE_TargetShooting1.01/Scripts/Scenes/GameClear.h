#ifndef __GAMECLEAR_H_INCLUDED__
#define __GAMECLEAR_H_INCLUDED__

#pragma once
#include "UIScreen.h"

class CGameClear :
	public CUIScreen
{
public:
	CGameClear(class CGame* pGame);
	~CGameClear();

	void LoadScene();
};

#endif // !__GAMECLEAR_H_INCLUDED__