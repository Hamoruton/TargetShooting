#include "TargetComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "HUD.h"
#include "BoxComponent.h"
#include "Math.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CTargetComponent::CTargetComponent(CActor* owner)
	:CComponent(owner) 
{
	m_pOwner->GetGame()->GetHUD()->AddTargetComponent(this);
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CTargetComponent::~CTargetComponent() 
{
	m_pOwner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}

