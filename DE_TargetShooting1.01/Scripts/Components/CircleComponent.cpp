#include "CircleComponent.h"
#include "Actor.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CCircleComponent::CCircleComponent(class CActor* owner)
	:CComponent(owner)
	, m_fRadius(0.0f)
{
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CCircleComponent::~CCircleComponent() 
{
}

//=================================================================
// ���S���W���擾
//=================================================================
const Vector3& CCircleComponent::GetCenter() const 
{
	return m_pOwner->GetPosition();
}

//=================================================================
// ���a�擾
//=================================================================
float CCircleComponent::GetRadius() const 
{
	return m_pOwner->GetScale() * m_fRadius;
}

//=================================================================
// �Փ˔���
//=================================================================
bool Collision(const CCircleComponent& a, const CCircleComponent& b) 
{
	Vector3 diff = a.GetCenter() - b.GetCenter();
	float distance_square = diff.LengthSq();

	float radius_square = a.GetRadius() + b.GetRadius();
	radius_square *= radius_square;

	return distance_square <= radius_square;
}