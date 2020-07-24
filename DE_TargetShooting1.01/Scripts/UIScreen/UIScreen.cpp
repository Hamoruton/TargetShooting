#include "UIScreen.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Font.h"
#include "Title.h"
#include "DialogBox.h"
#include "TargetActor.h"
#include "HUD.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CUIScreen::CUIScreen(CGame* pGame)
	:m_pGame(pGame)
	, m_pTitle(nullptr)
	, m_pBackground(nullptr)
	, m_vTitlePos(0.0f, 300.0f)
	, m_vNextButtonPos(0.0f, 200.0f)
	, m_vBGPos(0.0f, 0.0f)
	, m_sState(EActive)
{
	m_pGame->PushUI(this);
	m_pFont = m_pGame->GetFont("Assets/GN-KillGothic-U-KanaO.ttf");
	m_pButtonOn = m_pGame->GetRenderer()->GetTexture("Assets/ButtonYellow.png");
	m_pButtonOff = m_pGame->GetRenderer()->GetTexture("Assets/ButtonBlue.png");
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CUIScreen::~CUIScreen() 
{
	if (m_pTitle) 
	{
		m_pTitle->Unload();
		delete m_pTitle;
	}

	for (auto button : m_pButtons) 
	{
		delete button;
	}
	m_pButtons.clear();
}

//=================================================================
// �X�V
//=================================================================
void CUIScreen::Update(float deltaTime) 
{
}

//=================================================================
// �`��
//=================================================================
void CUIScreen::Draw(CShader* shader) 
{
	// �w�i
	if (m_pBackground) 
	{
		DrawTexture(shader, m_pBackground, m_vBGPos);
	}

	// �^�C�g��
	if (m_pTitle) 
	{
		DrawTexture(shader, m_pTitle, m_vTitlePos);
	}

	// �{�^��
	for (auto button : m_pButtons) 
	{
		CTexture* tex = button->GetHighlighted() ? m_pButtonOn : m_pButtonOff;
		DrawTexture(shader, tex, button->GetPosition());
		DrawTexture(shader, button->GetNameTex(), button->GetPosition());
	}
}

//=================================================================
// ���͏���
//=================================================================
void CUIScreen::ProcessInput(const uint8_t* keys) 
{
	if (!m_pButtons.empty()) 
	{
		// �}�E�X�̈ʒu���擾
		int x, y;
		SDL_GetMouseState(&x, &y);
		// (0,0�j���S���W�ɕϊ�
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= m_pGame->GetRenderer()->GetScreenWidth()*0.5f;
		mousePos.y = m_pGame->GetRenderer()->GetScreenHeight()*0.5f - mousePos.y;

		// �{�^���������\��
		for (auto button : m_pButtons) 
		{
			if (button->ContainsPoint(mousePos)) 
			{
				button->SetHighlighted(true);
			}
			else
			{
				button->SetHighlighted(false);
			}
		}
	}
}

void CUIScreen::HandleKeyPress(int key) 
{
	switch (key) 
	{
	case SDL_BUTTON_LEFT:
		if (!m_pButtons.empty()) 
		{
			for (auto button : m_pButtons) 
			{
				if (button->GetHighlighted()) 
				{
					button->OnClick();
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

//=================================================================
// UI�����
//=================================================================
void CUIScreen::Close() 
{
	m_sState = EClosing;
}

//=================================================================
// �^�C�g���V�[��
//=================================================================
void CUIScreen::TitleScene()
{
	m_sState = EClosing;
	new CTitle(m_pGame);
}

//=================================================================
// �Q�[���V�[��
//=================================================================
void CUIScreen::GameScene()
{
	m_sState = EClosing;
	bool flag = m_pGame->GetStartLoadData();
	if (flag) 
	{
		m_pGame->LoadData();
		m_pGame->SetStartLoadData(false);
	}
	else
	{
		CActor* actor = new CActor(m_pGame);

		// �^�[�Q�b�g�A�N�^
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(0.0f, -1450.0f, 200.0f));
		actor->SetRotation(Quaternion(Vector3::UnitZ, Math::PiOver2));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(0.0f, 1450.0f, 200.0f));
		actor->SetRotation(Quaternion(Vector3::UnitZ, -Math::PiOver2));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(-1450.0f, 0.0f, 100.0f));
		actor->SetRotation(Quaternion(Vector3::UnitZ, Math::TwoPi));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(-1450.0f, 0.0f, 400.0f));
		actor->SetRotation(Quaternion(Vector3::UnitZ, Math::TwoPi));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(-1450.0f, -500.0f, 200.0f));
		actor->SetRotation(Quaternion(Vector3::UnitZ, Math::TwoPi));
		actor = new CTargetActor(m_pGame);
		actor->SetPosition(Vector3(-1450.0f, 500.0f, 200.0f));
		actor->SetRotation(Quaternion(Vector3::UnitZ, Math::TwoPi));

		m_pGame->GetHUD()->SetTargetNum(10);
	}
}

//=================================================================
// �^�C�g���ݒ�
//=================================================================
void CUIScreen::SetTitle(const std::string& text,
						 const Vector3& color,
						 int pointSize) 
{
	// �ȑO�̃^�C�g���e�N�X�`�������݂���ꍇ�͂��������
	if (m_pTitle) 
	{
		m_pTitle->Unload();
		delete m_pTitle;
		m_pTitle = nullptr;
	}
	m_pTitle = m_pFont->RenderText(text, color, pointSize);
}

//=================================================================
// �{�^����ǉ�
//=================================================================
void CUIScreen::AddButton(const std::string& name, std::function<void()> onClick) 
{
	Vector2 dims(static_cast<float>(m_pButtonOn->GetWidth())
		, static_cast<float>(m_pButtonOn->GetHeight()));
	CButton* button = new CButton(name, m_pFont, onClick, m_vNextButtonPos, dims);
	m_pButtons.emplace_back(button);

	// ���{�^���̈ʒu���X�V
	// �{�^���ƃp�f�B���O�̍����ŉ��Ɉړ�
	m_vNextButtonPos.y -= m_pButtonOff->GetHeight() + 20.0f;
}

//=================================================================
// �e�N�X�`���`��
//=================================================================
void CUIScreen::DrawTexture(class CShader* shader, class CTexture* texture,
							const Vector2& offset, float scale) 
{
	// �e�N�X�`���̕�/�����ɂ���ăN���b�h���X�P�[�����O
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth())*scale,
		static_cast<float>(texture->GetHeight())*scale,
		1.0f
	);

	// ��ʏ�̈ʒu�ɕϊ�
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));

	// ���[���h���W�ɐݒ�
	Matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	texture->SetActive();

	// �N���b�h�`��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

//=================================================================
// �}�E�X���[�h
//=================================================================
void CUIScreen::SetRelativeMouseMode(bool relative) 
{
	if (relative) 
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////


//=================================================================
// �R���X�g���N�^
//=================================================================
CButton::CButton(const std::string& name, CFont* font,
				 std::function<void()> onClick,
				 const Vector2& pos, const Vector2& dims) 
	:m_OnClick(onClick)
	,m_pNameTex(nullptr)
	,m_pFont(font)
	,m_vPosition(pos)
	,m_vDimensions(dims)
	,m_bHighlighted(false)
{
	SetName(name);
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CButton::~CButton() 
{
	if (m_pNameTex) 
	{
		m_pNameTex->Unload();
		delete m_pNameTex;
	}
}

//=================================================================
// �{�^����ݒ�
//=================================================================
void CButton::SetName(const std::string& name) 
{
	m_sName = name;
	if (m_pNameTex) 
	{
		m_pNameTex->Unload();
		delete m_pNameTex;
		m_pNameTex = nullptr;
	}
	m_pNameTex = m_pFont->RenderText(m_sName);
}

//=================================================================
// �{�^�������i�����j
//=================================================================
bool CButton::ContainsPoint(const Vector2& pt) const 
{
	bool no = pt.x<(m_vPosition.x - m_vDimensions.x / 2.0f) ||
		pt.x>(m_vPosition.x + m_vDimensions.x / 2.0f) ||
		pt.y<(m_vPosition.y - m_vDimensions.y / 2.0f) ||
		pt.y>(m_vPosition.y + m_vDimensions.y / 2.0f);
	return !no;
}

//=================================================================
// �N���b�N�擾
//=================================================================
void CButton::OnClick() 
{
	if (m_OnClick) 
	{
		m_OnClick();
	}
}