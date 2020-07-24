#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CBoxComponent::CBoxComponent(CActor* owner, int updateOrder)
	:CComponent(owner, updateOrder)
	, m_cObjectBox(Vector3::Zero, Vector3::Zero)
	, m_cWorldBox(Vector3::Zero, Vector3::Zero)
	, m_bShouldRotate(true)
{
	m_pOwner->GetGame()->GetPhysWorld()->AddBox(this);
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CBoxComponent::~CBoxComponent() 
{
	m_pOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

//=================================================================
// �X�V
//=================================================================
void CBoxComponent::OnUpdateWorldTransform() 
{
	//�I�u�W�F�N�g�X�y�[�X�{�b�N�X�Ƀ��Z�b�g
	m_cWorldBox = m_cObjectBox;
	//�傫��
	m_cWorldBox.m_sMin *= m_pOwner->GetScale();
	m_cWorldBox.m_sMax *= m_pOwner->GetScale();
	//��]
	if (m_bShouldRotate) 
	{
		m_cWorldBox.Rotate(m_pOwner->GetRotation());
	}
	//�ϊ�
	m_cWorldBox.m_sMin += m_pOwner->GetPosition();
	m_cWorldBox.m_sMax += m_pOwner->GetPosition();
}
