#ifndef _NS_COUNTABLESET_INL
# error CountableSet.inl should only be included by CountableSet.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension D>
CountableSet<T, D>::CountableSet() :
	mData(D, (T)0)
{
	static_assert(D > 0, "Dimension has to be greater then 0.");
	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are ComplexNumber and the types allowed by std::is_floating_point.");
}

template<typename T, Dimension D>
CountableSet<T, D>::CountableSet(const T& f) :
	CountableSet()
{
	fill(f);
}

template<typename T, Dimension D>
CountableSet<T, D>::~CountableSet()
{
}

// Index
template<typename T, Dimension D>
const T& CountableSet<T, D>::linear_at(Index i) const
{
	NS_ASSERT(i < D);
	return mData[i];
}

template<typename T, Dimension D>
void CountableSet<T, D>::linear_set(Index i, const T& v)
{
	NS_ASSERT(i < D);
	mData[i] = v;
}

// Other
template<typename T, Dimension D>
T CountableSet<T, D>::sum() const
{
	T s = 0;
	for (Index i = 0; i < D; ++i)
		s += mData[i];

	return s;
}

template<typename T, Dimension D>
T CountableSet<T, D>::max() const
{
	T s = (T)std::numeric_limits<typename get_complex_internal<T>::type>::min();
	for (Index i = 0; i < D; ++i)
	{
		if (mData[i] > s)
			s = mData[i];
	}

	return s;
}

template<typename T, Dimension D>
T CountableSet<T, D>::min() const
{
	T s = (T)std::numeric_limits<typename get_complex_internal<T>::type>::max();
	for (Index i = 0; i < D; ++i)
	{
		if (mData[i] < s)
			s = mData[i];
	}

	return s;
}

template<typename T, Dimension D>
T CountableSet<T, D>::avg() const
{
	return sum() / size();// Overflow?
}

template<typename T, Dimension D>
void CountableSet<T, D>::do_reciprocal()
{
	NS_DEBUG_ASSERT(!hasZero());

	for (Index i = 0; i < D; ++i)
		mData[i] = (T)1 / mData[i];
}

template<typename T, Dimension D>
bool CountableSet<T, D>::hasNaN() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (std::isnan(mData[i]))
			return true;
	}

	return false;
}

template<typename T, Dimension D>
bool CountableSet<T, D>::hasInf() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (std::isinf(mData[i]))
			return true;
	}

	return false;
}

template<typename T, Dimension D>
bool CountableSet<T, D>::hasZero() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (mData[i] == (T)0)// Epsilon?
			return true;
	}

	return false;
}

template<typename T, Dimension D>
void CountableSet<T, D>::fill(const T& f)
{
	for (Index i = 0; i < D; ++i)
		mData[i] = f;
}

template<typename T, Dimension D>
void CountableSet<T, D>::swap(CountableSet<T, D>& v)
{
	std::swap(mData, v.mData);
}

NS_END_NAMESPACE