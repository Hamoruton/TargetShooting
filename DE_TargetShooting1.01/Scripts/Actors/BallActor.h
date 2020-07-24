#ifndef __BALLACTOR_H_INCLUDED__
#define __BALLACTOR_H_INCLUDED__

#pragma once
#include "Actor.h"

class CBallActor :
	public CActor
{
public:
	CBallActor(class CGame* pGame);

	void UpdateActor(float deltaTime) override;
	void SetPlayer(CActor* player);

	void HitTarget();

private:
	class CAudioComponent*	m_pAudioComponent;
	class CBallMove*		m_pMyMove;
	float					m_fLifeSpan;
};

#endif // !__BALLACTOR_H_INCLUDED__
