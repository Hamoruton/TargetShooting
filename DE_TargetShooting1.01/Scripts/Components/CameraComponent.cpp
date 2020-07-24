#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Game.h"
#include "AudioSystem.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CCameraComponent::CCameraComponent(CActor* owner, int updateOrder)
	:CComponent(owner, updateOrder)
{
}

//=================================================================
// �r���[�g�����X�t�H�[����ݒ�
//=================================================================
void CCameraComponent::SetViewMatrix(const Matrix4& view) 
{
	// �r���[�}�g���b�N�X�������_���[�ƃI�[�f�B�I�V�X�e���ɓn��
	CGame* pGame = m_pOwner->GetGame();
	pGame->GetRenderer()->SetViewMatrix(view);
	pGame->GetAudioSystem()->SetListener(view);
}
