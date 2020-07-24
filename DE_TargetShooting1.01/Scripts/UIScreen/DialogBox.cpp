#include "DialogBox.h"
#include "Game.h"
#include "Renderer.h"
#include <SDL.h>

//=================================================================
// �R���X�g���N�^
//=================================================================
CDialogBox::CDialogBox(class CGame* pGame, const std::string& text,
	std::function<void()> onOK)
	:CUIScreen(pGame)
{
	// �_�C�A���O�{�b�N�X�̈ʒu�𒲐�����
	m_vBGPos = Vector2(0.0f, 0.0f);
	m_vTitlePos = Vector2(0.0f, 100.0f);
	m_vNextButtonPos = Vector2(0.0f, 0.0f);

	m_pBackground = m_pGame->GetRenderer()->GetTexture("Assets/DialogBG.png");
	SetTitle(text, Vector3::Zero, 30);
	AddButton("OKButton", [onOK]() {
		onOK();
	});
	AddButton("CancelButton", [this](){
		Close();
	});
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CDialogBox::~CDialogBox() 
{
}