#include <algorithm>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "PlaneActor.h"
#include "WallActor.h"
#include "FPSActor.h"
#include "TargetActor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "PhysWorld.h"
#include "Font.h"
#include "UIScreen.h"
#include "HUD.h"
#include "PauseMenu.h"
#include "Title.h"

//=================================================================
// コンストラクタ
//=================================================================
CGame::CGame()
	: m_pRenderer(nullptr)
	, m_pAudioSystem(nullptr)
	, m_pPhysWorld(nullptr)
	, m_bUpdatingActors(false)
	, m_sGameState(EGameplay)
	, m_bStartLoadData(true)
{
}

//=================================================================
// デストラクタ
//=================================================================
CGame::~CGame() 
{
	UnloadData();
	TTF_Quit();
	delete m_pPhysWorld;
	delete m_pRenderer;
	if (m_pAudioSystem) 
	{
		m_pAudioSystem->Shutdown();
	}
	SDL_Quit();
}

//=================================================================
// 初期化
//=================================================================
bool CGame::Initialize() 
{
	// SDL初期化
	int sdl_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_result != 0) 
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	// レンダラー生成
	m_pRenderer = new CRenderer(this);
	if (!m_pRenderer->Initialize(1000.0f, 750.0f)) 
	{
		SDL_Log("Failed to initialize renderer");
		delete m_pRenderer;
		m_pRenderer = nullptr;
		return false;
	}

	// オーディオシステム生成
	m_pAudioSystem = new CAudioSystem(this);
	if (!m_pAudioSystem->Initialize()) 
	{
		SDL_Log("Failed to initialize audio system");
		m_pAudioSystem->Shutdown();
		delete m_pAudioSystem;
		m_pAudioSystem = nullptr;
		return false;
	}

	// ステージ上に物理判定生成
	m_pPhysWorld = new CPhysWorld(this);

	// SDL_ttf初期化
	if (TTF_Init() != 0) 
	{
		SDL_Log("Failed ton initialze SDL_ttf");
		return false;
	}

	// フォント
	LoadText("Assets/Japanese.gptext");

	// プレームタイマー
	m_iTicksCount = SDL_GetTicks();

	new CTitle(this);

	return true;
}

//=================================================================
// ゲームループ
//=================================================================
void CGame::RunLoop() 
{
	while (m_sGameState != EQuit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

//=================================================================
// 入力処理
//=================================================================
void CGame::ProcessInput() 
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type) 
		{
		case SDL_QUIT:
			m_sGameState = EQuit;
			break;

		// キーが最初に押されたときに発生
		case SDL_KEYDOWN:
			if (!event.key.repeat) 
			{
				if (m_sGameState == EGameplay) 
				{
					HandleKeyPress(event.key.keysym.sym);
				}
				else if (!m_pUIStack.empty()) 
				{
					m_pUIStack.back()->HandleKeyPress(event.key.keysym.sym);
				}
				
			}
			break;

		// マウスが押された時に発生
		case SDL_MOUSEBUTTONDOWN:
			if (m_sGameState == EGameplay) 
			{
				HandleKeyPress(event.button.button);
			}
			else if (!m_pUIStack.empty()) 
			{
				m_pUIStack.back()->HandleKeyPress(event.button.button);
			}
			break;

		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (m_sGameState == EGameplay) 
	{
		for (auto actor : m_pActors)
		{
			if (actor->GetState() == CActor::EActive) 
			{
				actor->ProcessInput(state);
			}
		}
	}
	else if (!m_pUIStack.empty()) 
	{
		m_pUIStack.back()->ProcessInput(state);
	}
}

void CGame::HandleKeyPress(int key) 
{
	switch (key) 
	{
		// ポーズ
	case SDLK_ESCAPE:
		new CPauseMenu(this);
		break;

	case '-':
	{
		// サウンドの大きさ減量
		float volume = m_pAudioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		m_pAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '^':
	{
		// サウンドの大きさ増量
		float volume = m_pAudioSystem->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		m_pAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case SDL_BUTTON_LEFT:
	{
		m_pFPSActor->Shoot();
		break;
	}

	default:
		break;
	}
}

//=================================================================
// 更新処理
//=================================================================
void CGame::UpdateGame() 
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_iTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - m_iTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) 
	{
		deltaTime = 0.05f;
	}
	m_iTicksCount = SDL_GetTicks();

	// アクタ
	if (m_sGameState == EGameplay) 
	{
		m_bUpdatingActors = true;
		for (auto actor : m_pActors)
		{
			actor->Update(deltaTime);
		}
		m_bUpdatingActors = false;

		for (auto pending : m_pPendingActors)
		{
			m_pActors.emplace_back(pending);
		}
		m_pPendingActors.clear();

		std::vector<CActor*> deadActors;
		for (auto actor : m_pActors)
		{
			if (actor->GetState() == CActor::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}

		for (auto actor : deadActors)
		{
			delete actor;
		}
	}
	
	// オーディオシステム
	m_pAudioSystem->Update(deltaTime);

	// HUI
	for (auto ui : m_pUIStack) 
	{
		if (ui->GetState() == CUIScreen::EActive) 
		{
			ui->Update(deltaTime);
		}
	}
	// 閉じているUIScreenを削除
	auto iter = m_pUIStack.begin();
	while (iter != m_pUIStack.end()) 
	{
		if ((*iter)->GetState() == CUIScreen::EClosing) 
		{
			delete *iter;
			iter = m_pUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

//=================================================================
// 出力処理
//=================================================================
void CGame::GenerateOutput() 
{
	m_pRenderer->Draw();
}

//=================================================================
// データの読み込み
//=================================================================
void CGame::LoadData() 
{
	// キューブアクタ生成
	CActor* actor = new CActor(this);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);

	// 床/壁アクア生成
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++) 
	{
		for (int j = 0; j < 10; j++) 
		{
			actor = new CPlaneActor(this);
			actor->SetPosition(Vector3(start + i * size, start + j * size, -100));
		}
	}

	// 左右壁アクタ生成
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		actor = new CWallActor(this);
		actor->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		actor->SetRotation(q);

		actor = new CWallActor(this);
		actor->SetPosition(Vector3(start + i * size, start - size, size * 2));
		actor->SetRotation(q);

		actor = new CWallActor(this);
		actor->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		actor->SetRotation(q);

		actor = new CWallActor(this);
		actor->SetPosition(Vector3(start + i * size, -start + size, size * 2));
		actor->SetRotation(q);
	}

	// 前後壁アクタ生成
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	for (int i = 0; i < 10; i++)
	{
		actor = new CWallActor(this);
		actor->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		actor->SetRotation(q);

		actor = new CWallActor(this);
		actor->SetPosition(Vector3(start - size, start + i * size, size * 2));
		actor->SetRotation(q);

		actor = new CWallActor(this);
		actor->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		actor->SetRotation(q);

		actor = new CWallActor(this);
		actor->SetPosition(Vector3(-start + size, start + i * size, size * 2));
		actor->SetRotation(q);
	}

	// ライトの設定
	m_pRenderer->SetAmbientLight(Vector3(0.5, 0.5, 0.5));
	DirectionalLight& dir = m_pRenderer->GetDirectionalLight();
	dir.direction = Vector3(0.0f, -0.707f, -0.707f);
	dir.diffuse_color = Vector3(0.78f, 0.88f, 1.0f);
	dir.spec_color = Vector3(0.8f, 0.8f, 0.8f);

	// HUDアクタ
	m_pHUD = new CHUD(this);

	// サウンド
	m_cMusicEvent = m_pAudioSystem->PlayEvent("event:/Music");

	// カメラの外観で相対マウスモードを有効にする
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// カメラアクタ
	m_pFPSActor = new CFPSActor(this);

	// ターゲットアクタ
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(0.0f, -1450.0f, 200.0f));
	actor->SetRotation(Quaternion(Vector3::UnitZ, Math::PiOver2));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(0.0f, 1450.0f, 200.0f));
	actor->SetRotation(Quaternion(Vector3::UnitZ, -Math::PiOver2));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(-1450.0f, 0.0f, 100.0f));
	actor->SetRotation(Quaternion(Vector3::UnitZ, Math::TwoPi));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(-1450.0f, 0.0f, 400.0f));
	actor->SetRotation(Quaternion(Vector3::UnitZ, Math::TwoPi));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(-1450.0f, -500.0f, 200.0f));
	actor->SetRotation(Quaternion(Vector3::UnitZ, Math::TwoPi));
	actor = new CTargetActor(this);
	actor->SetPosition(Vector3(-1450.0f, 500.0f, 200.0f));
	actor->SetRotation(Quaternion(Vector3::UnitZ, Math::TwoPi));
}

//=================================================================
// データの破棄
//=================================================================
void CGame::UnloadData() 
{
	// アクタ
	while (!m_pActors.empty()) 
	{
		delete m_pActors.back();
	}

	// UI
	while (!m_pUIStack.empty()) 
	{
		delete m_pUIStack.back();
		m_pUIStack.pop_back();
	}

	if (m_pRenderer) 
	{
		m_pRenderer->UnloadData();
	}
}

//=================================================================
// アクタ追加
//=================================================================
void CGame::AddActor(CActor* actor) 
{
	if (m_bUpdatingActors) 
	{
		m_pPendingActors.emplace_back(actor);
	}
	else
	{
		m_pActors.emplace_back(actor);
	}
}

//=================================================================
// アクタ削除
//=================================================================
void CGame::RemoveActor(CActor* actor) 
{
	auto iter = std::find(m_pPendingActors.begin(), m_pPendingActors.end(), actor);
	if (iter != m_pPendingActors.end()) 
	{
		std::iter_swap(iter, m_pPendingActors.end() - 1);
		m_pPendingActors.pop_back();
	}

	iter = std::find(m_pActors.begin(), m_pActors.end(), actor);
	if (iter != m_pActors.end()) 
	{
		std::iter_swap(iter, m_pActors.end() - 1);
		m_pActors.pop_back();
	}
}

//=================================================================
// プレーンアクタ追加
//=================================================================
void CGame::AddPlane(CPlaneActor* plane) 
{
	m_pPlanes.emplace_back(plane);
}

//=================================================================
// プレーンアクタ削除
//=================================================================
void CGame::RemovePlane(CPlaneActor* plane) 
{
	auto iter = std::find(m_pPlanes.begin(), m_pPlanes.end(), plane);
	m_pPlanes.erase(iter);
}

//=================================================================
// ウォールアクタ追加
//=================================================================
void CGame::AddWall(CWallActor* wall) 
{
	m_pWalls.emplace_back(wall);
}

//=================================================================
// ウォールアクタ削除
//=================================================================
void CGame::RemoveWall(CWallActor* wall) 
{
	auto iter = std::find(m_pWalls.begin(), m_pWalls.end(), wall);
	m_pWalls.erase(iter);
}

//=================================================================
// フォント取得
//=================================================================
CFont* CGame::GetFont(const std::string& fileName) 
{
	auto iter = m_pFonts.find(fileName);
	if (iter != m_pFonts.end()) 
	{
		return iter->second;
	}
	else
	{
		CFont* font = new CFont(this);
		if (font->Load(fileName)) 
		{
			m_pFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}

//=================================================================
// 
//=================================================================
void CGame::PushUI(CUIScreen* screen) 
{
	m_pUIStack.emplace_back(screen);
}

//=================================================================
// テクスチャ読み込み
//=================================================================
void CGame::LoadText(const std::string& fileName) 
{
	// 既にロードされている場合は、既存のマップをクリア
	m_pText.clear();

	// ファイルを開く
	std::ifstream file(fileName);
	if (!file.is_open()) 
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}

	// ファイル全体を文字列ストリームに読み込む
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();

	// このファイルをrapidJSONで開く
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject()) 
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}

	// テキストマップを解析
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr) 
	{
		if (itr->name.IsString() && itr->value.IsString()) 
		{
			m_pText.emplace(itr->name.GetString(), itr->value.GetString());
		}
	}
}

//=================================================================
// テクスチャ変数取得
//=================================================================
const std::string& CGame::GetText(const std::string& key) 
{
	static std::string errorMsg("**KEY NOT FOUND**");

	// テキストが存在する場合は、マップで検索します
	auto iter = m_pText.find(key);
	if (iter != m_pText.end()) 
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}