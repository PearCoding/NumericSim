#ifndef _NS_COUNTABLESET_INL
# error CountableSet.inl should only be included by CountableSet.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, class DC>
CountableSet<T, DC>::CountableSet()
{
	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are ComplexNumber and the types allowed by std::is_arithmetic.");
}

template<typename T, class DC>
template<class TMP>
CountableSet<T, DC>::CountableSet(size_t size, typename std::enable_if<std::is_same<TMP, dynamic_container_t<T>>::value>::type*) :
	CountableSet()
{
	this->mData.resize(size);
}

template<typename T, class DC>
CountableSet<T, DC>::~CountableSet()
{
}

// Index
template<typename T, class DC>
const T& CountableSet<T, DC>::linear_at(Index i) const
{
	NS_ASSERT(i < size());
	return mData[i];
}

template<typename T, class DC>
const T& CountableSet<T, DC>::operator[](Index i) const
{
	return linear_at(i);
}

template<typename T, class DC>
void CountableSet<T, DC>::linear_set(Index i, const T& v)
{
	NS_ASSERT(i < size());
	mData[i] = v;
}

template<typename T, class DC>
T& CountableSet<T, DC>::operator[](Index i)
{
	NS_ASSERT(i < size());
	return mData[i];
}

// Other
template<typename T, class DC>
size_t CountableSet<T, DC>::size() const
{
	return mData.size();
}

template<typename T, class DC>
T CountableSet<T, DC>::sum() const
{
	T s = 0;
	for (Index i = 0; i < size(); ++i)
		s += mData[i];

	return s;
}

template<typename T, class DC>
T CountableSet<T, DC>::max() const
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

template<typename T, class DC>
T CountableSet<T, DC>::min() const
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

template<typename T, class DC>
T CountableSet<T, DC>::avg() const
{
	return sum() / (T)size();// Overflow?
}

template<typename T, class DC>
void CountableSet<T, DC>::do_reciprocal()
{
	NS_DEBUG_ASSERT(!hasZero());

	for (Index i = 0; i < size(); ++i)
		mData[i] = (T)1 / mData[i];
}

template<typename T, class DC>
bool CountableSet<T, DC>::hasNaN() const
{
	for (Index i = 0; i < size(); ++i)
	{
		if (std::isnan(mData[i]))
			return true;
	}

	return false;
}

template<typename T, class DC>
bool CountableSet<T, DC>::hasInf() const
{
	for (Index i = 0; i < size(); ++i)
	{
		if (std::isinf(mData[i]))
			return true;
	}

	return false;
}

template<typename T, class DC>
bool CountableSet<T, DC>::hasZero() const
{
	for (Index i = 0; i < size(); ++i)
	{
		if (mData[i] == (T)0)// Epsilon?
			return true;
	}

	return false;
}

template<typename T, class DC>
void CountableSet<T, DC>::fill(const T& f)
{
	for (Index i = 0; i < size(); ++i)
		mData[i] = f;
}

template<typename T, class DC>
void CountableSet<T, DC>::swap(CountableSet<T, DC>& v)
{
	std::swap(mData, v.mData);
}

template<typename T, class DC>
template<class TMP>
typename std::enable_if<std::is_same<TMP, dynamic_container_t<T> >::value>::type 
CountableSet<T, DC>::resize(Dimension size)
{
	this->mData.resize(size);
}

NS_END_NAMESPACE