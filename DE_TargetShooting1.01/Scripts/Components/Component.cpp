#include "Component.h"
#include "Actor.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CComponent::CComponent(CActor* owner, int updateOrder) 
	:m_pOwner(owner)
	,m_iUpdateOrder(updateOrder)
{
	m_pOwner->AddComponent(this);
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CComponent::~CComponent() 
{
	m_pOwner->RemoveComponent(this);
}

//=================================================================
// �X�V
//=================================================================
void CComponent::Update(float deltaTime) 
{
}
