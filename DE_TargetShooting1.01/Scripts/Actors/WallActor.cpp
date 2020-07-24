#include "WallActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

//=================================================================
// コンストラクタ
//=================================================================
CWallActor::CWallActor(CGame* pGame)
	:CActor(pGame) 
{
	SetScale(10.0f);
	CMeshComponent* mc = new CMeshComponent(this);
	CMesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Wall.gpmesh");
	mc->SetMesh(mesh);

	m_pBox = new CBoxComponent(this);
	m_pBox->SetObjectBox(mesh->GetBox());

	pGame->AddWall(this);
}

//=================================================================
// デストラクタ
//=================================================================
CWallActor::~CWallActor() 
{
	GetGame()->RemoveWall(this);
}