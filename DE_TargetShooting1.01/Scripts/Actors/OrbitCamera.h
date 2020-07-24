#ifndef __ORBITCAMERA_H_INCLUDED__
#define __ORBITCAMERA_H_INCLUDED__

#pragma once
#include "CameraComponent.h"

class COrbitCamera :
	public CCameraComponent
{
public:
	COrbitCamera(class CActor* owner);

	void Update(float deltaTime) override;

	float GetPitchSpeed() const { return m_fPitchSpeed; }
	float GetYawSpeed() const { return m_fYawSpeed; }

	void SetPitchSpeed(float speed) { m_fPitchSpeed = speed; }
	void SetYawSpeed(float speed) { m_fYawSpeed = speed; }

private:
	Vector3		m_vOffset;		// �ڕW�Ƃ̋���
	Vector3		m_vUp;			// �J���������ʒu
	float		m_fPitchSpeed;	// �s�b�`�̉�]/�b���x
	float		m_fYawSpeed;	// ���E�̉�]/�b���x
};

#endif // !__ORBITCAMERA_H_INCLUDED__