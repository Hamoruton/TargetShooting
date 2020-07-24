#include <fstream>
#include <sstream>
#include <SDL.h>

#include "Shader.h"


//=================================================================
// コンストラクタ
//=================================================================
CShader::CShader()
	:m_glShaderProgram(0)
	, m_glVertexShader(0)
	, m_glFragShader(0)
{
}

//=================================================================
// デストラクタ
//=================================================================
CShader::~CShader() 
{
}

//=================================================================
// データの読み込み
//=================================================================
bool CShader::Load(const std::string& vertName, const std::string& fragName) 
{
	// 頂点シェーダーとピクセルシェーダーのコンパイル
	if (!CompileShader(vertName, GL_VERTEX_SHADER, m_glVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, m_glFragShader)) 
	{
		return false;
	}

	// シェーダーをシェーダープログラムにリンク
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
// データの削除
//=================================================================
void CShader::Unload() 
{
	glDeleteProgram(m_glShaderProgram);
	glDeleteShader(m_glVertexShader);
	glDeleteShader(m_glFragShader);
}

//=================================================================
// シェーダープログラムをアクティブにセット
//=================================================================
void CShader::SetActive() 
{
	glUseProgram(m_glShaderProgram);
}

//=================================================================
// シェーダープログラムを行列座標にセット
//=================================================================
void CShader::SetMatrixUniform(const char* name, const Matrix4& matrix) 
{
	GLuint loc = glGetUniformLocation(m_glShaderProgram, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

//=================================================================
// シェーダープログラムをベクトル座標にセット
//=================================================================
void CShader::SetVectorUniform(const char* name, const Vector3& vector) 
{
	GLuint loc = glGetUniformLocation(m_glShaderProgram, name);
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

//=================================================================
// シェーダープログラムを変数にセット
//=================================================================
void CShader::SetFloatUniform(const char* name, float value) 
{
	GLuint loc = glGetUniformLocation(m_glShaderProgram, name);
	glUniform1f(loc, value);
}

//=================================================================
// シェーダーコンパイル
//=================================================================
bool CShader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) 
{
	// ファイルを開く
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open()) 
	{
		// テクストを文字列に読み込む
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// シェーダーを生成
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
// シェーダーコンパイル判定
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
// シェーダープログラムのリンク判定
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
