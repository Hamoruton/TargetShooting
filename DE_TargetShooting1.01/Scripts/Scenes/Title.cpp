#include "Title.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "UIScreen.h"
#include <SDL.h>
#include "GL/glew.h"
#include "DialogBox.h"

//=================================================================
// コンストラクタ
//=================================================================
CTitle::CTitle(class CGame* pGame)
	:CUIScreen(pGame) 
{
	m_pGame->SetState(CGame::ETitle);
	SetRelativeMouseMode(false);
	m_pBackground =m_pGame->GetRenderer()->GetTexture("Assets/Background.jpg");
	SetTitlePos(Vector2(0.0f, 250.0f));
	SetTitle("Title", Color::Black, 60);
	SetNextButtonPos(Vector2(0.0f, -200.0f));
	AddButton("StartButton", [this]() {
		GameScene();
	});
	AddButton("QuitButton", [this]() {
		new CDialogBox(m_pGame, "QuitText",
			[this]() {
			m_pGame->SetState(CGame::EQuit);
		});
	});
}

//=================================================================
// デストラクタ
//=================================================================
CTitle::~CTitle() 
{
	SetRelativeMouseMode(true);
	m_pGame->SetState(CGame::EGameplay);
}
