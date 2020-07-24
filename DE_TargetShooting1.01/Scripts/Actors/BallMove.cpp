#include "BallMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h""
#include "BallActor.h"
#include "HUD.h"
#include "GameClear.h"

//=================================================================
// コンストラクタ
//=================================================================
CBallMove::CBallMove(CActor* owner)
	:CMoveComponent(owner) 
{
}

//=================================================================
// 更新
//=================================================================
void CBallMove::Update(float deltaTime) 
{
	// 進行方向にセグメントを作成
	const float segmentLength = 30.0f;
	Vector3 start = m_pOwner->GetPosition();
	Vector3 dir = m_pOwner->GetForward();
	Vector3 end = start + dir * segmentLength;

	// 線分を作成する
	LineSegment l(start, end);

	// テストセグメントとワールド
	CPhysWorld* phys = m_pOwner->GetGame()->GetPhysWorld();
	CPhysWorld::CollisionInfo info;

	if (phys->SegmentCast(l, info) && info.m_pActor != m_pPlayer) 
	{
		// 衝突した場合は、法線についてボールを反射
		dir = Vector3::Reflect(dir, info.m_sNormal);
		m_pOwner->RotateToNewForward(dir);

		// ターゲットに衝突しているかどうか
		CTargetActor* target = dynamic_cast<CTargetActor*>(info.m_pActor);
		if (target) 
		{
			static_cast<CBallActor*>(m_pOwner)->HitTarget();
			static_cast<CBallActor*>(m_pOwner)->SetState(CActor::EDead);
			target->SetState(CActor::EDead);
			int target_num = m_pOwner->GetGame()->GetHUD()->GetTargetNum();
			target_num -= 1;
			m_pOwner->GetGame()->GetHUD()->SetTargetNum(target_num);
			if (target_num == 0) 
			{
				new CGameClear(m_pOwner->GetGame());
			}
		}
	}

	// 基本クラスの更新は、前進速度に基づいて移動します
	CMoveComponent::Update(deltaTime);
}