#pragma once

#include "Config.h"

#include <initializer_list>
#include <type_traits>
#include <unordered_map>

NS_BEGIN_NAMESPACE

/** A utility class useful for sparse but big dimensional data
 */
template<typename T, Dimension D>
class SparseCountableSet
{
public:
	class SparseCountableSetIterator
	{
	private:
		friend SparseCountableSet;

		typedef typename std::unordered_map<Index, T>::const_iterator& map_iterator;
		SparseCountableSetIterator(map_iterator itr) :
			mItr(itr)
		{}

	public:
		bool operator ==(const SparseCountableSetIterator& other) const
		{
			return mItr == other.mItr;
		}

		bool operator !=(const SparseCountableSetIterator& other) const
		{
			return !(*this == other);
		}

		T operator *() const
		{
			return mItr->second;
		}

		const SparseCountableSetIterator& operator++ ()
		{
			++mItr;
			return *this;
		}

	private:
		map_iterator mItr;
	};

	typedef SparseCountableSetIterator iterator;

	SparseCountableSet();
	SparseCountableSet(const SparseCountableSet& v);
	SparseCountableSet(SparseCountableSet&& v);
	virtual ~SparseCountableSet();

	// Assignment
	SparseCountableSet& operator=(const SparseCountableSet& v);
	SparseCountableSet& operator=(SparseCountableSet&& v);

	// Index
	T linear_at(Index i) const;
	void linear_set(Index i, const T& v);

	iterator begin() const
	{
		return iterator(mData.begin());
	}

	iterator end() const
	{
		return iterator(mData.end());
	}

	// Other
	constexpr Dimension size() const;
	Dimension entries() const;

	T sum() const;
	T max() const;
	T min() const;
	T avg() const;

	void fill(const T& f);
	void swap(SparseCountableSet& v);

	bool hasNaN() const;
	bool hasInf() const;
	bool hasZero() const;

	bool isEmpty() const;// everything is zero!

protected:
	std::unordered_map<Index, T> mData;
};

NS_END_NAMESPACE

#define _NS_SPARSECOUNTABLESET_INL
# include "SparseCountableSet.inl"
#undef _NS_SPARSECOUNTABLESET_INL