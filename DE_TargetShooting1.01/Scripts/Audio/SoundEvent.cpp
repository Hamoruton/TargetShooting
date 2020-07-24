#include "SoundEvent.h"
#include "AudioSystem.h"
#include <fmod_studio.hpp>

//=================================================================
// �R���X�g���N�^
//=================================================================
CSoundEvent::CSoundEvent(class CAudioSystem* pSystem, unsigned int id)
	:m_pSystem(pSystem)
	, m_iID(id) 
{
}

CSoundEvent::CSoundEvent()
	: m_pSystem(nullptr)
	, m_iID(0)
{
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CSoundEvent::~CSoundEvent() 
{
}

//=================================================================
// FMOD�C�x���g�͗L�����ǂ���
//=================================================================
bool CSoundEvent::IsValid() 
{
	return(m_pSystem&&m_pSystem->GetEventInstance(m_iID) != nullptr);
}

//=================================================================
// �ăX�^�[�g
//=================================================================
void CSoundEvent::Restart() 
{
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		event->start();
	}
}

//=================================================================
// ��~
//=================================================================
void CSoundEvent::Stop(bool allowFadeOut) 
{
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		FMOD_STUDIO_STOP_MODE mode = allowFadeOut ?
			FMOD_STUDIO_STOP_ALLOWFADEOUT :
			FMOD_STUDIO_STOP_IMMEDIATE;
		event->stop(mode);
	}
}

//=================================================================
// �ꎞ��~
//=================================================================
void CSoundEvent::SetPaused(bool pause) 
{
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		event->setPaused(pause);
	}
}

bool CSoundEvent::GetPaused() const 
{
	bool retVal = false;
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		event->getPaused(&retVal);
	}
	return retVal;
}

//=================================================================
// �傫��
//=================================================================
void CSoundEvent::SetVolume(float value) 
{
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		event->setVolume(value);
	}
}

float CSoundEvent::GetVolume() const
{
	float retVal = 0.0f;
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event)
	{
		event->getVolume(&retVal);
	}
	return retVal;
}


//=================================================================
// �͈�
//=================================================================
void CSoundEvent::SetPitch(float value) 
{
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		event->setPitch(value);
	}
}

float CSoundEvent::GetPitch() const
{
	float retVal = 0.0f;
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event)
	{
		event->getPitch(&retVal);
	}
	return retVal;
}

//=================================================================
// �p�����[�^
//=================================================================
void CSoundEvent::SetParameter(const std::string& name, float value) 
{
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		event->setParameterValue(name.c_str(), value);
	}
}

float CSoundEvent::GetParameter(const std::string& name)
{
	float retVal = 0.0f;
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event)
	{
		event->getParameterValue(name.c_str(), &retVal);
	}
	return retVal;
}

//=================================================================
// 3D��Ԃł̃T�E���h�ݒ�
//=================================================================
bool CSoundEvent::Is3D() const 
{
	bool retVal = false;
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		FMOD::Studio::EventDescription* ed = nullptr;
		event->getDescription(&ed);
		if (ed) 
		{
			ed->is3D(&retVal);
		}
	}
	return retVal;
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void CSoundEvent::Set3DAttributes(const Matrix4& worldTrans) 
{
	auto event = m_pSystem ? m_pSystem->GetEventInstance(m_iID) : nullptr;
	if (event) 
	{
		FMOD_3D_ATTRIBUTES attr;
		attr.position = VecToFMOD(worldTrans.GetTranslation());
		attr.forward = VecToFMOD(worldTrans.GetXAxis());
		attr.up = VecToFMOD(worldTrans.GetZAxis());
		attr.velocity = { 0.0f,0.0f,0.0f };
		event->set3DAttributes(&attr);
	}
}