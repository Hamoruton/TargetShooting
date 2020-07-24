#include "MoveComponent.h"
#include "Actor.h"

//=================================================================
// コンストラクタ
//=================================================================
CMoveComponent::CMoveComponent(class CActor* owner, int updateOrder)
	:CComponent(owner, updateOrder)
	,m_fAngularSpeed(0.0f)
	,m_fForwardSpeed(0.0f)
{
}

//=================================================================
// デストラクタ
//=================================================================
CMoveComponent::~CMoveComponent() 
{
}

//=================================================================
// 更新
//=================================================================
void CMoveComponent::Update(float deltaTime) 
{
	if (!Math::NearZero(m_fAngularSpeed)) 
	{
		Quaternion rot = m_pOwner->GetRotation();
		float angle = m_fAngularSpeed * deltaTime;
		// 変位
		Quaternion inc(Vector3::UnitZ, angle);
		// 回転
		rot = Quaternion::Concatenate(rot, inc);
		m_pOwner->SetRotation(rot);
	}

	if (!Math::NearZero(m_fForwardSpeed) || !Math::NearZero(m_fStrafeSpeed))
	{
		Vector3 pos = m_pOwner->GetPosition();
		pos += m_pOwner->GetForward()*m_fForwardSpeed*deltaTime;
		pos += m_pOwner->GetRight()*m_fStrafeSpeed*deltaTime;
		m_pOwner->SetPosition(pos);
	}
}