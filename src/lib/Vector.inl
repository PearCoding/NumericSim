#ifndef _NS_VECTOR_INL
# error Vector.inl should only be included by Vector.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension D>
Vector<T, D>::Vector() :
	mData(nullptr)
{
	static_assert(D > 0, "Dimension has to be greater then 0.");
	static_assert(std::is_scalar<T>::value, "Type T has to be a scalar type.\nSee C++ reference of std::is_scalar for more information.");

	mData = std::make_shared<_Data>();
	memset(mData->ptr, 0, sizeof(T)*D);
}

template<typename T, Dimension D>
Vector<T, D>::Vector(const T& f) :
	Vector()
{
	fill(f);
}

template<typename T, Dimension D>
Vector<T, D>::Vector(std::initializer_list<T> l) : 
	Vector()
{
	NS_ASSERT(l.size() <= D);// Would be great if we could use size() with constexpr [C++14]
	Index i = 0;
	for (const T& v : l)
	{
		mData->ptr[i] = v;
		i++;
	}
}

template<typename T, Dimension D>
Vector<T, D>::Vector(const Vector<T, D>& v) :
	mData(v.mData)
{
}

template<typename T, Dimension D>
Vector<T, D>::Vector(Vector<T, D>&& v) :
	mData(std::move(v.mData))
{
}

template<typename T, Dimension D>
Vector<T, D>::~Vector()
{
}

// Private
template<typename T, Dimension D>
void Vector<T, D>::make_unique()
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
Vector<T, D>& Vector<T, D>::operator=(const Vector<T, D>& v)
{
	NS_ASSERT(v.mData);
	mData = v.mData;
	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator=(Vector<T, D>&& v)
{
	NS_ASSERT(v.mData);
	mData = std::move(v.mData);
	return *this;
}

// Index
template<typename T, Dimension D>
const T& Vector<T, D>::operator()(Index i) const
{
	NS_ASSERT(i < D);
	return mData->ptr[i];
}

template<typename T, Dimension D>
const T& Vector<T, D>::operator[](Index i) const
{
	NS_ASSERT(i < D);
	return mData->ptr[i];
}

template<typename T, Dimension D>
T& Vector<T, D>::operator[](Index i)
{
	make_unique();

	NS_ASSERT(i < D);
	return mData->ptr[i];
}

// Elementwise operations
template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator +=(const Vector<T, D>& v2)
{
	make_unique();

	for (Index i = 0; i < D; ++i)
		mData->ptr[i] += v2(i);

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator +=(const T& f)
{
	make_unique();

	for (Index i = 0; i < D; ++i)
		mData->ptr[i] += f;

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator -=(const Vector<T, D>& v2)
{
	make_unique();

	for (Index i = 0; i < D; ++i)
		mData->ptr[i] -= v2(i);

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator -=(const T& f)
{
	make_unique();

	for (Index i = 0; i < D; ++i)
		mData->ptr[i] -= f;

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator *=(const Vector<T, D>& v2)
{
	make_unique();

	for (Index i = 0; i < D; ++i)
		mData->ptr[i] *= v2(i);

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator *=(const T& f)
{
	make_unique();

	for (Index i = 0; i < D; ++i)
		mData->ptr[i] *= f;

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator /=(const Vector<T, D>& v2)
{
	make_unique();

	for (Index i = 0; i < D; ++i)
		mData->ptr[i] /= v2(i);

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator /=(const T& f)
{
	make_unique();

	T invF = 1 / f;// TODO: NAN?
	for (Index i = 0; i < D; ++i)
		mData->ptr[i] *= invF;

	return *this;
}

// Other
template<typename T, Dimension D>
constexpr Dimension Vector<T, D>::dim() const
{
	return D;
}

template<typename T, Dimension D>
T Vector<T, D>::sum() const
{
	T s = 0;
	for (Index i = 0; i < D; ++i)
		s += mData->ptr[i];

	return s;
}

template<typename T, Dimension D>
T Vector<T, D>::max() const
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
T Vector<T, D>::min() const
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
T Vector<T, D>::avg() const
{
	return sum() / dim();// Overflow?
}

template<typename T, Dimension D>
T Vector<T, D>::dot(const Vector& v) const
{
	T s = 0;
	for (Index i = 0; i < D; ++i)
		s += mData->ptr[i] * v.mData->ptr[i];

	return s;
}

template<typename T, Dimension D>
T Vector<T, D>::mag() const
{
	return std::sqrt(magSqr());
}

template<typename T, Dimension D>
T Vector<T, D>::magSqr() const
{
	return dot(*this);
}

template<typename T, Dimension D>
void Vector<T, D>::inv()
{
	NS_DEBUG_ASSERT(!hasZero());

	make_unique();
	for (Index i = 0; i < D; ++i)
		mData->ptr[i] = 1 / mData->ptr[i];
}

template<typename T, Dimension D>
bool Vector<T, D>::hasNaN() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (std::isnan(mData->ptr[i]))
			return true;
	}

	return false;
}

template<typename T, Dimension D>
bool Vector<T, D>::hasInf() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (std::isinf(mData->ptr[i]))
			return true;
	}

	return false;
}

template<typename T, Dimension D>
bool Vector<T, D>::hasZero() const
{
	for (Index i = 0; i < D; ++i)
	{
		if (mData->ptr[i] == 0)// Epsilon?
			return true;
	}

	return false;
}

template<typename T, Dimension D>
void Vector<T, D>::fill(const T& f)
{
	make_unique();
	for (Index i = 0; i < D; ++i)
		mData->ptr[i] = f;
}

// Boolean set operations
template<typename T, Dimension D>
template<Index I>
Vector<T, I + 1> Vector<T, D>::left() const
{
	static_assert(I < D, "Given left index exceeds dimension of vector.");
	Vector<T, I + 1> tmp;

	for (Index i = 0; i <= I; ++i)
		tmp[i] = mData->ptr[i];

	return tmp;
}

template<typename T, Dimension D>
template<Dimension MaxD>
Vector<T, MaxD> Vector<T, D>::left(Index i) const
{
	static_assert(MaxD > 0, "Max dimension has to bigger then 0.");
	NS_ASSERT(i < D);

	Index start = 0;
	if (MaxD < i + 1)
		start = i - MaxD + 1;

	Vector<T, MaxD> tmp;
	for (Index j = start; j < i + 1; ++j)
		tmp[j - start] = mData->ptr[j];

	return tmp;
}

template<typename T, Dimension D>
template<Index I>
Vector<T, D - I> Vector<T, D>::right() const
{
	static_assert(I < D, "Given right index exceeds dimension of vector.");
	Vector<T, D - I> tmp;

	for (Index i = I; i < D; ++i)
		tmp[i - I] = mData->ptr[i];

	return tmp;
}

template<typename T, Dimension D>
template<Dimension MaxD>
Vector<T, MaxD> Vector<T, D>::right(Index i) const
{
	static_assert(MaxD > 0, "Max dimension has to bigger then 0.");
	NS_ASSERT(i < D);
	
	Vector<T, MaxD> tmp;
	for (Index j = i; j < (i + MaxD) && j < D; ++j)
		tmp[j - i] = mData->ptr[j];

	return tmp;
}

template<typename T, Dimension D>
template<Index Start, Index End>
Vector<T, End - Start> Vector<T, D>::mid() const
{
	static_assert(Start < End, "Start index has to be lower then End index.");

	Vector<T, End - Start> tmp;
	for (Index i = Start; i < End && i < D; ++i)
		tmp[i - Start] = mData->ptr[i];

	return tmp;
}

template<typename T, Dimension D>
template<Dimension MaxD>
Vector<T, MaxD> Vector<T, D>::mid(Index start, Index end) const
{
	static_assert(MaxD > 0, "Max dimension has to bigger then 0.");
	NS_ASSERT(start < end);

	Vector<T, MaxD> tmp;
	for (Index i = start; i < end && i < D && i < (MaxD + start); ++i)
		tmp[i - start] = mData->ptr[i];

	return tmp;
}

template<typename T, Dimension D>
void Vector<T, D>::swap(Vector<T, D>& v)
{
	std::swap(mData, v.mData);
}

// Non member functions
// Element wise operations
template<typename T, Dimension D>
Vector<T, D> operator +(const Vector<T, D>& v1, const Vector<T, D>& v2)
{
	Vector<T, D> tmp = v1;
	return (tmp += v2);
}

template<typename T, Dimension D>
Vector<T, D> operator +(const Vector<T, D>& v1, T f)
{
	Vector<T, D> tmp = v1;
	return (tmp += f);
}

template<typename T, Dimension D>
Vector<T, D> operator +(T f, const Vector<T, D>& v1)
{
	return v1 + f;
}

template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v1, const Vector<T, D>& v2)
{
	Vector<T, D> tmp = v1;
	return (tmp -= v2);
}

template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v1, T f)
{
	Vector<T, D> tmp = v1;
	return (tmp -= f);
}

template<typename T, Dimension D>
Vector<T, D> operator -(T f, const Vector<T, D>& v1)
{
	Vector<T, D> tmp = -v1;
	return (tmp += f);
}

template<typename T, Dimension D>
Vector<T, D> operator -(const Vector<T, D>& v)
{
	Vector<T, D> tmp = v;
	return (tmp *= -1);
}

template<typename T, Dimension D>
Vector<T, D> operator *(const Vector<T, D>& v1, const Vector<T, D>& v2)
{
	Vector<T, D> tmp = v1;
	return (tmp *= v2);
}

template<typename T, Dimension D>
Vector<T, D> operator *(const Vector<T, D>& v1, T f)
{
	Vector<T, D> tmp = v1;
	return (tmp *= f);
}

template<typename T, Dimension D>
Vector<T, D> operator *(T f, const Vector<T, D>& v1)
{
	return v1 * f;
}

template<typename T, Dimension D>
Vector<T, D> operator /(const Vector<T, D>& v1, const Vector<T, D>& v2)
{
	Vector<T, D> tmp = v1;
	return (tmp /= v2);
}

template<typename T, Dimension D>
Vector<T, D> operator /(const Vector<T, D>& v1, T f)
{
	Vector<T, D> tmp = v1;
	return (tmp /= f);
}

template<typename T, Dimension D>
Vector<T, D> operator /(T f, const Vector<T, D>& v1)
{
	Vector<T, D>tmp = v1;
	tmp.inv();
	return (tmp *= f);
}

// Comparison
template<typename T, Dimension D>
bool operator ==(const Vector<T, D>& v1, const Vector<T, D>& v2)
{
	for (Index i = 0; i < D; ++i)
	{
		if (v1(i) != v2(i))
			return false;
	}

	return true;
}

template<typename T, Dimension D>
bool operator !=(const Vector<T, D>& v1, const Vector<T, D>& v2)
{
	return !(v1 == v2);
}

NS_END_NAMESPACE