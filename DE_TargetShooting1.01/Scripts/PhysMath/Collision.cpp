#include "Collision.h"
#include <algorithm>
#include <array>

//=================================================================
// ����
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
	// �Ō�ɏ��Z���s����sc��tc���擾
	sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
	tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

	// �ł��߂�2�̃|�C���g�̍����擾
	Vector3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

	return dP.LengthSq();   // �ł��߂�������2���Ԃ�
}

//=================================================================
// ����
//=================================================================

Plane::Plane(const Vector3& normal, float d)
	:m_sNormal(normal)
	, m_fD(d)
{
}

Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c) 
{
	// a����b�����a����c�ւ̃x�N�g�����v�Z
	Vector3 ab = b - a;
	Vector3 ac = c - a;
	// �N���X�ςƐ��K
	m_sNormal = Vector3::Cross(ab, ac);
	m_sNormal.Normalize();
	m_fD = -Vector3::Dot(a, m_sNormal);
}

float Plane::SignedDist(const Vector3& point) const 
{
	return Vector3::Dot(point, m_sNormal) - m_fD;
}

//=================================================================
// ��
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
// �����s�o�E���f�B���O�{�b�N�X axis-aligned bounding box
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
	// �{�b�N�X�̃R�[�i�[��8�̃|�C���g���쐬
	std::array<Vector3, 8> points;
	// �ŏ��_�͏�ɃR�[�i�[
	points[0] = m_sMin;
	points[1] = Vector3(m_sMax.x, m_sMin.y, m_sMin.z);
	points[2] = Vector3(m_sMin.x, m_sMax.y, m_sMin.z);
	points[3] = Vector3(m_sMin.x, m_sMin.y, m_sMax.z);
	points[4] = Vector3(m_sMin.x, m_sMax.y, m_sMax.z);
	points[5] = Vector3(m_sMax.x, m_sMin.y, m_sMax.z);
	points[6] = Vector3(m_sMax.x, m_sMax.y, m_sMin.z);
	// �ő�_�R�[�i�[
	points[7] = Vector3(m_sMax);

	// �ŏ��̓_����]
	Vector3 p = Vector3::Transform(points[0], q);
	// �ŏ�/�ő����]�����ŏ��̃|�C���g�Ƀ��Z�b�g
	m_sMin = p;
	m_sMax = p;
	// ��]�A�c��̃|�C���g�Ɋ�Â��čŏ�/�ő���X�V
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
	// �����̂�����ɂ��Y�����Ȃ��ꍇ�A�|�C���g�̓{�b�N�X�̓����ɂ���
	return !outside;
}

float AABB::MinDistSq(const Vector3& point) const 
{
	// �e���̍����v�Z����
	float dx = Math::Max(m_sMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - m_sMax.x);
	float dy = Math::Max(m_sMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - m_sMax.y);
	float dz = Math::Max(m_sMin.z - m_sMax.y, 0.0f);
	dz = Math::Max(dy, point.z - m_sMax.z);

	// ������2�掮
	return dx * dx + dy * dy + dz * dz;
}

//=================================================================
// �Փ˔���
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
		// t���͈͓��ɂ��邱�Ƃ��e�X�g
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

	// �����̂�������^�łȂ��ꍇ�A��������
	return !no;
}

bool Intersect(const LineSegment& l, const AABB& b, float outT, Vector3& outNorm) 
{
	// ���ׂẲ\��t�l�Ƃ����̕ӂ̖@����ۑ�����x�N�g��
	std::vector<std::pair<float, Vector3>> tValues;
	// X���ʂ��e�X�g����
	TestSidePlane(l.m_sStart.x, l.m_sEnd.x, b.m_sMin.x, Vector3::NegUnitX, tValues);
	TestSidePlane(l.m_sStart.x, l.m_sEnd.x, b.m_sMax.x, Vector3::UnitX, tValues);
	// Y���ʂ��e�X�g����
	TestSidePlane(l.m_sStart.y, l.m_sEnd.y, b.m_sMin.y, Vector3::NegUnitY, tValues);
	TestSidePlane(l.m_sStart.y, l.m_sEnd.y, b.m_sMax.y, Vector3::UnitY, tValues);
	// Z���ʂ��e�X�g����
	TestSidePlane(l.m_sStart.z, l.m_sEnd.z, b.m_sMin.z, Vector3::NegUnitZ, tValues);
	TestSidePlane(l.m_sStart.z, l.m_sEnd.z, b.m_sMax.z, Vector3::UnitZ, tValues);

	// t�l�������ɕ��בւ�
	std::sort(tValues.begin(), tValues.end(), [](
		const std::pair<float, Vector3>& a,
		const std::pair<float, Vector3>& b) {
		return a.first < b.first;
	});
	// �{�b�N�X�ɂ����̌�_���܂܂�Ă��邩�ǂ������e�X�g����
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

	//������̌������{�b�N�X�̋��E���ɂ���܂���
	return false;
}