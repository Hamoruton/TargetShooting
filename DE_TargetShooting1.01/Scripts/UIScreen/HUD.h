#ifndef __HUD_H_INCLUDED__
#define __HUD_H_INCLUDED__

#pragma once
#include "UIScreen.h"
#include <vector>
#include <string>

class CHUD :
	public CUIScreen
{
public:
	CHUD(class CGame* pGame);
	virtual ~CHUD();

	void Update(float deltaTime) override;
	void Draw(class CShader* shader) override;

	void AddTargetComponent(class CTargetComponent* tc);
	void RemoveTargetComponent(class CTargetComponent* tc);

	// ターゲット数
	void SetTargetNum(const std::string& tex,
		const Vector3& color = Color::Black,
		int pointSize = 30);

	int GetTargetNum() const { return m_iTargetNum; }
	void SetTargetNum(int value) { m_sTargetNum = std::to_string(value); m_iTargetNum = value; }

protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);

	class CTexture*		m_pHealtBar;
	class CTexture*		m_pRadar;
	class CTexture*		m_pCrosshair;
	class CTexture*		m_pCrosshairEnemy;
	class CTexture*		m_pBlipTex;
	class CTexture*		m_pRadarArrow;
	class CTexture*		m_pTargetUI;
	class CTexture*		m_pTargetNum;

	// ターゲットコンポーネント
	std::vector<class CTargetComponent*>	m_pTargetComponents;
	// レーダーに対するブリップの2Dオフセット
	std::vector<Vector2>					m_vBlips;
	// ラダー
	float		m_fRadarRange;
	float		m_fRadarRadius;
	
	bool		m_bTargetEnemy;

	// ターゲット
	int				m_iTargetNum;
	std::string		m_sTargetNum;
};

#endif // !__HUD_H_INCLUDED__