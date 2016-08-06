#ifndef _NS_COUNTABLESET_INL
# error CountableSet.inl should only be included by CountableSet.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension D>
CountableSet<T, D>::CountableSet() :
	mData(nullptr)
{
	static_assert(D > 0, "Dimension has to be greater then 0.");
	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are ComplexNumber and the types allowed by std::is_floating_point.");

	mData = std::make_shared<_Data>();
	memset(mData->ptr, 0, sizeof(T)*D);
}

template<typename T, Dimension D>
CountableSet<T, D>::CountableSet(const T& f) :
	CountableSet()
{
	fill(f);
}

template<typename T, Dimension D>
CountableSet<T, D>::CountableSet(const CountableSet<T, D>& v) :
	mData(v.mData)
{
}

template<typename T, Dimension D>
CountableSet<T, D>::CountableSet(CountableSet<T, D>&& v) :
	mData(std::move(v.mData))
{
}

template<typename T, Dimension D>
CountableSet<T, D>::~CountableSet()
{
}

// Private
template<typename T, Dimension D>
void CountableSet<T, D>::make_unique()
{
	if (!mData.unique())
	{
		std::shared_ptr<_Data> ptr = std::make_shared<_Data>();
		memcpy(ptr->ptr, mData->ptr, sizeof(T)*D);
		mData = std::move(ptr);
	}
}

// Assignment
template<typename T, Dimension D>
CountableSet<T, D>& CountableSet<T, D>::operator=(const CountableSet<T, D>& v)
{
	NS_ASSERT(v.mData);
	mData = v.mData;
	return *this;
}

template<typename T, Dimension D>
CountableSet<T, D>& CountableSet<T, D>::operator=(CountableSet<T, D>&& v)
{
	NS_ASSERT(v.mData);
	mData = std::move(v.mData);
	return *this;
}

// Index
template<typename T, Dimension D>
const T& CountableSet<T, D>::linear_at(Index i) const
{
	NS_ASSERT(i < D);
	return mData->ptr[i];
}

template<typename T, Dimension D>
void CountableSet<T, D>::linear_set(Index i, const T& v)
{
	make_unique();
	NS_ASSERT(i < D);
	mData->ptr[i] = v;
}

// Other
template<typename T, Dimension D>
T CountableSet<T, D>::sum() const
{
	T s = 0;
	for (Index i = 0; i < D; ++i)
		s += mData->ptr[i];

	return s;
}

template<typename T, Dimension D>
T CountableSet<T, D>::max() const
{
	T s = std::numeric_limits<T>::min();
	for (Index i = 0; i < D; ++i)
	{
		if (mData->ptr[i] > s)
			s = mData->ptr[i];
	}

	return s;
}

template<typename T, Dimension D>
T CountableSet<T, D>::min() const
{
	T s = std::numeric_limits<T>::max();
	for (Index i = 0; i < D; ++i)
	{
		if (mData->ptr[i] < s)
			s = mData->ptr[i];
	}

	return s;
}

template<typename T, Dimension D>
T CountableSet<T, D>::avg() const
{
	return sum() / size();// Overflow?
}

template<typename T, Dimension D>
void CountableSet<T, D>::inverse()
{
	NS_DEBUG_ASSERT(!hasZero());

	make_unique();
	for (Index i = 0; i < D; ++i)
		mData->ptr[i] = (T)1 / mData->ptr[i];
}

template<typename T, Dimension D>
bool CountableSet<T, D>::hasNaN() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (std::isnan(mData->ptr[i]))
			return true;
	}

	return false;
}

template<typename T, Dimension D>
bool CountableSet<T, D>::hasInf() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (std::isinf(mData->ptr[i]))
			return true;
	}

	return false;
}

template<typename T, Dimension D>
bool CountableSet<T, D>::hasZero() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (mData->ptr[i] == 0)// Epsilon?
			return true;
	}

	return false;
}

template<typename T, Dimension D>
void CountableSet<T, D>::fill(const T& f)
{
	make_unique();
	for (Index i = 0; i < D; ++i)
		mData->ptr[i] = f;
}

template<typename T, Dimension D>
void CountableSet<T, D>::swap(CountableSet<T, D>& v)
{
	std::swap(mData, v.mData);
}

NS_END_NAMESPACE