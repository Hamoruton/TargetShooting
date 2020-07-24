#ifndef __CAMERACOMPONENT_H_INCLUDED__
#define __CAMERACOMPONENT_H_INCLUDED__

#pragma once
#include "Component.h"
#include "Math.h"

class CCameraComponent :
	public CComponent
{
public:
	CCameraComponent(class CActor* owner, int updateOrder = 200);

protected:
	void SetViewMatrix(const Matrix4& view);
};

#endif // !__CAMERACOMPONENT_H_INCLUDED__