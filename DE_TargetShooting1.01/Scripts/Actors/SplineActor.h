#ifndef __SPLINEACTOR_H_INCLUDED__
#define __SPLINEACTOR_H_INCLUDED__

#pragma once
#include "Actor.h"

class CSplineActor :
	public CActor
{
public:
	CSplineActor(class CGame* pGame);

	void ActorInput(const uint8_t* keys) override;

	void RestartSpline();

private:
	class CSplineCamera*	m_pCameraComponent;
};

#endif // !__SPLINEACTOR_H_INCLUDED__

