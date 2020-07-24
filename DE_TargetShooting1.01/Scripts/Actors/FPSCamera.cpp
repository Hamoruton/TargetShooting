#include "FPSCamera.h"
#include "Actor.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CFPSCamera::CFPSCamera(CActor* owner)
	:CCameraComponent(owner)
	, m_fPitchSpeed(0.0f)
	, m_fMaxPitch(Math::Pi / 3.0f)
	, m_fPitch(0.0f)
{
}

//=================================================================
// �X�V
//=================================================================
void CFPSCamera::Update(float deltaTime) 
{
	CCameraComponent::Update(deltaTime);

	// �J�����ʒu
	Vector3 cameraPos = m_pOwner->GetPosition();

	// �s�b�`���x�Ɋ�Â��ăs�b�`���X�V����
	m_fPitch += m_fPitchSpeed * deltaTime;
	m_fPitch = Math::Clamp(m_fPitch, -m_fMaxPitch, m_fMaxPitch);

	// �s�b�`��]��\���N�H�[�^�j�I�����쐬
	Quaternion q(m_pOwner->GetRight(), m_fPitch);

	// �s�b�`�N�H�[�^�j�I���ŃA�N�^��O���ɉ�]
	Vector3 viewForward = Vector3::Transform(m_pOwner->GetForward(), q);

	// �ڕW�ʒu����O��100���j�b�g�O
	Vector3 target = cameraPos + viewForward * 100.0f;
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	// �r���[�g�����X�t�H�[���ݒ�
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}