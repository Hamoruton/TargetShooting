#include "Font.h"
#include "Texture.h"
#include <vector>
#include "Game.h"

//=================================================================
// コンストラクタ
//=================================================================
CFont::CFont(class CGame* pGame)
	:m_pGame(pGame) 
{
}

//=================================================================
// デストラクタ
//=================================================================
CFont::~CFont() 
{
}

//=================================================================
// フォントデータの読み込み
//=================================================================
bool CFont::Load(const std::string& fileName) 
{
	// これらのフォントサイズをサポート
	std::vector<int> fotSizes = 
	{
		8,9,
		10,11,12,14,16,18,
		20, 22, 24, 26, 28,
		30, 32, 34, 36, 38,
		40, 42, 44, 46, 48,
		52, 56,
		60, 64, 68,
		72
	};

	for (auto& size : fotSizes) 
	{
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr) 
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}
		m_pFontData.emplace(size, font);
	}
	return true;
}

//=================================================================
// フォントデータの破棄
//=================================================================
void CFont::Unload() 
{
	for (auto& font : m_pFontData) 
	{
		TTF_CloseFont(font.second);
	}
}

//=================================================================
// 文字列とこのフォントを指定して、テクスチャに描画
//=================================================================
CTexture* CFont::RenderText(const std::string& textKey, const Vector3& color, int pointSize) 
{
	CTexture* texture = nullptr;

	// SDL_Colorに変換
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	// ポイントサイズのフォントデータを見つける
	auto iter = m_pFontData.find(pointSize);
	if (iter != m_pFontData.end()) 
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = m_pGame->GetText(textKey);
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
		if (surf != nullptr) 
		{
			texture = new CTexture();
			texture->CreateFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("Point size %d is unsupported", pointSize);
	}

	return texture;
}