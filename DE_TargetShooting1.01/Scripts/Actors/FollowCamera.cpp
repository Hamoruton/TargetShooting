#include "FollowCamera.h"
#include "Actor.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CFollowCamera::CFollowCamera(CActor* owner)
	:CCameraComponent(owner)
	, m_fHorzDist(350.0f)
	, m_fVertDist(150.0f)
	, m_fTargetDist(100.0f)
	, m_fSpringConstant(64.0f)
{
}

//=================================================================
// �X�V
//=================================================================
void CFollowCamera::Update(float deltaTime) 
{
	CCameraComponent::Update(deltaTime);

	// �X�v�����O�萔���猸�����v�Z
	float dampening = 2.0f*Math::Sqrt(m_fSpringConstant);

	// ���z�I�Ȉʒu���v�Z
	Vector3 idealPos = ComputeCameraPos();

	// ���ۂƗ��z�̈Ⴂ���v�Z
	Vector3 diff = m_vActualPos - idealPos;

	// �X�v�����O�̉����x���v�Z
	Vector3 acel = -m_fSpringConstant * diff - dampening * m_vVelocity;

	// ���x�X�V
	m_vVelocity += acel * deltaTime;

	// �J�����ʒu�X�V
	m_vActualPos += m_vVelocity * deltaTime;
	Vector3 target = m_pOwner->GetPosition() + m_pOwner->GetForward()*m_fTargetDist;
	Matrix4 view = Matrix4::CreateLookAt(m_vActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

//=================================================================
// �J�����ʒu������
//=================================================================
void CFollowCamera::SnapToIdeal() 
{
	m_vActualPos = ComputeCameraPos();
	m_vVelocity = Vector3::Zero;
	Vector3 target = m_pOwner->GetPosition() + m_pOwner->GetForward()*m_fTargetDist;
	Matrix4 view = Matrix4::CreateLookAt(m_vActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

//=================================================================
// �J�����ʒu
//=================================================================
Vector3 CFollowCamera::ComputeCameraPos() const 
{
	// ���L�҂̌��Ə�̃J�����ʒu��ݒ�
	Vector3 cameraPos = m_pOwner->GetPosition();
	cameraPos -= m_pOwner->GetForward()*m_fHorzDist;
	cameraPos += Vector3::UnitZ*m_fVertDist;
	return cameraPos;
}