#ifndef __TARGETACTOR_H_INCLUDED__
#define __TARGETACTOR_H_INCLUDED__

#pragma once
#include "Actor.h"

class CTargetActor :
	public CActor
{
public:
	CTargetActor(class CGame* pGame);
};

#endif // !__TARGETACTOR_H_INCLUDED__