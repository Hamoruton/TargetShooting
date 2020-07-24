#include "PauseMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL.h>

//=================================================================
// �R���X�g���N�^
//=================================================================
CPauseMenu::CPauseMenu(CGame* pGame)
	:CUIScreen(pGame) 
{
	m_pGame->SetState(CGame::EPaused);
	SetRelativeMouseMode(false);
	SetTitle("PauseTitle",Color::Black);
	AddButton("ResumeButton", [this](){
		Close();
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
CPauseMenu::~CPauseMenu() 
{
	SetRelativeMouseMode(true);
	m_pGame->SetState(CGame::EGameplay);
}

//=================================================================
// ���͏���
//=================================================================
void CPauseMenu::HandleKeyPress(int key) 
{
	CUIScreen::HandleKeyPress(key);

	if (key == SDLK_ESCAPE) 
	{
		Close();
	}
}