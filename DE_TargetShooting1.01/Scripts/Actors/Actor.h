#pragma once
#ifndef __ACTOR_H_INCLUDED__
#define __ACTOR_H_INCLUDED__

#include <vector>

#include "Math.h"

class CActor
{
public:
	enum STATE 
	{
		EActive,
		EPaused,
		EDead
	};

	CActor(class CGame* pGame);
	virtual ~CActor();

public:
	// 更新
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	// 入力処理
	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyStage);

	// ゲッター/セッター
	const Vector3& GetPosition() const { return m_vecPosition; }
	void SetPosition(const Vector3& pos) { m_vecPosition = pos; m_bRecomputeWorldTransform = true; }
	float GetScale() const { return m_fScale; }
	void SetScale(float scale) { m_fScale = scale; m_bRecomputeWorldTransform = true; }
	const Quaternion& GetRotation() const { return m_qRotation; }
	void SetRotation(const Quaternion& rotation) { m_qRotation = rotation; m_bRecomputeWorldTransform = true; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return m_matWorldTransform; }

	STATE GetState() const { return m_eState; }
	void SetState(STATE state) { m_eState = state; }

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, m_qRotation); }
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, m_qRotation); }

	void RotateToNewForward(const Vector3& forward);

	class CGame* GetGame() { return m_pGame; }

	// コンポーネント追加/削除
	void AddComponent(class CComponent* component);
	void RemoveComponent(class CComponent* component);

private:
	STATE		m_eState;

	// ワールド座標
	Matrix4		m_matWorldTransform;
	Vector3		m_vecPosition;
	Quaternion	m_qRotation;
	float		m_fScale;
	bool		m_bRecomputeWorldTransform;

	std::vector<class CComponent*>	m_pComponents;
	class CGame*					m_pGame;
};

#endif // !__ACTOR_H_INCLUDED__