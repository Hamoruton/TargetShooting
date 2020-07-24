#include "AudioComponent.h"
#include "Actor.h"
#include "Game.h"
#include "AudioSystem.h"

//=================================================================
// コンストラクタ
//=================================================================
CAudioComponent::CAudioComponent(CActor* owner, int updateOrder)
	:CComponent(owner, updateOrder) 
{
}

//=================================================================
// デストラクタ
//=================================================================
CAudioComponent::~CAudioComponent() 
{
	StopAllEvents();
}

//=================================================================
// 更新
//=================================================================
void CAudioComponent::Update(float deltaTime) 
{
	CComponent::Update(deltaTime);

	// 無効な2Dイベントを削除する
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

	// 無効な3Dイベントを削除する
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
// サウンド時のワールド座標更新
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
// サウンドプレイ
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
// 全てのサウンドを停止
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

	// イベントクリア
	m_pEvents2D.clear();
	m_pEvents3D.clear();
}