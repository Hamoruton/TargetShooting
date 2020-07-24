#ifndef __VERTEXARRAY_H_INCLUDED__
#define __VERTEXARRAY_H_INCLUDED__

#pragma once
class CVertexArray
{
public:
	CVertexArray(const float* verts, unsigned int numVerts, 
		const unsigned int* indices, unsigned int numIndices);
	virtual ~CVertexArray();

	void SetActive();

	unsigned int GetNumIndices() const { return m_iNumIndices; }
	unsigned int GetNumVerts() const { return m_iNumVerts; }

private:
	unsigned int	m_iNumVerts;
	unsigned int	m_iNumIndices;

	unsigned int	m_iVertexBuffer;
	unsigned int	m_iIndexBuffer;
	unsigned int	m_iVertexArray;
};

#endif // !__VERTEXARRAY_H_INCLUDED__
