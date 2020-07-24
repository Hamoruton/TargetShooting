#include "SplineCamera.h"
#include "SplineCamera.h"

//=================================================================
// スプライン計算
//=================================================================
Vector3 Spline::Compute(size_t startIdx, float t) const 
{
	if (startIdx >= m_vControlPoints.size()) 
	{
		return m_vControlPoints.back();
	}
	else if (startIdx == 0) 
	{
		return m_vControlPoints[startIdx];
	}
	else if (startIdx + 2 >= m_vControlPoints.size()) 
	{
		return m_vControlPoints[startIdx];
	}

	// ポイント取得
	Vector3 p0 = m_vControlPoints[startIdx - 1];
	Vector3 p1 = m_vControlPoints[startIdx];
	Vector3 p2 = m_vControlPoints[startIdx + 1];
	Vector3 p3 = m_vControlPoints[startIdx + 2];

	Vector3 position = 0.5f * ((2.0f * p1) + (-1.0f * p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
		(-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);

	return position;
}

//=================================================================
// コンストラクタ
//=================================================================
CSplineCamera::CSplineCamera(CActor* owner)
	:CCameraComponent(owner)
	, m_szIndex(1)
	, m_fT(0.0f)
	, m_fSpeed(0.5f)
	, m_bPaused(true)
{
}

//=================================================================
// 更新
//=================================================================
void CSplineCamera::Update(float deltaTime) 
{
	CCameraComponent::Update(deltaTime);

	if (!m_bPaused) 
	{
		m_fT += m_fSpeed * deltaTime;
		if (m_fT >= 1.0f) 
		{
			if (m_szIndex < m_sPath.GetNumPoints() - 3) 
			{
				m_szIndex++;
				m_fT = m_fT - 1.0f;
			}
			else
			{
				m_bPaused = true;
			}
		}
	}

	Vector3 cameraPos = m_sPath.Compute(m_szIndex, m_fT);
	Vector3 target = m_sPath.Compute(m_szIndex, m_fT + 0.01);
	const Vector3 up = Vector3::UnitZ;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}

//=================================================================
// 再スタート
//=================================================================
void CSplineCamera::Restart() 
{
	m_szIndex = 1;
	m_fT = 0.0f;
	m_bPaused = false;
}