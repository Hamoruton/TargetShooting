#ifndef __DIALOGBOX_H_INCLUDED__
#define __DIALOGBOX_H_INCLUDED__

#pragma once
#include "UIScreen.h"

class CDialogBox :
	public CUIScreen
{
public:
	CDialogBox(class CGame* pGame, const std::string& text,
		std::function<void()> onOK);
	~CDialogBox();
};

#endif // !__DIALOGBOX_H_INCLUDED__
