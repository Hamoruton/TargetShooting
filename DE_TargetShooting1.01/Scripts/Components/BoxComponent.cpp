#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"

//=================================================================
// コンストラクタ
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
// デストラクタ
//=================================================================
CBoxComponent::~CBoxComponent() 
{
	m_pOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

//=================================================================
// 更新
//=================================================================
void CBoxComponent::OnUpdateWorldTransform() 
{
	//オブジェクトスペースボックスにリセット
	m_cWorldBox = m_cObjectBox;
	//大きさ
	m_cWorldBox.m_sMin *= m_pOwner->GetScale();
	m_cWorldBox.m_sMax *= m_pOwner->GetScale();
	//回転
	if (m_bShouldRotate) 
	{
		m_cWorldBox.Rotate(m_pOwner->GetRotation());
	}
	//変換
	m_cWorldBox.m_sMin += m_pOwner->GetPosition();
	m_cWorldBox.m_sMax += m_pOwner->GetPosition();
}
