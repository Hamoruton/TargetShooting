#ifndef __SHADER_H_INCLUDED__
#define __SHADER_H_INCLUDED__
#pragma once
#include <GL/glew.h>
#include <string>

#include "Math.h"

class CShader
{
public:
	CShader();
	virtual ~CShader();

	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();

	void SetActive();
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetVectorUniform(const char* name, const Vector3& vector);
	void SetFloatUniform(const char* name, float value);

private:
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	bool IsCompiled(GLuint shader);
	bool IsValidProgram();

private:
	GLuint	m_glVertexShader;
	GLuint	m_glFragShader;
	GLuint	m_glShaderProgram;
};

#endif // !__SHADER_H_INCLUDED__