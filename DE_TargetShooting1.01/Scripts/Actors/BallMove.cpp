#include "BallMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h""
#include "BallActor.h"
#include "HUD.h"
#include "GameClear.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CBallMove::CBallMove(CActor* owner)
	:CMoveComponent(owner) 
{
}

//=================================================================
// �X�V
//=================================================================
void CBallMove::Update(float deltaTime) 
{
	// �i�s�����ɃZ�O�����g���쐬
	const float segmentLength = 30.0f;
	Vector3 start = m_pOwner->GetPosition();
	Vector3 dir = m_pOwner->GetForward();
	Vector3 end = start + dir * segmentLength;

	// �������쐬����
	LineSegment l(start, end);

	// �e�X�g�Z�O�����g�ƃ��[���h
	CPhysWorld* phys = m_pOwner->GetGame()->GetPhysWorld();
	CPhysWorld::CollisionInfo info;

	if (phys->SegmentCast(l, info) && info.m_pActor != m_pPlayer) 
	{
		// �Փ˂����ꍇ�́A�@���ɂ��ă{�[���𔽎�
		dir = Vector3::Reflect(dir, info.m_sNormal);
		m_pOwner->RotateToNewForward(dir);

		// �^�[�Q�b�g�ɏՓ˂��Ă��邩�ǂ���
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

	// ��{�N���X�̍X�V�́A�O�i���x�Ɋ�Â��Ĉړ����܂�
	CMoveComponent::Update(deltaTime);
}