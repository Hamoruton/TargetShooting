#include "CircleComponent.h"
#include "Actor.h"

//=================================================================
// コンストラクタ
//=================================================================
CCircleComponent::CCircleComponent(class CActor* owner)
	:CComponent(owner)
	, m_fRadius(0.0f)
{
}

//=================================================================
// デストラクタ
//=================================================================
CCircleComponent::~CCircleComponent() 
{
}

//=================================================================
// 中心座標を取得
//=================================================================
const Vector3& CCircleComponent::GetCenter() const 
{
	return m_pOwner->GetPosition();
}

//=================================================================
// 半径取得
//=================================================================
float CCircleComponent::GetRadius() const 
{
	return m_pOwner->GetScale() * m_fRadius;
}

//=================================================================
// 衝突判定
//=================================================================
bool Collision(const CCircleComponent& a, const CCircleComponent& b) 
{
	Vector3 diff = a.GetCenter() - b.GetCenter();
	float distance_square = diff.LengthSq();

	float radius_square = a.GetRadius() + b.GetRadius();
	radius_square *= radius_square;

	return distance_square <= radius_square;
}