#include "TargetActor.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "TargetComponent.h"

//=================================================================
// コンストラクタ
//=================================================================
CTargetActor::CTargetActor(CGame* pGame)
	:CActor(pGame) 
{
	SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	CMeshComponent* mc = new CMeshComponent(this);
	CMesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
	mc->SetMesh(mesh);

	CBoxComponent* bc = new CBoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());
	new CTargetComponent(this);
}