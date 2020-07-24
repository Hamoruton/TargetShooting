#include "TargetComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "HUD.h"
#include "BoxComponent.h"
#include "Math.h"

//=================================================================
// コンストラクタ
//=================================================================
CTargetComponent::CTargetComponent(CActor* owner)
	:CComponent(owner) 
{
	m_pOwner->GetGame()->GetHUD()->AddTargetComponent(this);
}

//=================================================================
// デストラクタ
//=================================================================
CTargetComponent::~CTargetComponent() 
{
	m_pOwner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}

