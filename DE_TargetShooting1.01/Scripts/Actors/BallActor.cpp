#include "BallActor.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "AudioComponent.h"
#include "BallMove.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CBallActor::CBallActor(CGame* pGame)
	:CActor(pGame)
	, m_fLifeSpan(2.0f)
{
	CMeshComponent* mc = new CMeshComponent(this);
	CMesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
	mc->SetMesh(mesh);
	m_pMyMove = new CBallMove(this);
	m_pMyMove->SetForwardSpeed(1500.0f);
	m_pAudioComponent = new CAudioComponent(this);
}

//=================================================================
// �X�V
//=================================================================
void CBallActor::UpdateActor(float deltaTime) 
{
	CActor::UpdateActor(deltaTime);

	m_fLifeSpan -= deltaTime;
	if (m_fLifeSpan < 0.0f) 
	{
		SetState(EDead);
	}
}

//=================================================================
// �v���C���[�A�N�^�ݒ�
//=================================================================
void CBallActor::SetPlayer(CActor* player) 
{
	m_pMyMove->SetPlayer(player);
}

//=================================================================
// �^�[�Q�b�g�A�N�^�Ƃ̏Փ˔���
//=================================================================
void CBallActor::HitTarget() 
{
	m_pAudioComponent->PlayEvent("event:/Explosion2D");
}
