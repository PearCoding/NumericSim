#pragma once

#include "Config.h"
#include "CountableSet.h"

NS_BEGIN_NAMESPACE

/** A vector class useful for big dimensions
 */
template<typename T, Dimension D>
class Vector : public CountableSet<T, D>
{
public:
	Vector();
	explicit Vector(const T& f);
	Vector(std::initializer_list<T> l);

	virtual ~Vector();

	// Element wise operations
	Vector& operator +=(const Vector& v2);
	Vector& operator +=(const T& f);
	Vector& operator -=(const Vector& v2);
	Vector& operator -=(const T& f);
	Vector& operator *=(const Vector& v2);
	Vector& operator *=(const T& f);
	Vector& operator /=(const Vector& v2);
	Vector& operator /=(const T& f);

	const T& at(Index i) const;
	void set(Index i, const T& v);

	// Other
	constexpr Dimension dim() const;
	T dot(const Vector& v) const;
	T mag() const;
	T magSqr() const;

	// Boolean Set
	template<Index I>
	Vector<T, I + 1> left() const;
	template<Dimension MaxD = D>
	Vector<T, MaxD> left(Index i) const;

	template<Index I>
	Vector<T, D - I> right() const;
	template<Dimension MaxD = D>
	Vector<T, MaxD> right(Index i) const;

	template<Index Start, Index End>
	Vector<T, End - Start> mid() const;
	template<Dimension MaxD = D>
	Vector<T, MaxD> mid(Index start, Index end) const;
};

// Element wise operations
template<typename T, Dimension D>
Vector<T, D> operator +(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
Vector<T, D> operator +(const Vector<T, D>& v1, T f);
template<typename T, Dimension D>
Vector<T, D> operator +(T f, const Vector<T, D>& v1);
template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v1, T f);
template<typename T, Dimension D>
Vector<T, D> operator -(T f, const Vector<T, D>& v1);
template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v);
template<typename T, Dimension D>
Vector<T, D> operator *(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
Vector<T, D> operator *(const Vector<T, D>& v1, T f);
template<typename T, Dimension D>
Vector<T, D> operator *(T f, const Vector<T, D>& v1);
template<typename T, Dimension D>
Vector<T, D> operator /(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
Vector<T, D> operator /(const Vector<T, D>& v1, T f);
template<typename T, Dimension D>
Vector<T, D> operator /(T f, const Vector<T, D>& v1);

// Comparison
template<typename T, Dimension D>
bool operator ==(const Vector<T, D>& v1, const Vector<T, D>& v2);
template<typename T, Dimension D>
bool operator !=(const Vector<T, D>& v1, const Vector<T, D>& v2);

typedef Vector<float, 2> Vector2f;
typedef Vector<float, 3> Vector3f;
typedef Vector<double, 2> Vector2;
typedef Vector<double, 3> Vector3;

NS_END_NAMESPACE

#define _NS_VECTOR_INL
# include "Vector.inl"
#undef _NS_VECTOR_INL