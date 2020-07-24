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
	Vector3		m_vOffset;		// 目標との距離
	Vector3		m_vUp;			// カメラ鉛直位置
	float		m_fPitchSpeed;	// ピッチの回転/秒速度
	float		m_fYawSpeed;	// ヨウの回転/秒速度
};

#endif // !__ORBITCAMERA_H_INCLUDED__