#include "OrbitCamera.h"
#include "Actor.h"

//=================================================================
// コンストラクタ
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
// 更新
//=================================================================
void COrbitCamera::Update(float deltaTime) 
{
	CCameraComponent::Update(deltaTime);

	// カメラの上側のヨウクォータニオンを作成
	Quaternion yaw(Vector3::UnitZ, m_fYawSpeed*deltaTime);
	m_vOffset = Vector3::Transform(m_vOffset, yaw);
	m_vUp = Vector3::Transform(m_vUp, yaw);

	// カメラの前方/右側を計算
	Vector3 forward = -1.0f*m_vOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(m_vUp, forward);
	right.Normalize();

	// カメラの右側のピッチクォータニオンを作成
	Quaternion pitch(right, m_fPitchSpeed*deltaTime);
	m_vOffset = Vector3::Transform(m_vOffset, pitch);
	m_vUp = Vector3::Transform(m_vUp, pitch);

	// ワールド座標計算
	Vector3 target = m_pOwner->GetPosition();
	Vector3 cameraPos = target + m_vOffset;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, m_vUp);
	SetViewMatrix(view);
}
