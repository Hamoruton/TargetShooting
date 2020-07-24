#pragma once
#ifndef __MOVECOMPONENT_H_INCLUDED__
#define __MOVECOMPONENT_H_INCLUDED__

#include "Component.h"

class CMoveComponent :
	public CComponent
{
public:
	CMoveComponent(class CActor* owner, int updateOrder = 10);
	virtual ~CMoveComponent();

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return m_fAngularSpeed; }
	float GetForwardSpeed() const { return m_fForwardSpeed; }
	float GetStrafeSpeed() const { return m_fStrafeSpeed; }
	void SetAngularSpeed(float speed) { m_fAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { m_fForwardSpeed = speed; }
	void SetStrafeSpeed(float speed) { m_fStrafeSpeed = speed; }

private:
	float		m_fAngularSpeed;
	float		m_fForwardSpeed;
	float		m_fStrafeSpeed;
};

#endif // !__MOVECOMPONENT_H_INCLUDED__