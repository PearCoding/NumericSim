#ifndef _NS_VECTOR_INL
# error Vector.inl should only be included by Vector.h
#endif

NS_BEGIN_NAMESPACE

template<typename T>
Vector<T>::Vector() :
	CountableSet<T>()
{
}

template<typename T>
Vector<T>::Vector(Dimension size) :
	CountableSet<T>(size)
{
}

template<typename T>
Vector<T>::Vector(Dimension size, const T& f) :
	CountableSet<T>(size, f)
{
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> l) : 
	Vector(l.size())
{
	Index i = 0;
	for (const T& v : l)
	{
		set(i, v);
		i++;
	}
}

template<typename T>
Vector<T>::~Vector()
{
}

template<typename T>
const T& Vector<T>::at(Index i) const 
{
	return this->linear_at(i);
}

template<typename T>
void Vector<T>::set(Index i, const T& v) 
{ 
	this->linear_set(i, v);
}

// Other
template<typename T>
T Vector<T>::dot(const Vector& v) const
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();
	T s = 0;
	for (Index i = 0; i < this->size(); ++i)
		s += at(i) * v.at(i);

	return s;
}

template<typename T>
T Vector<T>::mag() const
{
	return std::sqrt(magSqr());
}

template<typename T>
T Vector<T>::magSqr() const
{
	return dot(*this);
}

// Boolean set operations
template<typename T>
Vector<T> Vector<T>::left(Index i) const
{
	NS_ASSERT(i < this->size());
	Vector<T> tmp(i+1);
	for (Index j = 0; j < i + 1; ++j)
		tmp.set(j, at(j));

	return tmp;
}

template<typename T>
Vector<T> Vector<T>::right(Index i) const
{
	NS_ASSERT(i < this->size());
	
	Vector<T> tmp(this->size() - i);

	for (Index j = i; j < this->size(); ++j)
		tmp.set(j - i, at(j));

	return tmp;
}

template<typename T>
Vector<T> Vector<T>::mid(Index start, Index end) const
{
	NS_ASSERT(start < end);

	Vector<T> tmp(end-start);
	for (Index i = start; i < end && i < size(); ++i)
		tmp.set(i - start, at(i));

	return tmp;
}

// Elementwise operations
template<typename T>
Vector<T>& Vector<T>::operator +=(const Vector<T>& v)
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] += v.at(i);

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator +=(const T& f)
{
	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] += f;

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator -=(const Vector<T>& v)
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] -= v.at(i);

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator -=(const T& f)
{
	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] -= f;

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator *=(const Vector<T>& v)
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] *= v.at(i);

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator *=(const T& f)
{
	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] *= f;

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator /=(const Vector<T>& v)
{
	if (this->size() != v.size())
		throw VectorSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] /= v.at(i);

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator /=(const T& f)
{
	T invF = (T)1 / f;// TODO: NAN?
	for (Index i = 0; i < this->size(); ++i)
		this->mData[i] *= invF;

	return *this;
}

// Non member functions
// Element wise operations
template<typename T>
Vector<T> operator +(const Vector<T>& v1, const Vector<T>& v2)
{
	Vector<T> tmp = v1;
	return (tmp += v2);
}

template<typename T>
Vector<T> operator +(const Vector<T>& v1, T f)
{
	Vector<T> tmp = v1;
	return (tmp += f);
}

template<typename T>
Vector<T> operator +(T f, const Vector<T>& v1)
{
	return v1 + f;
}

template<typename T>
Vector<T> operator -(const Vector<T>& v1, const Vector<T>& v2)
{
	Vector<T> tmp = v1;
	return (tmp -= v2);
}

template<typename T>
Vector<T> operator -(const Vector<T>& v1, T f)
{
	Vector<T> tmp = v1;
	return (tmp -= f);
}

template<typename T>
Vector<T> operator -(T f, const Vector<T>& v1)
{
	Vector<T> tmp = -v1;
	return (tmp += f);
}

template<typename T>
Vector<T> operator -(const Vector<T>& v)
{
	Vector<T> tmp = v;
	return (tmp *= -1);
}

template<typename T>
Vector<T> operator *(const Vector<T>& v1, const Vector<T>& v2)
{
	Vector<T> tmp = v1;
	return (tmp *= v2);
}

template<typename T>
Vector<T> operator *(const Vector<T>& v1, T f)
{
	Vector<T> tmp = v1;
	return (tmp *= f);
}

template<typename T>
Vector<T> operator *(T f, const Vector<T>& v1)
{
	return v1 * f;
}

template<typename T>
Vector<T> operator /(const Vector<T>& v1, const Vector<T>& v2)
{
	Vector<T> tmp = v1;
	return (tmp /= v2);
}

template<typename T>
Vector<T> operator /(const Vector<T>& v1, T f)
{
	Vector<T> tmp = v1;
	return (tmp /= f);
}

template<typename T>
Vector<T> operator /(T f, const Vector<T>& v1)
{
	Vector<T>tmp = v1;
	tmp.do_reciprocal();
	return (tmp *= f);
}

// Comparison
template<typename T>
bool operator ==(const Vector<T>& v1, const Vector<T>& v2)
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

template<typename T>
bool operator !=(const Vector<T>& v1, const Vector<T>& v2)
{
	return !(v1 == v2);
}

NS_END_NAMESPACE