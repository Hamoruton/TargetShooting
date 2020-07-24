#include "VertexArray.h"
#include <GL/glew.h>

//=================================================================
// �R���X�g���N�^
//=================================================================
CVertexArray::CVertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:m_iNumVerts(numVerts)
	, m_iNumIndices(numIndices)
{
	// ���_�z�񐶐�
	glGenVertexArrays(1, &m_iVertexArray);
	glBindVertexArray(m_iVertexArray);

	// ���_�o�b�t�@����
	glGenBuffers(1, &m_iVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 8 * sizeof(float), verts, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@����
	glGenBuffers(1, &m_iIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// ���_�������w��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 3));

	// �e�N�X�`�����W
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 6));
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CVertexArray::~CVertexArray() 
{
	glDeleteBuffers(1, &m_iVertexBuffer);
	glDeleteBuffers(1, &m_iIndexBuffer);
	glDeleteVertexArrays(1, &m_iVertexArray);
}

//=================================================================
// ���_�z����A�N�e�B�u�ɃZ�b�g
//=================================================================
void CVertexArray::SetActive() 
{
	glBindVertexArray(m_iVertexArray);
}