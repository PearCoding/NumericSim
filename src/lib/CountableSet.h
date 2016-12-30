#pragma once

#include "Types.h"

#include <vector>
#include <array>
#include <limits>

NS_BEGIN_NAMESPACE

template<typename T>
using dynamic_container_t = std::vector<T>;

template<typename T, Dimension N>
using fixed_container_t = std::array<T, N>;

template<typename T, class DC>
class CountableSet;

template<typename T, class DC>
class CountableSetIterator
{
	friend CountableSet<T, DC>;

private:
	CountableSetIterator(const CountableSet<T, DC>& vec, Index pos) :
		mPos(pos), mVec(vec)
	{}
public:
	bool isValid() const
	{
		return mPos < mVec.size();
	}

	bool operator ==(const CountableSetIterator& other) const
	{
		return (!isValid() && !other.isValid()) || mPos == other.mPos;
	}

	bool operator !=(const CountableSetIterator& other) const
	{
		return !(*this == other);
	}

	T operator *() const
	{
		NS_ASSERT(mPos < mVec.size());
		return mVec.linear_at(mPos);
	}

	const CountableSetIterator& operator++ ()
	{
		++mPos;
		return *this;
	}

	const CountableSetIterator operator++ (int)
	{
		auto it = *this;
		this->operator++();
		return it;
	}
private:
	Index mPos;
	const CountableSet<T,DC>& mVec;
};

/** A utility class useful for big dimensions
 */
template<typename T, class DC>
class CountableSet
{
public:
	typedef CountableSetIterator<T, DC> iterator;

	CountableSet();
	
	template<class TMP = DC>
	CountableSet(size_t size, typename std::enable_if<std::is_same<TMP, dynamic_container_t<T>>::value>::type* = nullptr);
	virtual ~CountableSet();

	// Index
	const T& linear_at(Index i) const;
	const T& operator[](Index i) const;
	
	void linear_set(Index i, const T& v);
	T& operator[](Index i);

	iterator begin() const
	{
		return iterator(*this, 0);
	}

	iterator end() const
	{
		return iterator(*this, size());
	}

	// Other
	size_t size() const;

	T sum() const;
	T max() const;
	T min() const;
	T avg() const;

	void do_reciprocal();

	void fill(const T& f);
	void swap(CountableSet& v);

	bool hasNaN() const;
	bool hasInf() const;
	bool hasZero() const;

	template<class TMP = DC>
	typename std::enable_if<std::is_same<TMP, dynamic_container_t<T>>::value>::type 
	resize(Dimension size);

protected:
	DC mData;
};

// Typedefs
template<typename T>
using DynamicCountableSet = CountableSet<T, dynamic_container_t<T> >;

template<typename T, Dimension N>
using FixedCountableSet = CountableSet<T, fixed_container_t<T,N> >;

// Type traits
template<template<typename,class> class S, typename T, class DC>
struct is_dynamic_countable_set : std::integral_constant<bool,
	std::is_same<DynamicCountableSet<T>, typename std::remove_cv<class S<T,DC> >::type>::value > {};

template<template<typename,class> class S, typename T, class DC>
struct is_fixed_countable_set : std::integral_constant<bool,
	!is_dynamic_countable_set<S,T,DC>::value > {};

NS_END_NAMESPACE

#define _NS_COUNTABLESET_INL
# include "CountableSet.inl"
#undef _NS_COUNTABLESET_INL