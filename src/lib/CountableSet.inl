#ifndef _NS_COUNTABLESET_INL
# error CountableSet.inl should only be included by CountableSet.h
#endif

NS_BEGIN_NAMESPACE

template<typename T>
CountableSet<T>::CountableSet() :
	CountableSet(1, (T)0)
{
}

template<typename T>
CountableSet<T>::CountableSet(Dimension size) :
	CountableSet(size, (T)0)
{
}

template<typename T>
CountableSet<T>::CountableSet(Dimension size, const T& f) :
	mData(size, f)
{
	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are ComplexNumber and the types allowed by std::is_floating_point.");
}

template<typename T>
CountableSet<T>::~CountableSet()
{
}

template<typename T>
void CountableSet<T>::resize(Dimension size)
{
	NS_ASSERT(size > 0);
	mData.resize(size, (T)0);
}
// Index
template<typename T>
const T& CountableSet<T>::linear_at(Index i) const
{
	NS_ASSERT(i < size());
	return mData[i];
}

template<typename T>
void CountableSet<T>::linear_set(Index i, const T& v)
{
	NS_ASSERT(i < size());
	mData[i] = v;
}

// Other
template<typename T>
T CountableSet<T>::sum() const
{
	T s = 0;
	for (Index i = 0; i < size(); ++i)
		s += mData[i];

	return s;
}

template<typename T>
T CountableSet<T>::max() const
{
	typedef typename get_complex_internal<T>::type _t;

	_t s = std::numeric_limits<_t>::min();
	for (Index i = 0; i < size(); ++i)
	{
		_t d = mag_by_complex_vt(mData[i]);
		if (d > s)
			s = d;
	}

	return (T)s;
}

template<typename T>
T CountableSet<T>::min() const
{
	typedef typename get_complex_internal<T>::type _t;

	_t s = std::numeric_limits<_t>::max();
	for (Index i = 0; i < size(); ++i)
	{
		_t d = mag_by_complex_vt(mData[i]);
		if (d < s)
			s = d;
	}

	return s;
}

template<typename T>
T CountableSet<T>::avg() const
{
	return sum() / (T)size();// Overflow?
}

template<typename T>
void CountableSet<T>::do_reciprocal()
{
	NS_DEBUG_ASSERT(!hasZero());

	for (Index i = 0; i < size(); ++i)
		mData[i] = (T)1 / mData[i];
}

template<typename T>
bool CountableSet<T>::hasNaN() const
{
	for (Index i = 0; i < size(); ++i)
	{
		if (std::isnan(mData[i]))
			return true;
	}

	return false;
}

template<typename T>
bool CountableSet<T>::hasInf() const
{
	for (Index i = 0; i < size(); ++i)
	{
		if (std::isinf(mData[i]))
			return true;
	}

	return false;
}

template<typename T>
bool CountableSet<T>::hasZero() const
{
	for (Index i = 0; i < size(); ++i)
	{
		if (mData[i] == (T)0)// Epsilon?
			return true;
	}

	return false;
}

template<typename T>
void CountableSet<T>::fill(const T& f)
{
	for (Index i = 0; i < size(); ++i)
		mData[i] = f;
}

template<typename T>
void CountableSet<T>::swap(CountableSet<T>& v)
{
	std::swap(mData, v.mData);
}

NS_END_NAMESPACE