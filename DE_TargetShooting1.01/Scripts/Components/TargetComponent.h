#ifndef __TARGETCOMPONENT_H_INCLUDED__
#define __TARGETCOMPONENT_H_INCLUDED__

#pragma once
#include "Component.h"

class CTargetComponent :
	public CComponent
{
public:
	CTargetComponent(class CActor* owner);
	~CTargetComponent();
};

#endif // !__TARGETCOMPONENT_H_INCLUDED__
