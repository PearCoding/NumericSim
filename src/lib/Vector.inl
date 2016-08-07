#ifndef _NS_VECTOR_INL
# error Vector.inl should only be included by Vector.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension D>
Vector<T, D>::Vector() :
	CountableSet()
{
}

template<typename T, Dimension D>
Vector<T, D>::Vector(const T& f) :
	CountableSet(f)
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
		set(i, v);
		i++;
	}
}

template<typename T, Dimension D>
Vector<T, D>::~Vector()
{
}

// Other
template<typename T, Dimension D>
constexpr Dimension Vector<T, D>::dim() const
{
	return D;
}

template<typename T, Dimension D>
T Vector<T, D>::dot(const Vector& v) const
{
	T s = 0;
	for (Index i = 0; i < D; ++i)
		s += at(i) * v.at(i);

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

// Boolean set operations
template<typename T, Dimension D>
template<Index I>
Vector<T, I + 1> Vector<T, D>::left() const
{
	static_assert(I < D, "Given left index exceeds dimension of vector.");
	Vector<T, I + 1> tmp;

	for (Index i = 0; i <= I; ++i)
		tmp.set(i, at(i));

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
		tmp.set(j - start, at(j));

	return tmp;
}

template<typename T, Dimension D>
template<Index I>
Vector<T, D - I> Vector<T, D>::right() const
{
	static_assert(I < D, "Given right index exceeds dimension of vector.");
	Vector<T, D - I> tmp;

	for (Index i = I; i < D; ++i)
		tmp.set(i - I, at(i));

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
		tmp.set(j - i, at(j));

	return tmp;
}

template<typename T, Dimension D>
template<Index Start, Index End>
Vector<T, End - Start> Vector<T, D>::mid() const
{
	static_assert(Start < End, "Start index has to be lower then End index.");

	Vector<T, End - Start> tmp;
	for (Index i = Start; i < End && i < D; ++i)
		tmp.set(i - Start, at(i));

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
		tmp.set(i - start, at(i));

	return tmp;
}

// Elementwise operations
template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator +=(const Vector<T, D>& v2)
{
	for (Index i = 0; i < D; ++i)
		mData[i] += v2.at(i);

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator +=(const T& f)
{
	for (Index i = 0; i < D; ++i)
		mData[i] += f;

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator -=(const Vector<T, D>& v2)
{
	for (Index i = 0; i < D; ++i)
		mData[i] -= v2.at(i);

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator -=(const T& f)
{
	for (Index i = 0; i < D; ++i)
		mData[i] -= f;

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator *=(const Vector<T, D>& v2)
{
	for (Index i = 0; i < D; ++i)
		mData[i] *= v2.at(i);

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator *=(const T& f)
{
	for (Index i = 0; i < D; ++i)
		mData[i] *= f;

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator /=(const Vector<T, D>& v2)
{
	for (Index i = 0; i < D; ++i)
		mData[i] /= v2.at(i);

	return *this;
}

template<typename T, Dimension D>
Vector<T, D>& Vector<T, D>::operator /=(const T& f)
{
	T invF = (T)1 / f;// TODO: NAN?
	for (Index i = 0; i < D; ++i)
		mData[i] *= invF;

	return *this;
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
	tmp.inverse();
	return (tmp *= f);
}

// Comparison
template<typename T, Dimension D>
bool operator ==(const Vector<T, D>& v1, const Vector<T, D>& v2)
{
	for (Index i = 0; i < D; ++i)
	{
		if (v1.at(i) != v2.at(i))
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