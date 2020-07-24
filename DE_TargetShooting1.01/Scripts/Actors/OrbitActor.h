#ifndef __ORBITACTOR_H_INCLUDED__
#define __ORBITACTOR_H_INCLUDED__

#pragma once
#include "Actor.h"

class COrbitActor :
	public CActor
{
public:
	COrbitActor(class CGame* pGame);

	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);

private:
	class COrbitCamera*		m_pCameraComponent;
	class CMeshComponent*	m_pMeshComponent;
};

#endif // !__ORBITACTOR_H_INCLUDED__