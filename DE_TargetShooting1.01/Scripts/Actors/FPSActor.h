#ifndef __FPSACTOR_H_INCLUDED__
#define __FPSACTOR_H_INCLUDED__

#pragma once
#include "Actor.h"
#include "SoundEvent.h"

class CFPSActor :
	public CActor
{
public:
	CFPSActor(class CGame* pGame);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

	void Shoot();

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);

	void FixCollisions();

private:
	class CMoveComponent*		m_pMoveComponent;
	class CAudioComponent*		m_pAudioComponent;
	class CMeshComponent*		m_pMeshComponent;
	class CFPSCamera*			m_pCameraComponent;
	class CActor*				m_pFPSModel;
	class CBoxComponent*		m_pBoxComponent;
	CSoundEvent					m_cFootstep;
	float						m_fLastFootstep;
};

#endif // !__FPSACTOR_H_INCLUDED__