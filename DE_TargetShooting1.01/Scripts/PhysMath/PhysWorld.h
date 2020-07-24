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

	// �Փˌ��ʂɊւ���L�p�ȏ��
	struct CollisionInfo 
	{
		Vector3 m_sPoint;
		Vector3 m_sNormal;
		class CBoxComponent*	m_pBox;
		class CActor*			m_pActor;
	};

	// �{�b�N�X�ƏՓ˂����ꍇtrue��Ԃ�
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	// �P���ȃy�A���C�Y���g�p���ďՓ˂��e�X�g
	void TestPairwise(std::function<void(class CActor*, class CActor*)> f);

	// �X�C�[�v�ƃv���[�����g�p���ďՓ˂��e�X�g
	void TestSweepAndPrune(std::function<void(class CActor*, class CActor*)> f);

	// �{�b�N�X�R���|�[�l���g�ǉ�/�폜
	void AddBox(class CBoxComponent* box);
	void RemoveBox(class CBoxComponent* box);

private:
	class CGame*						m_pGame;
	std::vector<class CBoxComponent*>	m_pBoxes;
};

#endif // !__PHYSWORLD_H_INCLUDED__