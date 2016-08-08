#pragma once

#include "Config.h"
#include "ComplexNumber.h"

#include <type_traits>
#include <vector>

NS_BEGIN_NAMESPACE

/** A utility class useful for big dimensions
 */
template<typename T, Dimension D>
class CountableSet
{
public:
	class CountableSetIterator
	{
	public:
		CountableSetIterator(const CountableSet<T, D>& vec, Index pos) :
			mPos(pos), mVec(vec)
		{}

		bool operator ==(const CountableSetIterator& other) const
		{
			return mPos == other.mPos;
		}

		bool operator !=(const CountableSetIterator& other) const
		{
			return !(*this == other);
		}

		T operator *() const
		{
			NS_ASSERT(mPos < D);
			return mVec.linear_at(mPos);
		}

		const CountableSetIterator& operator++ ()
		{
			++mPos;
			return *this;
		}

	private:
		Index mPos;
		const CountableSet<T, D>& mVec;
	};

	typedef CountableSetIterator iterator;

	CountableSet();
	explicit CountableSet(const T& f);
	virtual ~CountableSet();

	// Index
	const T& linear_at(Index i) const;
	void linear_set(Index i, const T& v);

	iterator begin() const
	{
		return CountableSetIterator(*this, 0);
	}

	iterator end() const
	{
		return CountableSetIterator(*this, D);
	}

	// Other
	constexpr Dimension size() const { return D; }
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