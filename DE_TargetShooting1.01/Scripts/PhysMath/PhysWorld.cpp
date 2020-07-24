#include "PhysWorld.h""
#include "BoxComponent.h"

#include <algorithm>
#include <SDL.h>

//=================================================================
// コンストラクタ
//=================================================================
CPhysWorld::CPhysWorld(CGame* pGame)
	:m_pGame(pGame) 
{
}

//=================================================================
// 当たり判定
//=================================================================
bool CPhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl) 
{
	bool collided = false;
	float closestT = Math::Infinity;
	Vector3 norm;

	//すべてのボックスに対してテストする
	for (auto box : m_pBoxes) 
	{
		float t = 0;
		//セグメントはボックスと交差していますか？
		if (Intersect(l, box->GetWorldBox(), t, norm))
		{
			//これは前の交差点より近いですか？
			if (t < closestT) 
			{
				outColl.m_sPoint = l.PointOnSegment(t);
				outColl.m_sNormal = norm;
				outColl.m_pBox = box;
				outColl.m_pActor = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

//=================================================================
// 単純なペアワイズを使用して衝突をテスト
//=================================================================
void CPhysWorld::TestPairwise(std::function<void(CActor*, CActor*)>f) 
{
	for (size_t i = 0; i < m_pBoxes.size(); i++)
	{
		for (size_t j = i + 1; j < m_pBoxes.size(); j++)
		{
			CBoxComponent* a = m_pBoxes[i];
			CBoxComponent* b = m_pBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				// 提供された関数を呼び出して交差を処理す​​る
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

//=================================================================
// スイープとプルーンを使用して衝突をテスト
//=================================================================
void CPhysWorld::TestSweepAndPrune(std::function<void(CActor*, CActor*)> f) 
{
	// min.xで並べ替え
	std::sort(m_pBoxes.begin(), m_pBoxes.end(),
		[](CBoxComponent* a, CBoxComponent* b) {
		return a->GetWorldBox().m_sMin.x <
			b->GetWorldBox().m_sMin.x;
	});

	for (size_t i = 0; i < m_pBoxes.size(); i++)
	{
		// 現在のボックスの最大xを取得
		CBoxComponent* a = m_pBoxes[i];
		float max = a->GetWorldBox().m_sMax.x;
		for (size_t j = i + 1; j < m_pBoxes.size(); j++)
		{
			CBoxComponent* b = m_pBoxes[j];
			
			if (b->GetWorldBox().m_sMin.x > max)
			{
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

//=================================================================
// ボックスコンポーネント追加
//=================================================================
void CPhysWorld::AddBox(CBoxComponent* box) 
{
	m_pBoxes.emplace_back(box);
}

//=================================================================
// ボックスコンポーネント削除
//=================================================================
void CPhysWorld::RemoveBox(CBoxComponent* box) 
{
	auto iter = std::find(m_pBoxes.begin(), m_pBoxes.end(), box);
	if (iter != m_pBoxes.end()) 
	{
		//ベクトルの最後までスワップしてポップオフ（コピーの消去を回避）
		std::iter_swap(iter, m_pBoxes.end() - 1);
		m_pBoxes.pop_back();
	}
}