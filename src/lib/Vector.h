#pragma once

#include "CountableSet.h"

#include "Exceptions.h"

NS_BEGIN_NAMESPACE

/** A vector class useful for big dimensions
 */
template<typename T, class DC>
class Vector : public CountableSet<T, DC>
{
public:
	Vector();

	template<class TMP = DC>//Dynamic
	Vector(size_t size, typename std::enable_if<std::is_same<TMP, dynamic_container_t<T>>::value>::type* = nullptr);

	template<class TMP = DC>//Dynamic
	Vector(std::initializer_list<T> l, typename std::enable_if<std::is_same<TMP, dynamic_container_t<T>>::value>::type* = nullptr);
	
	template<class TMP = DC>//Fixed
	Vector(std::initializer_list<T> l, typename std::enable_if<!std::is_same<TMP, dynamic_container_t<T>>::value>::type* = nullptr);

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
	Vector left(Index i) const;
	Vector right(Index i) const;
	Vector mid(Index start, Index end) const;
};

// Element wise operations
template<typename T, class DC>
Vector<T,DC> operator +(const Vector<T,DC>& v1, const Vector<T,DC>& v2);
template<typename T, class DC>
Vector<T,DC> operator +(const Vector<T,DC>& v1, T f);
template<typename T, class DC>
Vector<T,DC> operator +(T f, const Vector<T,DC>& v1);
template<typename T, class DC>
Vector<T,DC> operator -(const Vector<T,DC>& v1, const Vector<T,DC>& v2);
template<typename T, class DC>
Vector<T,DC> operator -(const Vector<T,DC>& v1, T f);
template<typename T, class DC>
Vector<T,DC> operator -(T f, const Vector<T,DC>& v1);
template<typename T, class DC>
Vector<T,DC> operator -(const Vector<T,DC>& v);
template<typename T, class DC>
Vector<T,DC> operator *(const Vector<T,DC>& v1, const Vector<T,DC>& v2);
template<typename T, class DC>
Vector<T,DC> operator *(const Vector<T,DC>& v1, T f);
template<typename T, class DC>
Vector<T,DC> operator *(T f, const Vector<T,DC>& v1);
template<typename T, class DC>
Vector<T,DC> operator /(const Vector<T,DC>& v1, const Vector<T,DC>& v2);
template<typename T, class DC>
Vector<T,DC> operator /(const Vector<T,DC>& v1, T f);
template<typename T, class DC>
Vector<T,DC> operator /(T f, const Vector<T,DC>& v1);

// Comparison
template<typename T, class DC>
bool operator ==(const Vector<T,DC>& v1, const Vector<T,DC>& v2);
template<typename T, class DC>
bool operator !=(const Vector<T,DC>& v1, const Vector<T,DC>& v2);

// Typedefs
template<typename T>
using DynamicVector = Vector<T, dynamic_container_t<T> >;

template<typename T, Dimension N>
using FixedVector = Vector<T, fixed_container_t<T,N> >;

template<typename T>
using Vector2D = FixedVector<T, 2>;
template<typename T>
using Vector3D = FixedVector<T, 3>;
template<typename T>
using Vector4D = FixedVector<T, 4>;

// Type traits
template<template<typename,class> class S, typename T, class DC>
struct is_dynamic_vector : std::integral_constant<bool,
	std::is_same<DynamicVector<T>, typename std::remove_cv<class S<T,DC> >::type>::value > {};

template<template<typename,class> class S, typename T, class DC>
struct is_fixed_vector : std::integral_constant<bool,
	!is_dynamic_vector<S,T,DC>::value > {};

NS_END_NAMESPACE

#define _NS_VECTOR_INL
# include "Vector.inl"
#undef _NS_VECTOR_INL