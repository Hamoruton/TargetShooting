#ifndef __FOLLOWCAMERA_H_INCLUDED__
#define __FOLLOWCAMERA_H_INCLUDED__

#pragma once
#include "CameraComponent.h"

class CFollowCamera :
	public CCameraComponent
{
public:
	CFollowCamera(class CActor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { m_fHorzDist = dist; }
	void SetVertDist(float dist) { m_fVertDist = dist; }
	void SetTargetDist(float dist) { m_fTargetDist = dist; }
	void SetSpringConstant(float spring) { m_fSpringConstant = spring; }

private:
	Vector3 ComputeCameraPos() const;
	
	Vector3 m_vActualPos;		// �J�����̈ʒu
	Vector3 m_vVelocity;		// �J�����̑��x
	float	m_fHorzDist;		// �����Ǐ]����
	float	m_fVertDist;		// �����Ǐ]����
	float	m_fTargetDist;		// �ڕW����
	float	m_fSpringConstant;	// �X�v�����O�萔
};

#endif // !__FOLLOWCAMERA_H_INCLUDED__