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
	
	Vector3 m_vActualPos;		// カメラの位置
	Vector3 m_vVelocity;		// カメラの速度
	float	m_fHorzDist;		// 水平追従距離
	float	m_fVertDist;		// 鉛直追従距離
	float	m_fTargetDist;		// 目標距離
	float	m_fSpringConstant;	// スプリング定数
};

#endif // !__FOLLOWCAMERA_H_INCLUDED__