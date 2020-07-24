#ifndef __COLLISION_H_INCLUDED__
#define __COLLISION_H_INCLUDED__

#pragma once
#include "Math.h"
#include <vector>

struct LineSegment 
{
	LineSegment(const Vector3& start, const Vector3& end);
	Vector3 PointOnSegment(float t) const;
	float MinDistSq(const Vector3& point) const;	// 線分の最小距離の2乗
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	Vector3 m_sStart;
	Vector3 m_sEnd;
};

struct Plane 
{
	Plane(const Vector3& normal, float d);
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);	//3点からの平面を作成
	float SignedDist(const Vector3& point) const;

	Vector3 m_sNormal;
	float	m_fD;
};

struct Sphere 
{
	Sphere(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;

	Vector3		m_sCenter;
	float		m_fRadius;
};

struct AABB 
{
	AABB(const Vector3& min, const Vector3& max);

	// 更新
	void UpdateMinMax(const Vector3& point);
	// クォータニオンによって回転
	void Rotate(const Quaternion& q);
	bool Contains(const Vector3& point) const;
	float MinDistSq(const Vector3& point) const;

	Vector3		m_sMin;
	Vector3		m_sMax;

};

bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const LineSegment& l, const AABB& b, float outT, Vector3& outNorm);

#endif // !__COLLISION_H_INCLUDED__





