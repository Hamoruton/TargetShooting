#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"


//=================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=================================================================
CMeshComponent::~CMeshComponent() 
{
	m_pOwner->GetGame()->GetRenderer()->RemoveMeshComponent(this);
}

//=================================================================
// �����_�����O
//=================================================================
void CMeshComponent::Draw(CShader* pShader) 
{
	// ���[���h���W
	pShader->SetMatrixUniform("uWorldTransform", m_pOwner->GetWorldTransform());

	// ���ʔ��˗�
	pShader->SetFloatUniform("uSpecPower", m_pMesh->GetSpecPower());

	// �e�N�X�`��
	CTexture* tex = m_pMesh->GetTexture(m_szTextureIndex);
	if (tex) 
	{
		tex->SetActive();
	}

	// ���b�V���̒��_�z��f�[�^
	CVertexArray* va = m_pMesh->GetVertexArray();
	va->SetActive();

	// �����_�����O
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}