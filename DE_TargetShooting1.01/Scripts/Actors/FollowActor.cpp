#include "FollowActor.h"
#include "FollowCamera.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "MoveComponent.h"

//=================================================================
// コンストラクタ
//=================================================================
CFollowActor::CFollowActor(CGame* pGame)
	:CActor(pGame)
{
	m_pMeshComponent = new CMeshComponent(this);
	m_pMeshComponent->SetMesh(pGame->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

	m_pMoveComponent = new CMoveComponent(this);
	m_pCameraComponent = new CFollowCamera(this);
	m_pCameraComponent->SnapToIdeal();
}

//=================================================================
// 入力処理
//=================================================================
void CFollowActor::ActorInput(const uint8_t* keys) 
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;

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
		angularSpeed -= Math::Pi;
	}
	if (keys[SDL_SCANCODE_D]) 
	{
		angularSpeed += Math::Pi;
	}

	m_pMoveComponent->SetForwardSpeed(forwardSpeed);
	m_pMoveComponent->SetAngularSpeed(angularSpeed);

	// 速度に基づいてカメラの水平距離を調整
	if (!Math::NearZero(forwardSpeed)) 
	{
		m_pCameraComponent->SetHorzDist(500.0f);
	}
	else
	{
		m_pCameraComponent->SetHorzDist(350.0f);
	}
}

//=================================================================
// 視覚変数設定
//=================================================================
void CFollowActor::SetVisible(bool visible) 
{
	m_pMeshComponent->SetVisible(visible);
}