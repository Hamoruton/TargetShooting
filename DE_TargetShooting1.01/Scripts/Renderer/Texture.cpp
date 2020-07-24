#include "Texture.h"

#include <soil.h>
#include <GL/glew.h>
#include <SDL.h>

//=================================================================
// コンストラクタ
//=================================================================
CTexture::CTexture()
	:m_iTextureID(0)
	, m_iWidth(0)
	, m_iHeight(0)
{
}

//=================================================================
// デストラクタ
//=================================================================
CTexture::~CTexture() 
{
}

//=================================================================
// データの読み込み
//=================================================================
bool CTexture::Load(const std::string& fileName) 
{
	int channels = 0;

	unsigned char* image = SOIL_load_image(fileName.c_str(), &m_iWidth, &m_iHeight, &channels, SOIL_LOAD_AUTO);
	if (image == nullptr) 
	{
		SDL_Log("SOIL failed to load image %s : %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4) 
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &m_iTextureID);
	glBindTexture(GL_TEXTURE_2D, m_iTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// 線形フィルタリングを有効にする
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

//=================================================================
// データの削除
//=================================================================
void CTexture::Unload() 
{
	glDeleteTextures(1, &m_iTextureID);
}

//=================================================================
// フォント用テクスチャ生成
//=================================================================
void CTexture::CreateFromSurface(struct SDL_Surface* surface) 
{
	m_iWidth = surface->w;
	m_iHeight = surface->h;

	// OpenGLテクスチャ生成
	glGenTextures(1, &m_iTextureID);
	glBindTexture(GL_TEXTURE_2D, m_iTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_BGRA,
		GL_UNSIGNED_BYTE, surface->pixels);

	// 線形フィルタリングを使用する
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//=================================================================
// テクスチャをアクティブにセット
//=================================================================
void CTexture::SetActive() 
{
	glBindTexture(GL_TEXTURE_2D, m_iTextureID);
}
	
