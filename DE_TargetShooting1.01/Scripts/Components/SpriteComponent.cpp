#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

//=================================================================
// コンストラクタ
//=================================================================
CSpriteComponent::CSpriteComponent(CActor* owner, int drawOrder)
	:CComponent(owner)
	, m_pTexture(nullptr)
	, m_iDrawOrder(drawOrder)
	, m_iTexWidth(0)
	, m_iTexHeight(0)
	, m_bVisible(true)
{
	m_pOwner->GetGame()->GetRenderer()->AddSprite(this);
}

//=================================================================
// デストラクタ
//=================================================================
CSpriteComponent::~CSpriteComponent() 
{
	m_pOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

//=================================================================
// 描画
//=================================================================
void CSpriteComponent::Draw(CShader* pShader) 
{
	if (m_pTexture) 
	{
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(m_iTexWidth),
			static_cast<float>(m_iTexHeight),
			1.0f
		);

		Matrix4 world = scaleMat * m_pOwner->GetWorldTransform();

		pShader->SetMatrixUniform("uWorldTransform", world);
		m_pTexture->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

//=================================================================
// テクスチャセット
//=================================================================
void CSpriteComponent::SetTexture(CTexture* pTexture) 
{
	m_pTexture = pTexture;
	m_iTexWidth = pTexture->GetWidth();
	m_iTexHeight = pTexture->GetHeight();
}