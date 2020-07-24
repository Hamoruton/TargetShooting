#ifndef __MESHCOMPONENT_H_INCLUDED__
#define __MESHCOMPONENT_H_INCLUDED__
#pragma once
#include "Component.h"
#include <cstddef>

class CMeshComponent :
	public CComponent
{
public:
	CMeshComponent(class CActor* owner);
	virtual ~CMeshComponent();

	virtual void Draw(class CShader* pShader);
	virtual void SetMesh(class CMesh* pMesh) { m_pMesh = pMesh; }
	void SetTextureIndex(size_t index) { m_szTextureIndex = index; }

	void SetVisible(bool visible) { m_bVisible = visible; }
	bool GetVisible() const { return m_bVisible; }

protected:
	class CMesh*	m_pMesh;
	size_t			m_szTextureIndex;
	bool			m_bVisible;
};

#endif // !__MESHCOMPONENT_H_INCLUDED__