#include "GameClear.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Title.h"
#include "HUD.h"
#include "TargetActor.h"
#include "DialogBox.h"
#include <SDL.h>

//=================================================================
// �R���X�g���N�^
//=================================================================
CGameClear::CGameClear(class CGame* pGame)
	:CUIScreen(pGame) 
{
	m_pGame->SetState(CGame::EGameclear);
	SetRelativeMouseMode(false);
	m_pBackground = m_pGame->GetRenderer()->GetTexture("Assets/Background02.jpg");
	SetTitlePos(Vector2(0.0f, 200.0f));
	SetTitle("GameClear", Color::LightYellow, 60);
	SetNextButtonPos(Vector2(0.0f, -200.0f));
	AddButton("TitleButton", [this]() {
		TitleScene();
	});
	AddButton("QuitButton", [this]() {
		new CDialogBox(m_pGame, "QuitText",
			[this]() {
			m_pGame->SetState(CGame::EQuit);
		});
	});
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CGameClear::~CGameClear() 
{
	m_pGame->SetState(CGame::ETitle);
	LoadScene();
}

//=================================================================
// �Q�[���V�[�����ēǂݍ���
//=================================================================
void CGameClear::LoadScene() 
{
	
}
