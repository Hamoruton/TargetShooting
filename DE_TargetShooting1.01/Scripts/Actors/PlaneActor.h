#ifndef __PLANEACTOR_H_INCLUDED__
#define __PLANEACTOR_H_INCLUDED__

#pragma once
#include "Actor.h"

class CPlaneActor :
	public CActor
{
public:
	CPlaneActor(class CGame* pGame);
	virtual ~CPlaneActor();

	class CBoxComponent* GetBox() { return m_pBox; }

private:
	class CBoxComponent*	m_pBox;
};

#endif // !__PLANEACTOR_H_INCLUDED__

