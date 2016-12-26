#ifndef _NS_VECTOR_INL
# error Vector.inl should only be included by Vector.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, class DC>
Vector<T,DC>::Vector() :
	CountableSet<T,DC>()
{
}

template<typename T, class DC>
template<class TMP>
Vector<T, DC>::Vector(size_t size, typename std::enable_if<std::is_same<TMP, dynamic_container_t<T>>::value>::type*) :
	CountableSet<T,DC>(size)
{
}

template<typename T, class DC>
template<class TMP>
Vector<T,DC>::Vector(std::initializer_list<T> l, typename std::enable_if<std::is_same<TMP, dynamic_container_t<T>>::value>::type*) : 
	Vector(l.size())
{
	Index i = 0;
	for (const T& v : l)
	{
		set(i, v);
		i++;
	}
}

template<typename T, class DC>
template<class TMP>
Vector<T,DC>::Vector(std::initializer_list<T> l, typename std::enable_if<!std::is_same<TMP, dynamic_container_t<T>>::value>::type*) : 
	Vector()
{
	Index i = 0;
	for (const T& v : l)
	{
		set(i, v);
		i++;
	}
}

template<typename T, class DC>
Vector<T,DC>::~Vector()
{
}

template<typename T, class DC>
const T& Vector<T,DC>::at(Index i) const 
{
	return this->linear_at(i);
}

template<typename T, class DC>
void Vector<T,DC>::set(Index i, const T& v) 
{ 
	this->linear_set(i, v);
}

// Other
template<typename T, class DC>
T Vector<T,DC>::dot(const Vector& v) const
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();
	T s = 0;
	for (Index i = 0; i < this->size(); ++i)
		s += at(i) * v.at(i);

	return s;
}

template<typename T, class DC>
T Vector<T,DC>::mag() const
{
	return std::sqrt(magSqr());
}

template<typename T, class DC>
T Vector<T,DC>::magSqr() const
{
	return dot(*this);
}

// Boolean set operations
// NOT WORKING
template<typename T, class DC>
Vector<T,DC> Vector<T,DC>::left(Index i) const
{
	NS_ASSERT(i < this->size());
	Vector<T,DC> tmp;//Size i+1
	for (Index j = 0; j < i + 1; ++j)
		tmp.set(j, at(j));

	return tmp;
}

// NOT WORKING
template<typename T, class DC>
Vector<T,DC> Vector<T,DC>::right(Index i) const
{
	NS_ASSERT(i < this->size());
	
	Vector<T,DC> tmp;//(this->size() - i)

	for (Index j = i; j < this->size(); ++j)
		tmp.set(j - i, at(j));

	return tmp;
}

// NOT WORKING
template<typename T, class DC>
Vector<T,DC> Vector<T,DC>::mid(Index start, Index end) const
{
	NS_ASSERT(start < end);

	Vector<T,DC> tmp;//(end-start)
	for (Index i = start; i < end && i < this->size(); ++i)
		tmp.set(i - start, at(i));

	return tmp;
}

// Elementwise operations
template<typename T, class DC>
Vector<T,DC>& Vector<T,DC>::operator +=(const Vector<T,DC>& v)
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] += v.at(i);

	return *this;
}

template<typename T, class DC>
Vector<T,DC>& Vector<T,DC>::operator +=(const T& f)
{
	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] += f;

	return *this;
}

template<typename T, class DC>
Vector<T,DC>& Vector<T,DC>::operator -=(const Vector<T,DC>& v)
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] -= v.at(i);

	return *this;
}

template<typename T, class DC>
Vector<T,DC>& Vector<T,DC>::operator -=(const T& f)
{
	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] -= f;

	return *this;
}

template<typename T, class DC>
Vector<T,DC>& Vector<T,DC>::operator *=(const Vector<T,DC>& v)
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] *= v.at(i);

	return *this;
}

template<typename T, class DC>
Vector<T,DC>& Vector<T,DC>::operator *=(const T& f)
{
	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] *= f;

	return *this;
}

template<typename T, class DC>
Vector<T,DC>& Vector<T,DC>::operator /=(const Vector<T,DC>& v)
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] /= v.at(i);

	return *this;
}

template<typename T, class DC>
Vector<T,DC>& Vector<T,DC>::operator /=(const T& f)
{
	T invF = (T)1 / f;// TODO: NAN?
	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] *= invF;

	return *this;
}

// Non member functions
// Element wise operations
template<typename T, class DC>
Vector<T,DC> operator +(const Vector<T,DC>& v1, const Vector<T,DC>& v2)
{
	Vector<T,DC> tmp = v1;
	return (tmp += v2);
}

template<typename T, class DC>
Vector<T,DC> operator +(const Vector<T,DC>& v1, T f)
{
	Vector<T,DC> tmp = v1;
	return (tmp += f);
}

template<typename T, class DC>
Vector<T,DC> operator +(T f, const Vector<T,DC>& v1)
{
	return v1 + f;
}

template<typename T, class DC>
Vector<T,DC> operator -(const Vector<T,DC>& v1, const Vector<T,DC>& v2)
{
	Vector<T,DC> tmp = v1;
	return (tmp -= v2);
}

template<typename T, class DC>
Vector<T,DC> operator -(const Vector<T,DC>& v1, T f)
{
	Vector<T,DC> tmp = v1;
	return (tmp -= f);
}

template<typename T, class DC>
Vector<T,DC> operator -(T f, const Vector<T,DC>& v1)
{
	Vector<T,DC> tmp = -v1;
	return (tmp += f);
}

template<typename T, class DC>
Vector<T,DC> operator -(const Vector<T,DC>& v)
{
	Vector<T,DC> tmp = v;
	return (tmp *= -1);
}

template<typename T, class DC>
Vector<T,DC> operator *(const Vector<T,DC>& v1, const Vector<T,DC>& v2)
{
	Vector<T,DC> tmp = v1;
	return (tmp *= v2);
}

template<typename T, class DC>
Vector<T,DC> operator *(const Vector<T,DC>& v1, T f)
{
	Vector<T,DC> tmp = v1;
	return (tmp *= f);
}

template<typename T, class DC>
Vector<T,DC> operator *(T f, const Vector<T,DC>& v1)
{
	return v1 * f;
}

template<typename T, class DC>
Vector<T,DC> operator /(const Vector<T,DC>& v1, const Vector<T,DC>& v2)
{
	Vector<T,DC> tmp = v1;
	return (tmp /= v2);
}

template<typename T, class DC>
Vector<T,DC> operator /(const Vector<T,DC>& v1, T f)
{
	Vector<T,DC> tmp = v1;
	return (tmp /= f);
}

template<typename T, class DC>
Vector<T,DC> operator /(T f, const Vector<T,DC>& v1)
{
	Vector<T,DC>tmp = v1;
	tmp.do_reciprocal();
	return (tmp *= f);
}

// Comparison
template<typename T, class DC>
bool operator ==(const Vector<T,DC>& v1, const Vector<T,DC>& v2)
{
	if (v1.size() != v2.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < v1.size(); ++i)
	{
		if (v1.at(i) != v2.at(i))
			return false;
	}

	return true;
}

template<typename T, class DC>
bool operator !=(const Vector<T,DC>& v1, const Vector<T,DC>& v2)
{
	return !(v1 == v2);
}

NS_END_NAMESPACE