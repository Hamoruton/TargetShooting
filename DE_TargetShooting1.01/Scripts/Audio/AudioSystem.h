#ifndef __AUDIOSYSTEM_H_INCLUDED__
#define __AUDIOSYSTEM_H_INCLUDED__

#pragma once
#include <unordered_map>
#include <string>

#include "SoundEvent.h"
#include "Math.h"

// FMODヘッダーを含めないように宣言を転送する
namespace FMOD 
{
	class System;
	namespace Studio 
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	}
}

class CAudioSystem
{
public:
	CAudioSystem(class CGame* pGame);
	virtual ~CAudioSystem();

	bool Initialize();
	void Shutdown();

	// データのロード/アンロード
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	CSoundEvent PlayEvent(const std::string& name);

	void Update(float deltaTime);

	void SetListener(const Matrix4& viewMatrix);

	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);

protected:
	friend class CSoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

private:
	static unsigned int		m_iNextID;

	class	CGame*			m_pGame;

	std::unordered_map<std::string, FMOD::Studio::Bank*>				m_pBanks;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*>	m_pEvents;
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*>		m_pEventInstances;
	std::unordered_map<std::string, FMOD::Studio::Bus*>					m_pBuses;

	FMOD::Studio::System*	m_pSystem;
	FMOD::System*			m_pLowLevelSystem;
};

#endif // !__AUDIOSYSTEM_H_INCLUDED__