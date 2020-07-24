#include "FPSCamera.h"
#include "Actor.h"

//=================================================================
// コンストラクタ
//=================================================================
CFPSCamera::CFPSCamera(CActor* owner)
	:CCameraComponent(owner)
	, m_fPitchSpeed(0.0f)
	, m_fMaxPitch(Math::Pi / 3.0f)
	, m_fPitch(0.0f)
{
}

//=================================================================
// 更新
//=================================================================
void CFPSCamera::Update(float deltaTime) 
{
	CCameraComponent::Update(deltaTime);

	// カメラ位置
	Vector3 cameraPos = m_pOwner->GetPosition();

	// ピッチ速度に基づいてピッチを更新する
	m_fPitch += m_fPitchSpeed * deltaTime;
	m_fPitch = Math::Clamp(m_fPitch, -m_fMaxPitch, m_fMaxPitch);

	// ピッチ回転を表すクォータニオンを作成
	Quaternion q(m_pOwner->GetRight(), m_fPitch);

	// ピッチクォータニオンでアクタを前方に回転
	Vector3 viewForward = Vector3::Transform(m_pOwner->GetForward(), q);

	// 目標位置から前方100ユニット前
	Vector3 target = cameraPos + viewForward * 100.0f;
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	// ビュートランスフォーム設定
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}