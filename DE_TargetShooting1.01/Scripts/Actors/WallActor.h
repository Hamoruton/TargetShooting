#ifndef __WALLACTOR_H_INCLUDED__
#define __WALLACTOR_H_INCLUDED__

#pragma once
#include "Actor.h"

class CWallActor :
	public CActor
{
public:
	CWallActor(class CGame* pGame);
	~CWallActor();

	class CBoxComponent* GetBox() { return m_pBox; }

private:
	class CBoxComponent*	m_pBox;
};

#endif // !__WALLACTOR_H_INCLUDED__
