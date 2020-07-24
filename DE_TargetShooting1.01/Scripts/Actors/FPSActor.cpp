#include "FPSActor.h"
#include "FPSCamera.h"
#include "BallActor.h"
#include "PlaneActor.h"
#include "WallActor.h"
#include "SDL_scancode.h"
#include "Game.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "AudioComponent.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "BoxComponent.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CFPSActor::CFPSActor(CGame* pGame)
	:CActor(pGame) 
{
	m_pMoveComponent = new CMoveComponent(this);
	m_pAudioComponent = new CAudioComponent(this);
	m_fLastFootstep = 0.0f;
	m_cFootstep = m_pAudioComponent->PlayEvent("event:/Footstep");
	m_cFootstep.SetPaused(true);

	m_pCameraComponent = new CFPSCamera(this);

	m_pFPSModel = new CActor(pGame);
	m_pFPSModel->SetScale(0.75f);
	m_pMeshComponent = new CMeshComponent(m_pFPSModel);
	m_pMeshComponent->SetMesh(pGame->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));

	m_pBoxComponent = new CBoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f), Vector3(25.0f, 25.0f, 87.5f));
	m_pBoxComponent->SetObjectBox(myBox);
	m_pBoxComponent->SetShouldRotate(false);
}

//=================================================================
// �X�V
//=================================================================
void CFPSActor::UpdateActor(float deltaTime) 
{
	CActor::UpdateActor(deltaTime);

	FixCollisions();

	// �����T�E���h
	m_fLastFootstep -= deltaTime;
	if (!Math::NearZero(m_pMoveComponent->GetForwardSpeed()) && m_fLastFootstep <= 0.0f) 
	{
		m_cFootstep.SetPaused(false);
		m_cFootstep.Restart();
		m_fLastFootstep = 0.5f;
	}

	// �A�N�^�[�̈ʒu�ɑ΂���FPS���f���̈ʒu���X�V
	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	m_pFPSModel->SetPosition(modelPos);

	// ��]������
	Quaternion q = GetRotation();

	// �J��������̃s�b�`�ŉ�]
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), m_pCameraComponent->GetPitch()));
	m_pFPSModel->SetRotation(q);
}

//=================================================================
// ���͏���
//=================================================================
void CFPSActor::ActorInput(const uint8_t* keys) 
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}

	m_pMoveComponent->SetForwardSpeed(forwardSpeed);
	m_pMoveComponent->SetStrafeSpeed(strafeSpeed);
	
	// �}�E�X����
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	const int maxMouseSpeed = 500;

	// �ō����x�ł̉�]/�b
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0) 
	{
		//[-1.0�`1.0]�ɕϊ�
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;

		// ��]/�b���|����
		angularSpeed *= maxAngularSpeed;
	}
	m_pMoveComponent->SetAngularSpeed(angularSpeed);

	// �s�b�`�v�Z
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0) 
	{
		//[-1.0�`1.0]�ɕϊ�
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	m_pCameraComponent->SetPitchSpeed(pitchSpeed);
}

//=================================================================
// �V���b�g
//=================================================================
void CFPSActor::Shoot() 
{
	// �J�n�_���擾�i��ʂ̒����t�߂̕��ʁj
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	Vector3 start = GetGame()->GetRenderer()->Unproject(screenPoint);

	// �I�_���擾�i��ʂ̒����A�߂��Ɖ����̊ԁj
	screenPoint.z = 0.9f;
	Vector3 end = GetGame()->GetRenderer()->Unproject(screenPoint);

	// �����x�N�g�����擾
	Vector3 dir = end - start;
	dir.Normalize();

	// �{�[�����X�|�[������
	CBallActor* ball = new CBallActor(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(start + dir * 20.0f);
	// �{�[������]�����ĐV��������������
	ball->RotateToNewForward(dir);
	// �T�E���h
	m_pAudioComponent->PlayEvent("event:/Shot");
}

//=================================================================
// �Փ˔���
//=================================================================
void CFPSActor::FixCollisions() 
{
	// ���[���h�g�����X�t�H�[�����Čv�Z
	ComputeWorldTransform();

	const AABB& playerBox = m_pBoxComponent->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetWalls();
	for (auto pa : planes) 
	{
		// ����PlaneActor�ƏՓ˂��܂����H
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			// ���ׂĂ̈Ⴂ���v�Z����
			float dx1 = planeBox.m_sMax.x - playerBox.m_sMin.x;
			float dx2 = planeBox.m_sMin.x - playerBox.m_sMax.x;
			float dy1 = planeBox.m_sMax.y - playerBox.m_sMin.y;
			float dy2 = planeBox.m_sMin.y - playerBox.m_sMax.y;
			float dz1 = planeBox.m_sMax.z - playerBox.m_sMin.z;
			float dz2 = planeBox.m_sMin.z - playerBox.m_sMax.z;

			// dx��dx1 / dx2�̂��Ⴂabs�������ɐݒ肵�܂�
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
			// dy��dx1 / dx2�̂��Ⴂabs�������ɐݒ肵�܂�
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
			// dz��dx1 / dx2�̂��Ⴂabs�������ɐݒ肵�܂�
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

			// �ǂ��炪�ł��߂����Ax / y�ʒu�𒲐����܂�
			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;
			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// �ʒu��ݒ肵�ă{�b�N�X�R���|�[�l���g���X�V
			SetPosition(pos);
			m_pBoxComponent->OnUpdateWorldTransform();
		}
	}
}

//=================================================================
// �����T�E���h
//=================================================================
void CFPSActor::SetFootstepSurface(float value) 
{
	m_cFootstep.SetPaused(true);
	m_cFootstep.SetParameter("Surface", value);
}

//=================================================================
// ���o�ϐ��ݒ�
//=================================================================
void CFPSActor::SetVisible(bool visible) 
{
	m_pMeshComponent->SetVisible(visible);
}