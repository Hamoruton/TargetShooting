#pragma once
#ifndef __CIRCLECOMPONENT_H_INCLUDED__
#define __CIRCLECOMPONENT_H_INCLUDED__

#include "Component.h"
#include "Math.h"

class CCircleComponent :
	public CComponent
{
public:
	CCircleComponent(class CActor* owner);
	virtual ~CCircleComponent();

	float GetRadius() const;
	void SetRadius(float radius) { m_fRadius = radius; }

	const Vector3& GetCenter() const;

private:
	float	m_fRadius;
};

bool Collision(const CCircleComponent& a, const CCircleComponent& b);

#endif // !__CIRCLECOMPONENT_H_INCLUDED__