#ifndef __FONT_H_INCLUDED__
#define __FONT_H_INCLUDED__

#pragma once
#include <string>
#include <unordered_map>
#include <SDL_ttf.h>
#include "Math.h"

class CFont
{
public:
	CFont(class CGame* pGame);
	virtual ~CFont();

	// �ǂݍ���/�폜
	bool Load(const std::string& fileName);
	void Unload();

	// ������Ƃ��̃t�H���g���w�肵�āA�e�N�X�`���ɕ`��
	class CTexture* RenderText(const std::string& textKey, 
							   const Vector3& color = Color::White, 
							   int pointSize = 30);

private:
	std::unordered_map<int, TTF_Font*>	m_pFontData;
	class CGame*						m_pGame;
};

#endif // !__FONT_H_INCLUDED__