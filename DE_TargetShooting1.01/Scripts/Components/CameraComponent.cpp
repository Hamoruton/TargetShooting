#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Game.h"
#include "AudioSystem.h"

//=================================================================
// コンストラクタ
//=================================================================
CCameraComponent::CCameraComponent(CActor* owner, int updateOrder)
	:CComponent(owner, updateOrder)
{
}

//=================================================================
// ビュートランスフォームを設定
//=================================================================
void CCameraComponent::SetViewMatrix(const Matrix4& view) 
{
	// ビューマトリックスをレンダラーとオーディオシステムに渡す
	CGame* pGame = m_pOwner->GetGame();
	pGame->GetRenderer()->SetViewMatrix(view);
	pGame->GetAudioSystem()->SetListener(view);
}
