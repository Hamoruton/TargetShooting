#ifndef __UISCREEN_H_INCLUDED__
#define __UISCREEN_H_INCLUDED__

#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class CButton 
{
public:
	CButton(const std::string& name, class CFont* font,
		std::function<void()> onClick,
		const Vector2& pos, const Vector2& dims);
	virtual ~CButton();

	// ボタンの名前を設定
	void SetName(const std::string& name);

	class CTexture* GetNameTex() { return m_pNameTex; }
	const Vector2& GetPosition() const { return m_vPosition; }
	void SetHighlighted(bool sel) { m_bHighlighted = sel; }
	bool GetHighlighted() const { return m_bHighlighted; }

	// ポイントがボタンの境界内にある場合はtrueを返す
	bool ContainsPoint(const Vector2& pt) const;

	// ボタンがクリックされたときに呼び出す
	void OnClick();

private:
	std::function<void()>	m_OnClick;
	std::string				m_sName;
	class CTexture*			m_pNameTex;
	class CFont*			m_pFont;
	Vector2					m_vPosition;
	Vector2					m_vDimensions;
	bool					m_bHighlighted;
};

class CUIScreen
{
public:
	CUIScreen(class CGame* pGame);
	virtual ~CUIScreen();

	virtual void Update(float deltaTime);
	virtual void Draw(class CShader* shader);
	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

	// UIがアクティブまたは閉じているかどうかを追跡
	enum UIState 
	{
		EActive,
		EClosing
	};
	// 状態を終了に設定
	void Close();

	// タイトルシーン
	void TitleScene();

	void GameScene();

	UIState GetState() const { return m_sState; }
	class CFont* GetFont() const { return m_pFont; }

	// タイトルのテキストを変更
	void SetTitle(const std::string& tex,
				  const Vector3& color = Color::White,
				  int pointSize = 40);

	// この画面にボタンを追加
	void AddButton(const std::string& name, std::function<void()> onClick);

	void SetNextButtonPos(Vector2 pos) { m_vNextButtonPos = pos; }
	void SetTitlePos(Vector2 pos) { m_vTitlePos = pos; }

protected:
	// テクスチャを描画するヘルパー
	void DrawTexture(class CShader* shader, class CTexture* texture,
					 const Vector2& offset = Vector2::Zero,
					 float scale = 1.0f);

	// マウスモードを相対かどうかに設定
	void SetRelativeMouseMode(bool relative);
	class CGame*		m_pGame;

	class CFont*		m_pFont;
	class CTexture*		m_pTitle;
	class CTexture*		m_pBackground;
	class CTexture*		m_pButtonOn;
	class CTexture*		m_pButtonOff;

	// ポジションを構成
	Vector2		m_vTitlePos;
	Vector2		m_vNextButtonPos;
	Vector2		m_vBGPos;

	// 状態
	UIState		m_sState;

	// ボタンリスト
	std::vector<CButton*>		m_pButtons;
};

#endif // !__UISCREEN_H_INCLUDED__