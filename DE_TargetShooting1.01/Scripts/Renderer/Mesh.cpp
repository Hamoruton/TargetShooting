#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL.h>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Math.h"

//=================================================================
// コンストラクタ
//=================================================================
CMesh::CMesh()
	:m_cBox(Vector3::Infinity, Vector3::NegInfinity)
	, m_pVertexArray(nullptr)
	, m_fRadius(0.0f)
	, m_fSpecPower(100.0f)
{
}

//=================================================================
// デストラクタ
//=================================================================
CMesh::~CMesh() 
{
}

//=================================================================
// データの読み込み
//=================================================================
bool CMesh::Load(const std::string& fileName, CRenderer* pRenderer) 
{
	// ファイルを開く
	std::ifstream file(fileName);
	if (!file.is_open()) 
	{
		SDL_Log("File not found : Mesh %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) 
	{
		SDL_Log("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// バージョンチェック
	if (ver != 1) 
	{
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	m_sShaderName = doc["shader"].GetString();

	size_t vertSize = 8;

	// テクスチャ読み込み
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1) 
	{
		SDL_Log("Mesh %s has no textures", fileName.c_str());
		return false;
	}

	m_fSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++) 
	{
		// 既にテクスチャはロードされているかどうか
		std::string texName = textures[i].GetString();
		CTexture* tex = pRenderer->GetTexture(texName);
		if (tex == nullptr) 
		{
			tex = pRenderer->GetTexture(texName);
			if (tex == nullptr) 
			{
				tex = pRenderer->GetTexture("Assets/Default.png");
			}
		}
		m_pTextures.emplace_back(tex);
	}

	// 頂点の読み込み
	const rapidjson::Value& vertsJson = doc["vertices"];
	if(!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	m_fRadius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8)
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		m_fRadius = Math::Max(m_fRadius, pos.LengthSq());
		m_cBox.UpdateMinMax(pos);

		for (rapidjson::SizeType i = 0; i < vert.Size(); i++)
		{
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
		}
	}
	m_fRadius = Math::Sqrt(m_fRadius);

	// インデックスの読み込み
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// 頂点配列を生成
	m_pVertexArray = new CVertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		indices.data(), static_cast<unsigned>(indices.size()));

	return true;
}

//=================================================================
// データの削除
//=================================================================
void CMesh::Unload() 
{
	delete  m_pVertexArray;
	m_pVertexArray = nullptr;
}

//=================================================================
// テクスチャ取得
//=================================================================
CTexture* CMesh::GetTexture(size_t index) 
{
	if (index < m_pTextures.size()) 
	{
		return m_pTextures[index];
	}
	else
	{
		return nullptr;
	}
}