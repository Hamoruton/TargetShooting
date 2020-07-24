#ifndef __TEXTURE_H_INCLUDED__
#define __TEXTURE_H_INCLUDED__

#pragma once
#include <string>

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

	bool Load(const std::string& fileName);
	void Unload();
	void CreateFromSurface(struct SDL_Surface* surface);

	void SetActive();

	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }

private:
	unsigned int	m_iTextureID;
	int				m_iWidth;
	int				m_iHeight;
};

#endif // !__TEXTURE_H_INCLUDED__