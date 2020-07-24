#include "OrbitCamera.h"
#include "Actor.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
COrbitCamera::COrbitCamera(CActor* owner)
	:CCameraComponent(owner)
	, m_vOffset(-400.0f, 0.0f, 100.0f)
	, m_vUp(Vector3::UnitZ)
	, m_fPitchSpeed(0.0f)
	, m_fYawSpeed(0.0f)
{
}

//=================================================================
// �X�V
//=================================================================
void COrbitCamera::Update(float deltaTime) 
{
	CCameraComponent::Update(deltaTime);

	// �J�����̏㑤�̃��E�N�H�[�^�j�I�����쐬
	Quaternion yaw(Vector3::UnitZ, m_fYawSpeed*deltaTime);
	m_vOffset = Vector3::Transform(m_vOffset, yaw);
	m_vUp = Vector3::Transform(m_vUp, yaw);

	// �J�����̑O��/�E�����v�Z
	Vector3 forward = -1.0f*m_vOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(m_vUp, forward);
	right.Normalize();

	// �J�����̉E���̃s�b�`�N�H�[�^�j�I�����쐬
	Quaternion pitch(right, m_fPitchSpeed*deltaTime);
	m_vOffset = Vector3::Transform(m_vOffset, pitch);
	m_vUp = Vector3::Transform(m_vUp, pitch);

	// ���[���h���W�v�Z
	Vector3 target = m_pOwner->GetPosition();
	Vector3 cameraPos = target + m_vOffset;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, m_vUp);
	SetViewMatrix(view);
}
