#ifndef __SOUNDEVENT_H_INCLUDED__
#define __SOUNDEVENT_H_INCLUDED__

#pragma once
#include <string>
#include "Math.h"

class CSoundEvent
{
public:
	CSoundEvent();
	virtual ~CSoundEvent();

	bool IsValid();
	void Restart();
	void Stop(bool allowFadeOut = true);

	// �Z�b�^�[
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);

	// �Q�b�^�[
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);

	// �T�E���h�̈ʒu
	bool Is3D() const;
	void Set3DAttributes(const Matrix4& worldTrans);

protected:
	// AudioSystem�݂̂����̃R���X�g���N�^�ɃA�N�Z�X�\
	friend class CAudioSystem;
	CSoundEvent(class CAudioSystem* pSystem, unsigned int id);

private:
	class CAudioSystem*		m_pSystem;
	unsigned int			m_iID;
};

#endif // !__SOUNDEVENT_H_INCLUDED__