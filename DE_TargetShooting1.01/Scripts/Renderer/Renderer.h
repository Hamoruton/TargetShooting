#ifndef __RENDERER_H_INCLUDED__
#define __RENDERER_H_INCLUDED__
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "SDL.h"
#include "Math.h"

struct DirectionalLight 
{
	Vector3 direction;			// ÉâÉCÉgÇÃï˚å¸
	Vector3 diffuse_color;		// ägéUêE
	Vector3 spec_color;		// ãæñ êF
};

class CRenderer
{
public:
	CRenderer(class CGame* pGame);
	virtual ~CRenderer();

	bool Initialize(float screenWidth, float screenHeight);
	void UnloadData();

	void Draw();

	void AddSprite(class CSpriteComponent* pSprite);
	void RemoveSprite(class CSpriteComponent* pSprite);

	void AddMeshComponent(class CMeshComponent* pMesh);
	void RemoveMeshComponent(class CMeshComponent* pMesh);

	class CTexture* GetTexture(const std::string& fileName);
	class CMesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { m_matView = view; }

	void SetAmbientLight(const Vector3& ambient) { m_vecAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return m_sDirLight; }

	Vector3 Unproject(const Vector3& screenPoint) const;
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;
	float GetScreenWidth() const { return m_fScreenWidth; }
	float GetScreenHeight() const { return m_fScreenHeight; }

private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class CShader *pShader);

	std::unordered_map<std::string, class CTexture*>	m_pTextures;
	std::unordered_map<std::string, class CMesh*>		m_pMeshes;
	std::vector<class CSpriteComponent*>				m_pSprites;
	std::vector<class CMeshComponent*>					m_pMeshComponents;

	class CGame*	m_pGame;

	class CShader*			m_pSpriteShader;
	class CVertexArray*		m_pSpriteVerts;
	class CShader*			m_pMeshShader;

	Matrix4		m_matView;
	Matrix4		m_matProjection;

	float	m_fScreenWidth;
	float	m_fScreenHeight;

	Vector3			m_vecAmbientLight;
	DirectionalLight	m_sDirLight;

	SDL_Window*		m_pWindow;
	SDL_GLContext	m_glContext;
};

#endif // !__RENDERER_H_INCLUDED__