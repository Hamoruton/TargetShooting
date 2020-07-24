#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BoxComponent.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CPlaneActor::CPlaneActor(CGame* pGame)
	:CActor(pGame)
{
	SetScale(10.0f);
	CMeshComponent* mc = new CMeshComponent(this);
	CMesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);

	m_pBox = new CBoxComponent(this);
	m_pBox->SetObjectBox(mesh->GetBox());

	pGame->AddPlane(this);
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CPlaneActor::~CPlaneActor() 
{
	GetGame()->RemovePlane(this);
}


