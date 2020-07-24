#ifndef __SPLINECAMERA_H_INCLUDED__
#define __SPLINECAMERA_H_INCLUDED__

#pragma once
#include "CameraComponent.h"
#include <vector>

struct Spline 
{
	std::vector<Vector3>	m_vControlPoints;
	Vector3	Compute(size_t startIdx, float t) const;
	size_t GetNumPoints() const { return m_vControlPoints.size(); }
};

class CSplineCamera :
	public CCameraComponent
{
public:
	CSplineCamera(class CActor* owner);

	void Update(float deltaTime) override;
	void Restart();

	void SetSpeed(float speed) { m_fSpeed = speed; }
	void SetSpline(const Spline& spline) { m_sPath = spline; }
	void SetPaused(bool pause) { m_bPaused = pause; }

private:
	Spline		m_sPath;
	size_t		m_szIndex;
	float		m_fT;
	float		m_fSpeed;
	bool		m_bPaused;
};

#endif // !__SPLINECAMERA_H_INCLUDED__