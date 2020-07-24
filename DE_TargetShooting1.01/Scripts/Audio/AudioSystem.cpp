#include "AudioSystem.h"
#include <SDL_log.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

unsigned int CAudioSystem::m_iNextID = 0;

//=================================================================
// �R���X�g���N�^
//=================================================================
CAudioSystem::CAudioSystem(CGame* pGame)
	:m_pGame(pGame)
	, m_pSystem(nullptr)
	, m_pLowLevelSystem(nullptr)
{
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CAudioSystem::~CAudioSystem() 
{
}

//=================================================================
// ������
//=================================================================
bool CAudioSystem::Initialize() 
{
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR,
		FMOD_DEBUG_MODE_TTY
	);

	// FMOD�X�^�W�I�V�X�e���I�u�W�F�N�g�̍쐬
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&m_pSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system : %s", FMOD_ErrorString(result));
		return false;
	}

	// FMOD������
	result = m_pSystem->initialize(
		512,							// �����T�E���h�̍ő吔
		FMOD_STUDIO_INIT_NORMAL,		// �f�t�H���g�ݒ���g�p
		FMOD_INIT_NORMAL,				// �f�t�H���g�ݒ���g�p
		nullptr
	);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system : %s", FMOD_ErrorString(result));
		return false;
	}

	m_pSystem->getLowLevelSystem(&m_pLowLevelSystem);

	// �f�[�^�̓ǂݍ���
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

//=================================================================
// �V���b�g�_�E��
//=================================================================
void CAudioSystem::Shutdown() 
{
	UnloadAllBanks();
	if (m_pSystem) 
	{
		m_pSystem->release();
	}
}

//=================================================================
// �f�[�^�̓ǂݍ���
//=================================================================
void CAudioSystem::LoadBank(const std::string& name) 
{
	if (m_pBanks.find(name) != m_pBanks.end()) 
	{
		return;
	}

	// Bank�̓ǂݍ���
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = m_pSystem->loadBankFile(
		name.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL,
		&bank
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK) 
	{
		m_pBanks.emplace(name, bank);
		bank->loadSampleData();

		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0) 
		{
			// Bank���X�g���擾
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++) 
			{
				FMOD::Studio::EventDescription* e = events[i];
				e->getPath(eventName, maxPathLength, nullptr);
				// �ǉ�
				m_pEvents.emplace(eventName, e);
			}
		}

		// Buses�̐����擾
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0) 
		{
			// Bank��Buses���X�g���擾
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++) 
			{
				FMOD::Studio::Bus* bus = buses[i];
				bus->getPath(busName, 512, nullptr);
				// �ǉ�
				m_pBuses.emplace(busName, bus);
			}
		}
	}
}

//=================================================================
// �f�[�^�̔j��
//=================================================================
void CAudioSystem::UnloadBank(const std::string& name) 
{
	auto iter = m_pBanks.find(name);
	if (iter == m_pBanks.end()) 
	{
		return;
	}

	// �o���N�����[�h
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = m_pSystem->loadBankFile(
		name.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL,
		&bank
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK) 
	{
		m_pBanks.emplace(name, bank);
		bank->loadSampleData();
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0) 
		{
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++) 
			{
				FMOD::Studio::EventDescription* e = events[i];
				e->getPath(eventName, maxPathLength, nullptr);
				m_pEvents.emplace(eventName, e);
			}
		}
	}

	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0) 
	{
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++) 
		{
			FMOD::Studio::Bus* bus = buses[i];
			bus->getPath(busName, 512, nullptr);
			auto busi = m_pBuses.find(busName);
			if (busi != m_pBuses.end()) 
			{
				m_pBuses.erase(busi);
			}
		}
	}
	// �T���v���f�[�^��Bank��j��
	bank->unloadSampleData();
	bank->unload();
	m_pBanks.erase(iter);
}

void CAudioSystem::UnloadAllBanks() 
{
	for (auto& iter : m_pBanks) 
	{
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	m_pBanks.clear();
	m_pEvents.clear();
}

//=================================================================
// �v���C�C�x���g
//=================================================================
CSoundEvent CAudioSystem::PlayEvent(const std::string& name) 
{
	unsigned int retID = 0;
	auto iter = m_pEvents.find(name);
	if (iter != m_pEvents.end()) 
	{
		// �C�x���g����
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event) 
		{
			event->start();

			// ����ID���擾���}�b�v�ɒǉ�
			m_iNextID++;
			retID = m_iNextID;
			m_pEventInstances.emplace(retID, event);
		}
	}
	return CSoundEvent(this, retID);
}

//=================================================================
// �X�V
//=================================================================
void CAudioSystem::Update(float delteTime) 
{
	std::vector<unsigned int> done;
	for (auto& iter : m_pEventInstances) 
	{
		FMOD::Studio::EventInstance* e = iter.second;
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED) 
		{
			e->release();
			done.emplace_back(iter.first);
		}
	}

	for (auto id : done) 
	{
		m_pEventInstances.erase(id);
	}

	// FMOD�X�V
	m_pSystem->update();
}

namespace 
{
	FMOD_VECTOR VecToFMOD(const Vector3& in) 
	{
		// ���W����ϊ�
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

//=================================================================
// �T�E���h�͈�
//=================================================================
void CAudioSystem::SetListener(const Matrix4& viewMatrix) 
{
	// �r���[�s��𔽓]���Đ������x�N�g�����擾����
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	// �ʒu��ݒ�
	listener.position = VecToFMOD(invView.GetTranslation());
	listener.forward = VecToFMOD(invView.GetZAxis());
	listener.up = VecToFMOD(invView.GetYAxis());
	listener.velocity = { 0.0f,0.0f,0.0f };
	// FMOD�𑗐M
	m_pSystem->setListenerAttributes(0, &listener);
}

//=================================================================
// �T�E���h�̑傫���擾
//=================================================================
float CAudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = m_pBuses.find(name);
	if (iter != m_pBuses.end()) 
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

//=================================================================
// �ꎞ��~�ϐ��擾
//=================================================================
bool CAudioSystem::GetBusPaused(const std::string& name) const 
{
	bool retVal = false;
	const auto iter = m_pBuses.find(name);
	if (iter != m_pBuses.end()) 
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

//=================================================================
// �T�E���h�̑傫����ݒ�
//=================================================================
void CAudioSystem::SetBusVolume(const std::string& name, float volume) 
{
	auto iter = m_pBuses.find(name);
	if (iter != m_pBuses.end()) 
	{
		iter->second->setVolume(volume);
	}
}

//=================================================================
// �ꎞ��~�ϐ��ݒ�
//=================================================================
void CAudioSystem::SetBusPaused(const std::string& name, bool pause) 
{
	auto iter = m_pBuses.find(name);
	if (iter != m_pBuses.end()) 
	{
		iter->second->setPaused(pause);
	}
}

//=================================================================
// �C�x���g�C���X�^���X�擾
//=================================================================
FMOD::Studio::EventInstance* CAudioSystem::GetEventInstance(unsigned int id) 
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = m_pEventInstances.find(id);
	if (iter != m_pEventInstances.end()) 
	{
		event = iter->second;
	}
	return event;
}