#include "Actor.h"
#include "Game.h"
#include "Component.h"

//=================================================================
// コンストラクタ
//=================================================================
CActor::CActor(CGame* pGame)
	:m_eState(EActive)
	, m_vecPosition(Vector3::Zero)
	, m_fScale(1.0f)
	, m_qRotation(Quaternion::Identity)
	, m_pGame(pGame)
	, m_bRecomputeWorldTransform(true)
{
	m_pGame->AddActor(this);
}

//=================================================================
// デストラクタ
//=================================================================
CActor::~CActor() 
{
	m_pGame->RemoveActor(this);
	while (!m_pComponents.empty()) 
	{
		delete m_pComponents.back();
	}
}

//=================================================================
// 更新
//=================================================================
void CActor::Update(float deltaTime) 
{
	if (m_eState == EActive) 
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

//=================================================================
// コンポーネント更新
//=================================================================
void CActor::UpdateComponents(float deltaTime) 
{
	for (auto comp : m_pComponents) 
	{
		comp->Update(deltaTime);
	}
}

//=================================================================
// アクタ更新
//=================================================================
void CActor::UpdateActor(float deltaTime) 
{
}

//=================================================================
// 入力処理
//=================================================================
void CActor::ProcessInput(const uint8_t* keyState) 
{
	if (m_eState == EActive) 
	{
		for (auto comp : m_pComponents) 
		{
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

//=================================================================
// 回転後の方向
//=================================================================
void CActor::RotateToNewForward(const Vector3& forward) 
{
	// オリジナル（ユニットX）と新規の違いを理解する
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	if (dot > 0.9999f) 
	{
		SetRotation(Quaternion::Identity);
	}
	else if (dot < -0.9999f) 
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 外積から軸を中心に回転
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

//=================================================================
// アクタ入力処理
//=================================================================
void CActor::ActorInput(const uint8_t* keyStage) 
{
}

//=================================================================
// ワールド座標更新
//=================================================================
void CActor::ComputeWorldTransform() 
{
	if (m_bRecomputeWorldTransform) 
	{
		m_bRecomputeWorldTransform = false;
		// スケーリング→回転→移動
		m_matWorldTransform = Matrix4::CreateScale(m_fScale);
		m_matWorldTransform *= Matrix4::CreateFromQuaternion(m_qRotation);
		m_matWorldTransform *= Matrix4::CreateTranslation(m_vecPosition);

		// コンポーネントワールド座標更新
		for (auto comp : m_pComponents) 
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

//=================================================================
// コンポーネント追加
//=================================================================
void CActor::AddComponent(CComponent* pComponent) 
{
	int myOrder = pComponent->GetUpdateOrder();
	auto iter = m_pComponents.begin();
	for (; iter != m_pComponents.end(); ++iter) 
	{
		if (myOrder < (*iter)->GetUpdateOrder()) 
		{
			break;
		}
	}
	m_pComponents.insert(iter, pComponent);
}

//=================================================================
// コンポーネント削除
//=================================================================
void CActor::RemoveComponent(CComponent* pComponent) 
{
	auto iter = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);
	if (iter != m_pComponents.end()) 
	{
		m_pComponents.erase(iter);
	}
}