#ifndef __FOLLOWACTOR_H_INCLUDED__
#define __FOLLOWACTOR_H_INCLUDED__

#pragma once
#include "Actor.h"

class CFollowActor :
	public CActor
{
public:
	CFollowActor(class CGame* pGame);

	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);

private:
	class CMoveComponent*		m_pMoveComponent;
	class CFollowCamera*		m_pCameraComponent;
	class CMeshComponent*		m_pMeshComponent;
};

#endif // !__FOLLOWACTOR_H_INCLUDED__