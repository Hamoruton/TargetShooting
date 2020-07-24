#ifndef __MESH_H_INCLUDED__
#define __MESH_H_INCLUDED__

#pragma once
#include <vector>
#include <string>
#include "Collision.h"

class CMesh
{
public:
	CMesh();
	virtual ~CMesh();

	bool Load(const std::string& fileName, class CRenderer* pRenderer);
	void Unload();

	class CVertexArray* GetVertexArray() { return m_pVertexArray; }
	class CTexture*		GetTexture(size_t index);
	const std::string& GetShaderName() const { return m_sShaderName; }
	float GetRadius() const { return m_fRadius; }
	float GetSpecPower() const { return m_fSpecPower; }
	const AABB& GetBox() const { return m_cBox; }

private:
	AABB						 m_cBox;
	std::vector<class CTexture*> m_pTextures;
	class CVertexArray*			 m_pVertexArray;
	std::string					 m_sShaderName;
	float						 m_fRadius;
	float						 m_fSpecPower;		// •\–Ê‚Ì‹¾–Ê”½ŽË—Í
};

#endif // !__MESH_H_INCLUDED__
