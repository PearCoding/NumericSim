#ifndef _NS_SPARSECOUNTABLESET_INL
# error SparseCountableSet.inl should only be included by SparseCountableSet.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension D>
SparseCountableSet<T, D>::SparseCountableSet() :
	mData()
{
	static_assert(D > 0, "Dimension has to be greater then 0.");
	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are ComplexNumber and the types allowed by std::is_floating_point.");
}

template<typename T, Dimension D>
SparseCountableSet<T, D>::SparseCountableSet(const SparseCountableSet<T, D>& v) :
	mData(v.mData)
{
}

template<typename T, Dimension D>
SparseCountableSet<T, D>::SparseCountableSet(SparseCountableSet<T, D>&& v) :
	mData(std::move(v.mData))
{
}

template<typename T, Dimension D>
SparseCountableSet<T, D>::~SparseCountableSet()
{
}

// Assignment
template<typename T, Dimension D>
SparseCountableSet<T, D>& SparseCountableSet<T, D>::operator=(const SparseCountableSet<T, D>& v)
{
	mData = v.mData;
	return *this;
}

template<typename T, Dimension D>
SparseCountableSet<T, D>& SparseCountableSet<T, D>::operator=(SparseCountableSet<T, D>&& v)
{
	mData = std::move(v.mData);
	return *this;
}

// Index
template<typename T, Dimension D>
T SparseCountableSet<T, D>::linear_at(Index i) const
{
	NS_ASSERT(i < D);
	if (mData.count(i))
		return mData.at(i);
	else
		return 0;
}

template<typename T, Dimension D>
void SparseCountableSet<T, D>::linear_set(Index i, const T& v)
{
	NS_ASSERT(i < D);
	mData[i] = v;
}

// Other
template<typename T, Dimension D>
constexpr Dimension SparseCountableSet<T, D>::size() const
{
	return D;
}

template<typename T, Dimension D>
Dimension SparseCountableSet<T, D>::entries() const
{
	return mData.size();
}

template<typename T, Dimension D>
bool SparseCountableSet<T, D>::isEmpty() const
{
	return mData.empty();
}

template<typename T, Dimension D>
T SparseCountableSet<T, D>::sum() const
{
	T s = 0;
	for (auto l : mData)
		s += l;

	return s;
}

template<typename T, Dimension D>
T SparseCountableSet<T, D>::max() const
{
	T s = std::numeric_limits<T>::min();
	for (auto l : mData)
	{
		if (l > s)
			s = l;
	}

	if (mData.size() != size() && 0 > s)
		s = 0;

	return s;
}

template<typename T, Dimension D>
T SparseCountableSet<T, D>::min() const
{
	T s = std::numeric_limits<T>::max();
	for (auto l : mData)
	{
		if (l < s)
			s = l;
	}

	if (mData.size() != size() && 0 < s)
		s = 0;

	return s;
}

template<typename T, Dimension D>
T SparseCountableSet<T, D>::avg() const
{
	return sum() / size();// Overflow?
}

template<typename T, Dimension D>
bool SparseCountableSet<T, D>::hasNaN() const
{
	for (auto l : mData)
	{
		if (std::isnan(l))
			return true;
	}

	return false;
}

template<typename T, Dimension D>
bool SparseCountableSet<T, D>::hasInf() const
{
	for (auto l : mData)
	{
		if (std::isinf(l))
			return true;
	}

	return false;
}

template<typename T, Dimension D>
bool SparseCountableSet<T, D>::hasZero() const
{
	return mData.size() != size();
}

template<typename T, Dimension D>
void SparseCountableSet<T, D>::fill(const T& f)
{
	for (Index i = 0; i < D; ++i)
		mData[i] = f;
}

template<typename T, Dimension D>
void SparseCountableSet<T, D>::swap(SparseCountableSet<T, D>& v)
{
	std::swap(mData, v.mData);
}

NS_END_NAMESPACE