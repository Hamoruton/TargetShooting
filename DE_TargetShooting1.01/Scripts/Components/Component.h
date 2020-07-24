#pragma once
#ifndef __COMPONENT_H_INCLUDED__
#define __COMPONENT_H_INCLUDED__

#include <cstdint>

class CComponent
{
public:
	CComponent(class CActor* owner, int updateOrder = 100);
	virtual ~CComponent();

	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keyState) {}
	virtual void OnUpdateWorldTransform() {}

	class CActor* GetOwner() { return m_pOwner; }
	int GetUpdateOrder() const { return m_iUpdateOrder; }

protected:
	class CActor*	m_pOwner;
	int				m_iUpdateOrder;
};

#endif // !__COMPONENT_H_INCLUDED__