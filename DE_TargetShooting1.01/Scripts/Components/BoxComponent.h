#ifndef __BOXCOMPONENT_H_INCLUDED__
#define __BOXCOMPONENT_H_INCLUDED__

#pragma once
#include "Component.h"
#include "Collision.h"

class CBoxComponent :
	public CComponent
{
public:
	CBoxComponent(class CActor* owner, int updateOrder = 100);
	virtual ~CBoxComponent();

	void OnUpdateWorldTransform() override;

	void SetObjectBox(const AABB& model) { m_cObjectBox = model; }
	const AABB& GetWorldBox() const { return m_cWorldBox; }

	void SetShouldRotate(bool value) { m_bShouldRotate = value; }

private:
	AABB	m_cObjectBox;
	AABB	m_cWorldBox;
	bool	m_bShouldRotate;
};

#endif // !__BOXCOMPONENT_H_INCLUDED__