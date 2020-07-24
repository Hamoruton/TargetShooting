#include "AudioComponent.h"
#include "Actor.h"
#include "Game.h"
#include "AudioSystem.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CAudioComponent::CAudioComponent(CActor* owner, int updateOrder)
	:CComponent(owner, updateOrder) 
{
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CAudioComponent::~CAudioComponent() 
{
	StopAllEvents();
}

//=================================================================
// �X�V
//=================================================================
void CAudioComponent::Update(float deltaTime) 
{
	CComponent::Update(deltaTime);

	// ������2D�C�x���g���폜����
	auto iter = m_pEvents2D.begin();
	while (iter != m_pEvents2D.end()) 
	{
		if (!iter->IsValid()) 
		{
			iter = m_pEvents2D.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// ������3D�C�x���g���폜����
	iter = m_pEvents3D.begin();
	while (iter != m_pEvents3D.end()) 
	{
		if (!iter->IsValid()) 
		{
			iter = m_pEvents3D.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

//=================================================================
// �T�E���h���̃��[���h���W�X�V
//=================================================================
void CAudioComponent::OnUpdateWorldTransform() 
{
	Matrix4 world = m_pOwner->GetWorldTransform();
	for (auto& event : m_pEvents3D) 
	{
		if (event.IsValid()) 
		{
			event.Set3DAttributes(world);
		}
	}
}

//=================================================================
// �T�E���h�v���C
//=================================================================
CSoundEvent CAudioComponent::PlayEvent(const std::string& name) 
{
	CSoundEvent e = m_pOwner->GetGame()->GetAudioSystem()->PlayEvent(name);
	// 2D / 3D
	if (e.Is3D()) 
	{
		m_pEvents3D.emplace_back(e);
		e.Set3DAttributes(m_pOwner->GetWorldTransform());
	}
	else
	{
		m_pEvents2D.emplace_back(e);
	}
	return e;
}

//=================================================================
// �S�ẴT�E���h���~
//=================================================================
void CAudioComponent::StopAllEvents() 
{
	for (auto& e : m_pEvents2D) 
	{
		e.Stop();
	}
	for (auto& e : m_pEvents3D) 
	{
		e.Stop();
	}

	// �C�x���g�N���A
	m_pEvents2D.clear();
	m_pEvents3D.clear();
}