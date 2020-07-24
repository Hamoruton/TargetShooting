#pragma once
#ifndef __SPRITECOMPONENT_H_INCLUDED__
#define __SPRITECOMPONENT_H_INCLUDED__

#include "Component.h"
#include "SDL.h"

class CSpriteComponent :
	public CComponent
{
public:
	CSpriteComponent(class CActor* owner, int drawOrder = 100);
	virtual ~CSpriteComponent();

	virtual void Draw(class CShader* pRenderer);
	virtual void SetTexture(class CTexture* pTexture);

	int	GetDrawOrder() const { return m_iDrawOrder; }
	int GetTexWidth() const { return m_iTexWidth; }
	int GetTexHeight() const { return m_iTexHeight; }

	void SetVisible(bool visible) { m_bVisible = visible; }
	bool GetVisible() const { return m_bVisible; }

protected:
	class CTexture* m_pTexture;
	int				m_iDrawOrder;
	int				m_iTexWidth;
	int				m_iTexHeight;
	bool			m_bVisible;
};

#endif // !__SPRITECOMPONENT_H_INCLUDED__



