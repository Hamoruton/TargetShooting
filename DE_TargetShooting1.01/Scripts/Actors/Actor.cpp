#include "Actor.h"
#include "Game.h"
#include "Component.h"

//=================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
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
// �X�V
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
// �R���|�[�l���g�X�V
//=================================================================
void CActor::UpdateComponents(float deltaTime) 
{
	for (auto comp : m_pComponents) 
	{
		comp->Update(deltaTime);
	}
}

//=================================================================
// �A�N�^�X�V
//=================================================================
void CActor::UpdateActor(float deltaTime) 
{
}

//=================================================================
// ���͏���
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
// ��]��̕���
//=================================================================
void CActor::RotateToNewForward(const Vector3& forward) 
{
	// �I���W�i���i���j�b�gX�j�ƐV�K�̈Ⴂ�𗝉�����
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
		// �O�ς��玲�𒆐S�ɉ�]
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

//=================================================================
// �A�N�^���͏���
//=================================================================
void CActor::ActorInput(const uint8_t* keyStage) 
{
}

//=================================================================
// ���[���h���W�X�V
//=================================================================
void CActor::ComputeWorldTransform() 
{
	if (m_bRecomputeWorldTransform) 
	{
		m_bRecomputeWorldTransform = false;
		// �X�P�[�����O����]���ړ�
		m_matWorldTransform = Matrix4::CreateScale(m_fScale);
		m_matWorldTransform *= Matrix4::CreateFromQuaternion(m_qRotation);
		m_matWorldTransform *= Matrix4::CreateTranslation(m_vecPosition);

		// �R���|�[�l���g���[���h���W�X�V
		for (auto comp : m_pComponents) 
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

//=================================================================
// �R���|�[�l���g�ǉ�
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
// �R���|�[�l���g�폜
//=================================================================
void CActor::RemoveComponent(CComponent* pComponent) 
{
	auto iter = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);
	if (iter != m_pComponents.end()) 
	{
		m_pComponents.erase(iter);
	}
}