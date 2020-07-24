#include "HUD.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include "PhysWorld.h"
#include "FPSActor.h"
#include "TargetComponent.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CHUD::CHUD(CGame* pGame)
	:CUIScreen(pGame)
	, m_fRadarRange(2000.0f)
	, m_fRadarRadius(92.0f)
	, m_bTargetEnemy(false)
	, m_pTargetNum(nullptr)
	, m_iTargetNum(10)
{
	CRenderer* r = m_pGame->GetRenderer();
	m_pHealtBar = r->GetTexture("Assets/HealthBar.png");
	m_pRadar = r->GetTexture("Assets/Radar.png");
	m_pCrosshair = r->GetTexture("Assets/Crosshair.png");
	m_pCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
	m_pBlipTex = r->GetTexture("Assets/Blip.png");
	m_pRadarArrow = r->GetTexture("Assets/RadarArrow.png");
	m_pTargetUI = r->GetTexture("Assets/TargetUI.png");

	m_sTargetNum = std::to_string(m_iTargetNum);
	SetTargetNum(m_sTargetNum.c_str());
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CHUD::~CHUD() 
{
}

//=================================================================
// �X�V
//=================================================================
void CHUD::Update(float deltaTime) 
{
	CUIScreen::Update(deltaTime);

	UpdateCrosshair(deltaTime);
	UpdateRadar(deltaTime);
}

//=================================================================
// �`��
//=================================================================
void CHUD::Draw(CShader* shader) 
{
	// �e�̏Ə�
	CTexture* cross = m_bTargetEnemy ? m_pCrosshairEnemy : m_pCrosshair;
	DrawTexture(shader, cross, Vector2::Zero, 2.0f);

	// �X�e�[�W�}�b�v
	const Vector2 radarPos(-360.0f, -240.0f);
	DrawTexture(shader, m_pRadar, radarPos, 1.0f);
	for (Vector2& blip : m_vBlips) 
	{
		DrawTexture(shader, m_pBlipTex, radarPos + blip, 1.0f);
	}
	DrawTexture(shader, m_pRadarArrow, radarPos);

	// �^�[�Q�b�gUI
	const Vector2 targetUIPos(380.0f, 330.0f);
	DrawTexture(shader, m_pTargetUI, targetUIPos);

	const Vector2 targetNumPos(450.0f, 328.0f);
	SetTargetNum(m_sTargetNum.c_str());
	DrawTexture(shader, m_pTargetNum, targetNumPos);
}

//=================================================================
// �^�[�Q�b�g�R���|�[�l���g�ǉ�
//=================================================================
void CHUD::AddTargetComponent(CTargetComponent* tc) 
{
	m_pTargetComponents.emplace_back(tc);
}

//=================================================================
// �^�[�Q�b�g�R���|�[�l���g�폜
//=================================================================
void CHUD::RemoveTargetComponent(CTargetComponent* tc) 
{
	auto iter = std::find(m_pTargetComponents.begin(), m_pTargetComponents.end(), tc);
	m_pTargetComponents.erase(iter);
}

//=================================================================
// �e�̏Ə��X�V
//=================================================================
void CHUD::UpdateCrosshair(float deltaTime) 
{
	m_bTargetEnemy = false;
	// ���������
	const float fAmiDist = 5000.0f;
	Vector3 start, dir;
	m_pGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * fAmiDist);

	CPhysWorld::CollisionInfo info;
	if (m_pGame->GetPhysWorld()->SegmentCast(l, info)) 
	{
		// �^�[�Q�b�g�Ƃ̓����蔻��
		for (auto tc : m_pTargetComponents) 
		{
			if (tc->GetOwner() == info.m_pActor) 
			{
				m_bTargetEnemy = true;
				break;
			}
		}
	}
}

//=================================================================
// �X�e�[�W�}�b�v
//=================================================================
void CHUD::UpdateRadar(float deltaTime) 
{
	// �Ō�̃t���[������u���b�v�ʒu���N���A
	m_vBlips.clear();

	// �v���C���[�̈ʒu�����[�_�[���W�ɕϊ����܂��ix�O���Az����j
	Vector3 playerPos = m_pGame->GetPlayer()->GetPosition();
	Vector2 playerPos2D(playerPos.y, playerPos.x);
	Vector3 playerForward = m_pGame->GetPlayer()->GetForward();
	Vector2 playerForward2D(playerForward.x, playerForward.y);

	// atan2���g�p���ă��[�_�[�̉�]���擾����
	float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	// 2D��]�s����쐬����
	Matrix3 rotMat = Matrix3::CreateRotation(angle);

	// �u���b�v�̈ʒu���擾����
	for (auto tc : m_pTargetComponents) 
	{
		Vector3 targetPos = tc->GetOwner()->GetPosition();
		Vector2 actorPos2D(targetPos.y, targetPos.x);

		// �v���C���[�ƃ^�[�Q�b�g�Ԃ̃x�N�g�����v�Z����
		Vector2 playerToTarget = actorPos2D - playerPos2D;

		// �͈͓����ǂ������m�F
		if (playerToTarget.LengthSq() <= (m_fRadarRange*m_fRadarRange)) 
		{
			// playerToTarget������̃I�t�Z�b�g�ɕϊ����܂�
			// ��ʏ�̃��[�_�[�̒��S
			Vector2 blipPos = playerToTarget;
			blipPos *= m_fRadarRadius / m_fRadarRange;

			// blipPos����]
			blipPos = Vector2::Transform(blipPos, rotMat);
			m_vBlips.emplace_back(blipPos);
		}
	}
}

//=================================================================
// �^�[�Q�b�g���ݒ�
//=================================================================
void CHUD::SetTargetNum(const std::string& tex,
	const Vector3& color,
	int pointSize) 
{
	if (m_pTargetNum)
	{
		m_pTargetNum->Unload();
		delete m_pTargetNum;
		m_pTargetNum = nullptr;
	}
	m_pTargetNum = m_pFont->RenderText(tex, color, pointSize);
}
