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
// コンストラクタ
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
// 更新
//=================================================================
void CFPSActor::UpdateActor(float deltaTime) 
{
	CActor::UpdateActor(deltaTime);

	FixCollisions();

	// 足音サウンド
	m_fLastFootstep -= deltaTime;
	if (!Math::NearZero(m_pMoveComponent->GetForwardSpeed()) && m_fLastFootstep <= 0.0f) 
	{
		m_cFootstep.SetPaused(false);
		m_cFootstep.Restart();
		m_fLastFootstep = 0.5f;
	}

	// アクターの位置に対するFPSモデルの位置を更新
	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	m_pFPSModel->SetPosition(modelPos);

	// 回転初期化
	Quaternion q = GetRotation();

	// カメラからのピッチで回転
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), m_pCameraComponent->GetPitch()));
	m_pFPSModel->SetRotation(q);
}

//=================================================================
// 入力処理
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
	
	// マウス動作
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	const int maxMouseSpeed = 500;

	// 最高速度での回転/秒
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0) 
	{
		//[-1.0～1.0]に変換
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;

		// 回転/秒を掛ける
		angularSpeed *= maxAngularSpeed;
	}
	m_pMoveComponent->SetAngularSpeed(angularSpeed);

	// ピッチ計算
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0) 
	{
		//[-1.0～1.0]に変換
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	m_pCameraComponent->SetPitchSpeed(pitchSpeed);
}

//=================================================================
// ショット
//=================================================================
void CFPSActor::Shoot() 
{
	// 開始点を取得（画面の中央付近の平面）
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	Vector3 start = GetGame()->GetRenderer()->Unproject(screenPoint);

	// 終点を取得（画面の中央、近くと遠いの間）
	screenPoint.z = 0.9f;
	Vector3 end = GetGame()->GetRenderer()->Unproject(screenPoint);

	// 方向ベクトルを取得
	Vector3 dir = end - start;
	dir.Normalize();

	// ボールをスポーンする
	CBallActor* ball = new CBallActor(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(start + dir * 20.0f);
	// ボールを回転させて新しい方向を向く
	ball->RotateToNewForward(dir);
	// サウンド
	m_pAudioComponent->PlayEvent("event:/Shot");
}

//=================================================================
// 衝突判定
//=================================================================
void CFPSActor::FixCollisions() 
{
	// ワールドトランスフォームを再計算
	ComputeWorldTransform();

	const AABB& playerBox = m_pBoxComponent->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetWalls();
	for (auto pa : planes) 
	{
		// このPlaneActorと衝突しますか？
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			// すべての違いを計算する
			float dx1 = planeBox.m_sMax.x - playerBox.m_sMin.x;
			float dx2 = planeBox.m_sMin.x - playerBox.m_sMax.x;
			float dy1 = planeBox.m_sMax.y - playerBox.m_sMin.y;
			float dy2 = planeBox.m_sMin.y - playerBox.m_sMax.y;
			float dz1 = planeBox.m_sMax.z - playerBox.m_sMin.z;
			float dz2 = planeBox.m_sMin.z - playerBox.m_sMax.z;

			// dxをdx1 / dx2のより低いabsを持つ方に設定します
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
			// dyをdx1 / dx2のより低いabsを持つ方に設定します
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
			// dzをdx1 / dx2のより低いabsを持つ方に設定します
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

			// どちらが最も近いか、x / y位置を調整します
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

			// 位置を設定してボックスコンポーネントを更新
			SetPosition(pos);
			m_pBoxComponent->OnUpdateWorldTransform();
		}
	}
}

//=================================================================
// 足音サウンド
//=================================================================
void CFPSActor::SetFootstepSurface(float value) 
{
	m_cFootstep.SetPaused(true);
	m_cFootstep.SetParameter("Surface", value);
}

//=================================================================
// 視覚変数設定
//=================================================================
void CFPSActor::SetVisible(bool visible) 
{
	m_pMeshComponent->SetVisible(visible);
}