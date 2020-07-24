#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"


//=================================================================
// コンストラクタ
//=================================================================
CMeshComponent::CMeshComponent(CActor* owner)
	:CComponent(owner)
	, m_pMesh(nullptr)
	, m_szTextureIndex(0)
	, m_bVisible(true)
{
	m_pOwner->GetGame()->GetRenderer()->AddMeshComponent(this);
}

//=================================================================
// デストラクタ
//=================================================================
CMeshComponent::~CMeshComponent() 
{
	m_pOwner->GetGame()->GetRenderer()->RemoveMeshComponent(this);
}

//=================================================================
// レンダリング
//=================================================================
void CMeshComponent::Draw(CShader* pShader) 
{
	// ワールド座標
	pShader->SetMatrixUniform("uWorldTransform", m_pOwner->GetWorldTransform());

	// 鏡面反射力
	pShader->SetFloatUniform("uSpecPower", m_pMesh->GetSpecPower());

	// テクスチャ
	CTexture* tex = m_pMesh->GetTexture(m_szTextureIndex);
	if (tex) 
	{
		tex->SetActive();
	}

	// メッシュの頂点配列データ
	CVertexArray* va = m_pMesh->GetVertexArray();
	va->SetActive();

	// レンダリング
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}