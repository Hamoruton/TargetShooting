#ifndef __FPSCAMERA_H_INCLUDED__
#define __FPSCAMERA_H_INCLUDED__

#pragma once
#include "CameraComponent.h"

class CFPSCamera :
	public CCameraComponent
{
public:
	CFPSCamera(class CActor* owner);

	void Update(float deltaTime) override;

	float GetPitch() const { return m_fPitch; }
	float GetPitchSpeed() const { return m_fPitchSpeed; }
	float GetMaxPitch() const { return m_fMaxPitch; }

	void SetPitchSpeed(float speed) { m_fPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { m_fMaxPitch = pitch; }

private:
	float	m_fPitchSpeed;		// �s�b�`�̉�]/�b���x
	float	m_fMaxPitch;		// �O������̍ő�s�b�`�΍�
	float	m_fPitch;			// ���݂̃s�b�`
};

#endif // !__FPSCAMERA_H_INCLUDED__