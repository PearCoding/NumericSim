#pragma once

#include "Types.h"

NS_BEGIN_NAMESPACE

template<typename T>
class Triangle2D
{
public:
	T v1[2];
	T v2[2];
	T v3[2];
	
	Triangle2D() :
		v1{0,0}, v2{0,0}, v3{0,0}
	{
	}
	
	Triangle2D(T x1, T y1, T x2, T y2, T x3, T y3) :
		v1{x1,y1}, v2{x2,y2}, v3{x3,y3}
	{
	}

	inline T area() const
	{
		return ((T)0.5) * std::abs(v1[0] * (v2[1] - v3[1]) +
			v2[0] * (v3[1] - v1[1]) +
			v3[0] * (v1[1] - v2[1]));
	}

	Triangle2D gradient() const
	{
		Triangle2D<T> t;
		t.v1[0] = v2[1] - v3[1];
		t.v1[1] = v3[0] - v2[0];

		t.v2[0] = v3[1] - v1[1];
		t.v2[1] = v1[0] - v3[0];

		t.v3[0] = v1[1] - v2[1];
		t.v3[1] = v2[0] - v1[0];

		return t;
	}
};

NS_END_NAMESPACE