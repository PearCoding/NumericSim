#pragma once

#include "Config.h"
#include "CountableSet.h"

#include "Exceptions.h"

NS_BEGIN_NAMESPACE

/** A vector class useful for big dimensions
 */
template<typename T>
class Vector : public CountableSet<T>
{
public:
	Vector();
	explicit Vector(Dimension size);
	Vector(Dimension size, const T& f);
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
	T dot(const Vector& v) const;
	T mag() const;
	T magSqr() const;

	// Boolean Set
	Vector<T> left(Index i) const;
	Vector<T> right(Index i) const;
	Vector<T> mid(Index start, Index end) const;
};

// Element wise operations
template<typename T>
Vector<T> operator +(const Vector<T>& v1, const Vector<T>& v2);
template<typename T>
Vector<T> operator +(const Vector<T>& v1, T f);
template<typename T>
Vector<T> operator +(T f, const Vector<T>& v1);
template<typename T>
Vector<T> operator -(const Vector<T>& v1, const Vector<T>& v2);
template<typename T>
Vector<T> operator -(const Vector<T>& v1, T f);
template<typename T>
Vector<T> operator -(T f, const Vector<T>& v1);
template<typename T>
Vector<T> operator -(const Vector<T>& v);
template<typename T>
Vector<T> operator *(const Vector<T>& v1, const Vector<T>& v2);
template<typename T>
Vector<T> operator *(const Vector<T>& v1, T f);
template<typename T>
Vector<T> operator *(T f, const Vector<T>& v1);
template<typename T>
Vector<T> operator /(const Vector<T>& v1, const Vector<T>& v2);
template<typename T>
Vector<T> operator /(const Vector<T>& v1, T f);
template<typename T>
Vector<T> operator /(T f, const Vector<T
>& v1);

// Comparison
template<typename T>
bool operator ==(const Vector<T>& v1, const Vector<T>& v2);
template<typename T, Dimension D>
bool operator !=(const Vector<T>& v1, const Vector<T>& v2);

NS_END_NAMESPACE

#define _NS_VECTOR_INL
# include "Vector.inl"
#undef _NS_VECTOR_INL