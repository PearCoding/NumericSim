#pragma once

#include "Config.h"
#include "ComplexNumber.h"

#include <type_traits>
#include <vector>
#include <limits>

NS_BEGIN_NAMESPACE

template<typename T>
class CountableSet;

template<typename T>
class CountableSetIterator
{
	friend CountableSet<T>;

private:
	CountableSetIterator(const CountableSet<T>& vec, Index pos) :
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
		auto it = *it;
		this->operator++();
		return it;
	}
private:
	Index mPos;
	const CountableSet<T>& mVec;
};

/** A utility class useful for big dimensions
 */
template<typename T>
class CountableSet
{
public:
	typedef CountableSetIterator<T> iterator;

	CountableSet();
	explicit CountableSet(Dimension size);
	CountableSet(Dimension size, const T& f);
	virtual ~CountableSet();

	void resize(Dimension size);

	// Index
	const T& linear_at(Index i) const;
	void linear_set(Index i, const T& v);

	iterator begin() const
	{
		return iterator(*this, 0);
	}

	iterator end() const
	{
		return iterator(*this, size());
	}

	// Other
	Dimension size() const { return mData.size(); }
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

protected:
	std::vector<T> mData;
};

NS_END_NAMESPACE

#define _NS_COUNTABLESET_INL
# include "CountableSet.inl"
#undef _NS_COUNTABLESET_INL