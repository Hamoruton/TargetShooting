#ifndef __AUDIOCOMPONENT_H_INCLUDED__
#define __AUDIOCOMPONENT_H_INCLUDED__

#pragma once
#include "Component.h"
#include "SoundEvent.h"

#include <vector>
#include <string>

class CAudioComponent :
	public CComponent
{
public:
	CAudioComponent(class CActor* owner, int updateOrder = 200);
	virtual ~CAudioComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;

	CSoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();

private:
	std::vector<CSoundEvent>	m_pEvents2D;
	std::vector<CSoundEvent>	m_pEvents3D;
};

#endif // !__AUDIOCOMPONENT_H_INCLUDED__