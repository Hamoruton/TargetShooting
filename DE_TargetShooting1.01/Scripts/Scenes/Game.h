#pragma once
#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "SDL.h"
#include "Math.h"
#include "SoundEvent.h"

class CGame
{
public:
	CGame();
	virtual ~CGame();

	bool Initialize();
	void RunLoop();

	void AddActor(class CActor* actor);
	void RemoveActor(class CActor* actor);

	class CRenderer* GetRenderer() { return m_pRenderer; }
	class CAudioSystem* GetAudioSystem() { return m_pAudioSystem; }
	class CPhysWorld* GetPhysWorld() { return m_pPhysWorld; }
	class CHUD* GetHUD() { return m_pHUD; }

	const std::vector<class CUIScreen*>& GetUIStack() { return m_pUIStack; }
	void PushUI(class CUIScreen* screen);

	class CFPSActor* GetPlayer() { return m_pFPSActor; }

	enum GameState 
	{
		ETitle,
		EGameplay,
		EPaused,
		EGameclear,
		EQuit
	};

	GameState GetState() const { return m_sGameState; }
	void SetState(GameState state) { m_sGameState = state; }

	class CFont* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

	void LoadData();
	void UnloadData();

	void AddPlane(class CPlaneActor* plane);
	void RemovePlane(class CPlaneActor* plane);
	std::vector<class CPlaneActor*>& GetPlanes() { return m_pPlanes; }
	void AddWall(class CWallActor* wall);
	void RemoveWall(class CWallActor* wall);
	std::vector<class CWallActor*>& GetWalls() { return m_pWalls; }

	bool GetStartLoadData() const { return m_bStartLoadData; }
	void SetStartLoadData(bool value) { m_bStartLoadData = value; }

private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();

	// 全てのアクタ
	std::vector<class CActor*>						m_pActors;
	std::vector<class CActor*>						m_pPendingActors;
	std::vector<class CUIScreen*>					m_pUIStack;
	std::unordered_map < std::string, std::string>	m_pText;
	std::unordered_map<std::string, class CFont*>	m_pFonts;

	class CRenderer*		m_pRenderer;
	class CAudioSystem*		m_pAudioSystem;
	class CPhysWorld*		m_pPhysWorld;
	class CHUD*				m_pHUD;

	Uint32				m_iTicksCount;
	GameState			m_sGameState;
	bool				m_bUpdatingActors;

	// ゲームオブジェクト
	std::vector<class CPlaneActor*>	m_pPlanes;
	std::vector<class CWallActor*>	m_pWalls;
	class CFPSActor*				m_pFPSActor;
	class CSpriteComponent*			m_pCrosshair;
	CSoundEvent						m_cMusicEvent;

	//　データの読み込み
	bool		m_bStartLoadData;
};


#endif // !__GAME_H_INCLUDED__