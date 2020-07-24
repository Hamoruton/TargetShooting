#include "OrbitActor.h"
#include "OrbitCamera.h"
#include "Game.h"
#include "Renderer.h"
#include "MoveComponent.h"
#include "MeshComponent.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
COrbitActor::COrbitActor(CGame* pGame)
	:CActor(pGame) 
{
	m_pMeshComponent = new CMeshComponent(this);
	m_pMeshComponent->SetMesh(pGame->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	SetPosition(Vector3(-500.0f, 100.0f, -100.0f));

	m_pCameraComponent = new COrbitCamera(this);
}

//=================================================================
// ���͏���
//=================================================================
void COrbitActor::ActorInput(const uint8_t* keys) 
{
	// �}�E�X����
	int x, y;
	Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);

	// �E�N���b�N��ێ����Ă���ꍇ�ɂ̂݉�]��K�p����
	if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) 
	{
		const int maxMouseSpeed = 500;
		const float maxOrbitSpeed = Math::Pi * 8;
		// ���E�v�Z
		float yawSpeed = 0.0f;
		if (x != 0) 
		{
			yawSpeed = static_cast<float>(x) / maxMouseSpeed;
			yawSpeed *= maxOrbitSpeed;
		}
		m_pCameraComponent->SetYawSpeed(-yawSpeed);

		// �s�b�`�v�Z
		float pitchSpeed = 0.0f;
		if (y != 0) 
		{
			pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
			pitchSpeed *= maxOrbitSpeed;
		}
		m_pCameraComponent->SetPitchSpeed(pitchSpeed);
	}
}

//=================================================================
// ���o�ϐ��ݒ�
//=================================================================
void COrbitActor::SetVisible(bool visible) 
{
	m_pMeshComponent->SetVisible(visible);
}