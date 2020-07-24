#include <fstream>
#include <sstream>
#include <SDL.h>

#include "Shader.h"


//=================================================================
// �R���X�g���N�^
//=================================================================
CShader::CShader()
	:m_glShaderProgram(0)
	, m_glVertexShader(0)
	, m_glFragShader(0)
{
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CShader::~CShader() 
{
}

//=================================================================
// �f�[�^�̓ǂݍ���
//=================================================================
bool CShader::Load(const std::string& vertName, const std::string& fragName) 
{
	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�̃R���p�C��
	if (!CompileShader(vertName, GL_VERTEX_SHADER, m_glVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, m_glFragShader)) 
	{
		return false;
	}

	// �V�F�[�_�[���V�F�[�_�[�v���O�����Ƀ����N
	m_glShaderProgram = glCreateProgram();
	glAttachShader(m_glShaderProgram, m_glVertexShader);
	glAttachShader(m_glShaderProgram, m_glFragShader);
	glLinkProgram(m_glShaderProgram);

	if (!IsValidProgram()) 
	{
		return false;
	}
	return true;
}

//=================================================================
// �f�[�^�̍폜
//=================================================================
void CShader::Unload() 
{
	glDeleteProgram(m_glShaderProgram);
	glDeleteShader(m_glVertexShader);
	glDeleteShader(m_glFragShader);
}

//=================================================================
// �V�F�[�_�[�v���O�������A�N�e�B�u�ɃZ�b�g
//=================================================================
void CShader::SetActive() 
{
	glUseProgram(m_glShaderProgram);
}

//=================================================================
// �V�F�[�_�[�v���O�������s����W�ɃZ�b�g
//=================================================================
void CShader::SetMatrixUniform(const char* name, const Matrix4& matrix) 
{
	GLuint loc = glGetUniformLocation(m_glShaderProgram, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

//=================================================================
// �V�F�[�_�[�v���O�������x�N�g�����W�ɃZ�b�g
//=================================================================
void CShader::SetVectorUniform(const char* name, const Vector3& vector) 
{
	GLuint loc = glGetUniformLocation(m_glShaderProgram, name);
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

//=================================================================
// �V�F�[�_�[�v���O������ϐ��ɃZ�b�g
//=================================================================
void CShader::SetFloatUniform(const char* name, float value) 
{
	GLuint loc = glGetUniformLocation(m_glShaderProgram, name);
	glUniform1f(loc, value);
}

//=================================================================
// �V�F�[�_�[�R���p�C��
//=================================================================
bool CShader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) 
{
	// �t�@�C�����J��
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open()) 
	{
		// �e�N�X�g�𕶎���ɓǂݍ���
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// �V�F�[�_�[�𐶐�
		outShader = glCreateShader(shaderType);
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader)) 
		{
			SDL_Log("Faild to comile shader %s : ", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found : %s", fileName.c_str());
		return false;
	}

	return true;
}

//=================================================================
// �V�F�[�_�[�R���p�C������
//=================================================================
bool CShader::IsCompiled(GLuint shader) 
{
	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) 
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed : %s", buffer);
		return false;
	}

	return true;
}

//=================================================================
// �V�F�[�_�[�v���O�����̃����N����
//=================================================================
bool CShader::IsValidProgram() 
{
	GLint status;

	glGetProgramiv(m_glShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) 
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(m_glShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status : %s", buffer);
		return false;
	}

	return true;
}
