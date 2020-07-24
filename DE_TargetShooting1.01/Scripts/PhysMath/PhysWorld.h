#ifndef __PHYSWORLD_H_INCLUDED__
#define __PHYSWORLD_H_INCLUDED__

#pragma once
#include <vector>
#include <functional>

#include "Math.h"
#include "Collision.h"

class CPhysWorld
{
public:
	CPhysWorld(class CGame* pGame);

	// 衝突結果に関する有用な情報
	struct CollisionInfo 
	{
		Vector3 m_sPoint;
		Vector3 m_sNormal;
		class CBoxComponent*	m_pBox;
		class CActor*			m_pActor;
	};

	// ボックスと衝突した場合trueを返す
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	// 単純なペアワイズを使用して衝突をテスト
	void TestPairwise(std::function<void(class CActor*, class CActor*)> f);

	// スイープとプルーンを使用して衝突をテスト
	void TestSweepAndPrune(std::function<void(class CActor*, class CActor*)> f);

	// ボックスコンポーネント追加/削除
	void AddBox(class CBoxComponent* box);
	void RemoveBox(class CBoxComponent* box);

private:
	class CGame*						m_pGame;
	std::vector<class CBoxComponent*>	m_pBoxes;
};

#endif // !__PHYSWORLD_H_INCLUDED__