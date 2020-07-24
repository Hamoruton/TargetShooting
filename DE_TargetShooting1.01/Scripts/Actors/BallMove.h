#ifndef __BALLMOVE_H_INCLUDED__
#define __BALLMOVE_H_INCLUDED__

#pragma once
#include "MoveComponent.h"

class CBallMove :
	public CMoveComponent
{
public:
	CBallMove(class CActor* owner);

	void SetPlayer(CActor* player) { m_pPlayer = player; }
	void Update(float deltaTime) override;

protected:
	class CActor*	m_pPlayer;
};

#endif // !__BALLMOVE_H_INCLUDED__
