#include "SplineActor.h"
#include "SplineCamera.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "MoveComponent.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CSplineActor::CSplineActor(CGame* pGame) 
	:CActor(pGame)
{
	m_pCameraComponent = new CSplineCamera(this);

	// �X�v���C������
	Spline path;
	path.m_vControlPoints.emplace_back(Vector3::Zero);
	for (int i = 0; i < 5; i++) 
	{
		if (i % 2 == 0) 
		{
			path.m_vControlPoints.emplace_back(Vector3(300.0f*(i + 1), 300.0f, 300.0f));
		}
		else
		{
			path.m_vControlPoints.emplace_back(Vector3(300.0f*(i + 1), 0.0f, 0.0f));
		}
	}

	m_pCameraComponent->SetSpline(path);
	m_pCameraComponent->SetPaused(false);
}

//=================================================================
// ���͏���
//=================================================================
void CSplineActor::ActorInput(const uint8_t* keys) 
{
}

//=================================================================
// �ăX�^�[�g
//=================================================================
void CSplineActor::RestartSpline() 
{
	m_pCameraComponent->Restart();
}