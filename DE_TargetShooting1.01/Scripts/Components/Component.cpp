#include "Component.h"
#include "Actor.h"

//=================================================================
// コンストラクタ
//=================================================================
CComponent::CComponent(CActor* owner, int updateOrder) 
	:m_pOwner(owner)
	,m_iUpdateOrder(updateOrder)
{
	m_pOwner->AddComponent(this);
}

//=================================================================
// デストラクタ
//=================================================================
CComponent::~CComponent() 
{
	m_pOwner->RemoveComponent(this);
}

//=================================================================
// 更新
//=================================================================
void CComponent::Update(float deltaTime) 
{
}
