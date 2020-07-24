#include "VertexArray.h"
#include <GL/glew.h>

//=================================================================
// コンストラクタ
//=================================================================
CVertexArray::CVertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:m_iNumVerts(numVerts)
	, m_iNumIndices(numIndices)
{
	// 頂点配列生成
	glGenVertexArrays(1, &m_iVertexArray);
	glBindVertexArray(m_iVertexArray);

	// 頂点バッファ生成
	glGenBuffers(1, &m_iVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 8 * sizeof(float), verts, GL_STATIC_DRAW);

	// インデックスバッファ生成
	glGenBuffers(1, &m_iIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// 頂点属性を指定
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 3));

	// テクスチャ座標
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 6));
}

//=================================================================
// デストラクタ
//=================================================================
CVertexArray::~CVertexArray() 
{
	glDeleteBuffers(1, &m_iVertexBuffer);
	glDeleteBuffers(1, &m_iIndexBuffer);
	glDeleteVertexArrays(1, &m_iVertexArray);
}

//=================================================================
// 頂点配列をアクティブにセット
//=================================================================
void CVertexArray::SetActive() 
{
	glBindVertexArray(m_iVertexArray);
}