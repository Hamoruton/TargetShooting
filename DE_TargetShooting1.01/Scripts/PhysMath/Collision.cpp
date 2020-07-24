#include "Collision.h"
#include <algorithm>
#include <array>

//=================================================================
// 線分
//=================================================================

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
	:m_sStart(start)
	, m_sEnd(end)
{
}

Vector3 LineSegment::PointOnSegment(float t) const 
{
	return m_sStart + (m_sEnd - m_sStart)*t;
}

float LineSegment::MinDistSq(const Vector3& point) const 
{
	Vector3 ab = m_sEnd - m_sStart;
	Vector3 ba = -1.0f*ab;
	Vector3 ac = point - m_sStart;
	Vector3 bc = point - m_sEnd;

	if (Vector3::Dot(ab, ac) < 0.0f) 
	{
		return ac.LengthSq();
	}
	else if (Vector3::Dot(ba, bc) < 0.0f) 
	{
		return bc.LengthSq();
	}
	else
	{
		float scalar = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab);
		Vector3 p = scalar * ab;
		return (ac - p).LengthSq();
	}
}

float LineSegment::MinDistSq(const LineSegment& s1, const LineSegment& s2) 
{
	Vector3 u = s1.m_sEnd - s1.m_sStart;
	Vector3 v = s2.m_sEnd - s2.m_sStart;
	Vector3 w = s1.m_sStart - s2.m_sStart;
	float a = Vector3::Dot(u, u);
	float b = Vector3::Dot(u, v);
	float c = Vector3::Dot(v, v);
	float d = Vector3::Dot(u, w);
	float e = Vector3::Dot(v, w);
	float D = a * c - b * b;
	float sc, sN, sD = D;
	float tc, tN, tD = D;

	if (Math::NearZero(D))
	{ 
		sN = 0.0;         
		sD = 1.0;         
		tN = e;
		tD = c;
	}
	else 
	{     
		sN = (b*e - c * d);
		tN = (a*e - b * d);
		if (sN < 0.0) 
		{       
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) 
		{ 
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) 
	{        
		tN = 0.0;
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else 
		{
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) 
	{   
		tN = tD;
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else 
		{
			sN = (-d + b);
			sD = a;
		}
	}
	// 最後に除算を行ってscとtcを取得
	sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
	tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

	// 最も近い2つのポイントの差を取得
	Vector3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

	return dP.LengthSq();   // 最も近い距離の2乗を返す
}

//=================================================================
// 平面
//=================================================================

Plane::Plane(const Vector3& normal, float d)
	:m_sNormal(normal)
	, m_fD(d)
{
}

Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c) 
{
	// aからbおよびaからcへのベクトルを計算
	Vector3 ab = b - a;
	Vector3 ac = c - a;
	// クロス積と正規
	m_sNormal = Vector3::Cross(ab, ac);
	m_sNormal.Normalize();
	m_fD = -Vector3::Dot(a, m_sNormal);
}

float Plane::SignedDist(const Vector3& point) const 
{
	return Vector3::Dot(point, m_sNormal) - m_fD;
}

//=================================================================
// 球
//=================================================================

Sphere::Sphere(const Vector3& center, float radius)
	:m_sCenter(center)
	, m_fRadius(radius)
{
}

bool Sphere::Contains(const Vector3& point) const 
{
	float distSq = (m_sCenter - point).LengthSq();
	return distSq <= (m_fRadius*m_fRadius);
}

//=================================================================
// 軸平行バウンディングボックス axis-aligned bounding box
//=================================================================

AABB::AABB(const Vector3& min, const Vector3& max)
	:m_sMin(min)
	, m_sMax(max)
{
}

void AABB::UpdateMinMax(const Vector3& point) 
{
	m_sMin.x = Math::Min(m_sMin.x, point.x);
	m_sMin.y = Math::Min(m_sMin.y, point.y);
	m_sMin.z = Math::Min(m_sMin.z, point.z);

	m_sMax.x = Math::Max(m_sMax.x, point.x);
	m_sMax.y = Math::Max(m_sMax.y, point.y);
	m_sMax.z = Math::Max(m_sMax.z, point.z);
}

void AABB::Rotate(const Quaternion& q) 
{
	// ボックスのコーナーの8つのポイントを作成
	std::array<Vector3, 8> points;
	// 最小点は常にコーナー
	points[0] = m_sMin;
	points[1] = Vector3(m_sMax.x, m_sMin.y, m_sMin.z);
	points[2] = Vector3(m_sMin.x, m_sMax.y, m_sMin.z);
	points[3] = Vector3(m_sMin.x, m_sMin.y, m_sMax.z);
	points[4] = Vector3(m_sMin.x, m_sMax.y, m_sMax.z);
	points[5] = Vector3(m_sMax.x, m_sMin.y, m_sMax.z);
	points[6] = Vector3(m_sMax.x, m_sMax.y, m_sMin.z);
	// 最大点コーナー
	points[7] = Vector3(m_sMax);

	// 最初の点を回転
	Vector3 p = Vector3::Transform(points[0], q);
	// 最小/最大を回転した最初のポイントにリセット
	m_sMin = p;
	m_sMax = p;
	// 回転、残りのポイントに基づいて最小/最大を更新
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

bool AABB::Contains(const Vector3& point) const 
{
	bool outside = point.x < m_sMin.x ||
		point.y < m_sMin.y ||
		point.z < m_sMin.z ||
		point.x > m_sMax.x ||
		point.y > m_sMax.y ||
		point.z > m_sMax.z;
	// これらのいずれにも該当しない場合、ポイントはボックスの内側にある
	return !outside;
}

float AABB::MinDistSq(const Vector3& point) const 
{
	// 各軸の差を計算する
	float dx = Math::Max(m_sMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - m_sMax.x);
	float dy = Math::Max(m_sMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - m_sMax.y);
	float dz = Math::Max(m_sMin.z - m_sMax.y, 0.0f);
	dz = Math::Max(dy, point.z - m_sMax.z);

	// 距離の2乗式
	return dx * dx + dy * dy + dz * dz;
}

//=================================================================
// 衝突判定
//=================================================================

bool TestSidePlane(float start, float end, float negd, const Vector3& norm,
	std::vector<std::pair<float, Vector3>>& out)
{
	float denom = end - start;
	if (Math::NearZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;
		// tが範囲内にあることをテスト
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Intersect(const AABB& a, const AABB& b) 
{
	bool no = a.m_sMax.x < b.m_sMin.x ||
		a.m_sMax.y < b.m_sMin.y ||
		a.m_sMax.z < b.m_sMin.z ||
		b.m_sMax.x < a.m_sMin.x ||
		b.m_sMax.y < a.m_sMin.y ||
		b.m_sMax.z < a.m_sMin.z;

	// これらのいずれも真でない場合、交差する
	return !no;
}

bool Intersect(const LineSegment& l, const AABB& b, float outT, Vector3& outNorm) 
{
	// すべての可能なt値とそれらの辺の法線を保存するベクトル
	std::vector<std::pair<float, Vector3>> tValues;
	// X平面をテストする
	TestSidePlane(l.m_sStart.x, l.m_sEnd.x, b.m_sMin.x, Vector3::NegUnitX, tValues);
	TestSidePlane(l.m_sStart.x, l.m_sEnd.x, b.m_sMax.x, Vector3::UnitX, tValues);
	// Y平面をテストする
	TestSidePlane(l.m_sStart.y, l.m_sEnd.y, b.m_sMin.y, Vector3::NegUnitY, tValues);
	TestSidePlane(l.m_sStart.y, l.m_sEnd.y, b.m_sMax.y, Vector3::UnitY, tValues);
	// Z平面をテストする
	TestSidePlane(l.m_sStart.z, l.m_sEnd.z, b.m_sMin.z, Vector3::NegUnitZ, tValues);
	TestSidePlane(l.m_sStart.z, l.m_sEnd.z, b.m_sMax.z, Vector3::UnitZ, tValues);

	// t値を昇順に並べ替え
	std::sort(tValues.begin(), tValues.end(), [](
		const std::pair<float, Vector3>& a,
		const std::pair<float, Vector3>& b) {
		return a.first < b.first;
	});
	// ボックスにこれらの交点が含まれているかどうかをテストする
	Vector3 point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.Contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	//いずれの交差もボックスの境界内にありません
	return false;
}