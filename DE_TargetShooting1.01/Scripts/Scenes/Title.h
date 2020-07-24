#ifndef __TITLE_H_INCLUDED__
#define __TITLE_H_INCLUDED__

#pragma once
#include "UIScreen.h"

class CTitle :
	public CUIScreen
{
public:
	CTitle(class CGame* pGame);
	~CTitle();
};

#endif // !__TITLE_H_INCLUDED__
