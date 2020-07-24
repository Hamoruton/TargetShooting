#include "FollowCamera.h"
#include "Actor.h"

//=================================================================
// コンストラクタ
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
// 更新
//=================================================================
void CFollowCamera::Update(float deltaTime) 
{
	CCameraComponent::Update(deltaTime);

	// スプリング定数から減衰を計算
	float dampening = 2.0f*Math::Sqrt(m_fSpringConstant);

	// 理想的な位置を計算
	Vector3 idealPos = ComputeCameraPos();

	// 実際と理想の違いを計算
	Vector3 diff = m_vActualPos - idealPos;

	// スプリングの加速度を計算
	Vector3 acel = -m_fSpringConstant * diff - dampening * m_vVelocity;

	// 速度更新
	m_vVelocity += acel * deltaTime;

	// カメラ位置更新
	m_vActualPos += m_vVelocity * deltaTime;
	Vector3 target = m_pOwner->GetPosition() + m_pOwner->GetForward()*m_fTargetDist;
	Matrix4 view = Matrix4::CreateLookAt(m_vActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

//=================================================================
// カメラ位置初期化
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
// カメラ位置
//=================================================================
Vector3 CFollowCamera::ComputeCameraPos() const 
{
	// 所有者の後ろと上のカメラ位置を設定
	Vector3 cameraPos = m_pOwner->GetPosition();
	cameraPos -= m_pOwner->GetForward()*m_fHorzDist;
	cameraPos += Vector3::UnitZ*m_fVertDist;
	return cameraPos;
}