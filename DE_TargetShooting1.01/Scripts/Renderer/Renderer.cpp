#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include "GL/glew.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Game.h"
#include "UIScreen.h"

//=================================================================
// �R���X�g���N�^
//=================================================================
CRenderer::CRenderer(CGame* pGame)
	:m_pGame(pGame)
	,m_pSpriteShader(nullptr)
	,m_pMeshShader(nullptr)
{
}

//=================================================================
// �f�X�g���N�^
//=================================================================
CRenderer::~CRenderer() 
{
	delete m_pSpriteVerts;
	m_pSpriteShader->Unload();
	delete m_pSpriteShader;
	m_pMeshShader->Unload();
	delete m_pMeshShader;
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_pWindow);
}

//=================================================================
// �f�[�^�̔j��
//=================================================================
void CRenderer::UnloadData() 
{
	for (auto tex : m_pTextures) 
	{
		tex.second->Unload();
		delete tex.second;
	}
	m_pTextures.clear();

	for (auto mesh : m_pMeshes) 
	{
		mesh.second->Unload();
		delete mesh.second;
	}
	m_pMeshes.clear();
}

//=================================================================
// ������
//=================================================================
bool CRenderer::Initialize(float screenWidth, float screenHeight) 
{
	m_fScreenWidth = screenWidth;
	m_fScreenHeight = screenHeight;

	// OpenGL�����ɃZ�b�g
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// �E�B���h�E����
	m_pWindow = SDL_CreateWindow("TargetShooting", 50, 50,
		static_cast<int>(m_fScreenWidth), static_cast<int>(m_fScreenHeight), SDL_WINDOW_OPENGL);
	if (!m_pWindow) 
	{
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	// �R���e�L�X�g����
	m_glContext = SDL_GL_CreateContext(m_pWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) 
	{
		SDL_Log("Failed to initialize GLEW");
		return false;
	}

	glGetError();

	// �V�F�[�_�[����/�R���p�C��
	if (!LoadShaders()) 
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// �X�v���C�g�`��̂��߂̎l�p�`(�N���b�h)�𐶐�
	CreateSpriteVerts();

	return true;
}

//=================================================================
// �����_�����O
//=================================================================
void CRenderer::Draw() 
{
	glClearColor(0.8f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���b�V�������_�����O 
	// �[�x�o�b�t�@�����O��L���ɂ���/�A���t�@�u�����h�𖳌��ɂ���
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	m_pMeshShader->SetActive();
	m_pMeshShader->SetMatrixUniform("uViewProj", m_matView*m_matProjection);
	SetLightUniforms(m_pMeshShader);
	for (auto mc : m_pMeshComponents) 
	{
		if (mc->GetVisible()) 
		{
			mc->Draw(m_pMeshShader);
		}
	}

	glDisable(GL_DEPTH_TEST);

	// �X�v���C�g�����_�����O
	// �J���[�o�b�t�@�[�ŃA���t�@�u�����f�B���O��L���ɂ���
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	m_pSpriteShader->SetActive();
	m_pSpriteVerts->SetActive();
	for (auto sprite : m_pSprites) 
	{
		if (sprite->GetVisible()) 
		{
			sprite->Draw(m_pSpriteShader);
		}
	}

	// HUD
	for (auto ui : m_pGame->GetUIStack()) 
	{
		ui->Draw(m_pSpriteShader);
	}

	SDL_GL_SwapWindow(m_pWindow);
}

//=================================================================
// �X�v���C�g�R���|�[�l���g�ǉ�
//=================================================================
void CRenderer::AddSprite(CSpriteComponent* pSprite) 
{
	int myDrawOrder = pSprite->GetDrawOrder();
	auto iter = m_pSprites.begin();
	for (; iter != m_pSprites.end(); ++iter) 
	{
		if (myDrawOrder < (*iter)->GetDrawOrder()) 
		{
			break;
		}
	}

	// �C�e���[�^�̈ʒu�̑O�ɗv�f��}��
	m_pSprites.insert(iter, pSprite);
}

//=================================================================
// �X�v���C�g�R���|�[�l���g�폜
//=================================================================
void CRenderer::RemoveSprite(CSpriteComponent* pSprite) 
{
	auto iter = std::find(m_pSprites.begin(), m_pSprites.end(), pSprite);
	m_pSprites.erase(iter);
}

//=================================================================
// ���b�V���R���|�[�l���g�ǉ�
//=================================================================
void CRenderer::AddMeshComponent(CMeshComponent* pMesh) 
{
	m_pMeshComponents.emplace_back(pMesh);
}

//=================================================================
// ���b�V���R���|�[�l���g�폜
//=================================================================
void CRenderer::RemoveMeshComponent(CMeshComponent* pMesh) 
{
	auto iter = std::find(m_pMeshComponents.begin(), m_pMeshComponents.end(), pMesh);
	m_pMeshComponents.erase(iter);
}

//=================================================================
// �e�N�X�`���擾
//=================================================================
CTexture* CRenderer::GetTexture(const std::string& fileName) 
{
	CTexture* tex = nullptr;
	auto iter = m_pTextures.find(fileName);
	if (iter != m_pTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new CTexture();
		if (tex->Load(fileName))
		{
			m_pTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

//=================================================================
// ���b�V���擾
//=================================================================
CMesh* CRenderer::GetMesh(const std::string& fileName) 
{
	CMesh* mesh = nullptr;
	auto iter = m_pMeshes.find(fileName);
	if (iter != m_pMeshes.end()) 
	{
		mesh = iter->second;
	}
	else
	{
		mesh = new CMesh();
		if (mesh->Load(fileName, this)) 
		{
			m_pMeshes.emplace(fileName, mesh);
		}
		else
		{
			delete mesh;
			mesh = nullptr;
		}
	}
	return mesh;
}

//=================================================================
// �V�F�[�_�[�̓ǂݍ���
//=================================================================
bool CRenderer::LoadShaders() 
{
	// �X�v���C�g�V�F�[�_�[����
	m_pSpriteShader = new CShader();
	if (!m_pSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) 
	{
		return false;
	}
	m_pSpriteShader->SetActive();

	// �r���[���W/�v���W�F�N�V�������W
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_fScreenWidth, m_fScreenHeight);
	m_pSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// ���b�V���V�F�[�_�[����
	m_pMeshShader = new CShader();
	if (!m_pMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag")) 
	{
		return false;
	}
	m_pMeshShader->SetActive();

	// �r���[���W/�v���W�F�N�V�������W
	m_matView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	m_matProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		m_fScreenWidth, m_fScreenHeight, 25.0f, 10000.0f);
	m_pMeshShader->SetMatrixUniform("uViewProj", m_matView * m_matProjection);

	return true;
}

//=================================================================
// �X�v���C�g�`��̂��߂̃N���b�h����
//=================================================================
void CRenderer::CreateSpriteVerts() 
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f,	// top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f,	// bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_pSpriteVerts = new CVertexArray(vertices, 4, indices, 6);
}

//=================================================================
// ���C�g�̐ݒ�
//=================================================================
void CRenderer::SetLightUniforms(CShader* pShader) 
{
	Matrix4 invView = m_matView;
	invView.Invert();
	pShader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	// ���͌�
	pShader->SetVectorUniform("uAmbientLight", m_vecAmbientLight);

	// �w�������C�g
	pShader->SetVectorUniform("uDirLight.mDirection",
		m_sDirLight.direction);
	pShader->SetVectorUniform("uDirLight.mDiffuseColor",
		m_sDirLight.diffuse_color);
	pShader->SetVectorUniform("uDirLight.mSpecColor",
		m_sDirLight.spec_color);
}

//=================================================================
// ��ˉe�ϊ�
//=================================================================
Vector3 CRenderer::Unproject(const Vector3& screenPoint) const 
{
	// screenPoint���f�o�C�X���W�ɕϊ��i-1����+1�̊ԁj
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (m_fScreenWidth)*0.5f;
	deviceCoord.y /= (m_fScreenHeight)*0.5f;

	// ��ˉe�s��ɂ��x�N�g���̕ϊ�
	Matrix4 unprojection = m_matView * m_matProjection;
	unprojection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

//=================================================================
// �X�N���[�������擾
//=================================================================
void CRenderer::GetScreenDirection(Vector3& outStart, Vector3& outDir) const 
{
	// �J�n�_���擾�i��ʂ̒����t�߂̕��ʁj
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = Unproject(screenPoint);

	// �I�_���擾�i��ʂ̒����A�߂��Ɖ����̊ԁj
	screenPoint.z = 0.9f;
	Vector3 end = Unproject(screenPoint);

	// �����x�N�g���擾
	outDir = end - outStart;
	outDir.Normalize();
}