#ifndef _NS_MATRIX_INL
# error Matrix.inl should only be included by Matrix.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2>::Matrix() :
	CountableSet()
{
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2>::Matrix(const T& f) :
	CountableSet(f)
{
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2>::Matrix(std::initializer_list<std::initializer_list<T> > l) :
	Matrix()
{
	NS_ASSERT(l.size() <= D2);// Would be great if we could use size() with constexpr [C++14]
	Index i = 0;
	for (const auto& v : l)
	{
		Index j = 0;
		NS_ASSERT(v.size() <= D1);

		for (const auto& w : v)
		{
			mData->ptr[i*D1 + j] = w;
			++j;
		}
		++i;
	}
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2>::~Matrix()
{
}

template<typename T, Dimension D1, Dimension D2>
const T& Matrix<T, D1, D2>::operator()(Index i1, Index i2) const
{
	NS_ASSERT(i1 < D1);
	NS_ASSERT(i2 < D2);
	return mData->ptr[i2*D1 + i1];
}

template<typename T, Dimension D1, Dimension D2>
T& Matrix<T, D1, D2>::operator()(Index i1, Index i2)
{
	make_unique();

	NS_ASSERT(i1 < D1);
	NS_ASSERT(i2 < D2);
	return mData->ptr[i2*D1 + i1];
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2>& Matrix<T, D1, D2>::operator +=(const Matrix<T, D1,D2>& v2)
{
	make_unique();

	for (Index i = 0; i < D1*D2; ++i)
		mData->ptr[i] += v2.mData->ptr[i];

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2>& Matrix<T, D1, D2>::operator -=(const Matrix<T, D1, D2>& v2)
{
	make_unique();

	for (Index i = 0; i < D1*D2; ++i)
		mData->ptr[i] -= v2.mData->ptr[i];

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2>& Matrix<T, D1, D2>::operator *=(const Matrix<T, D1, D2>& v2)
{
	make_unique();

	for (Index i = 0; i < D1*D2; ++i)
		mData->ptr[i] *= v2.mData->ptr[i];

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2>& Matrix<T, D1, D2>::operator *=(const T& f)
{
	make_unique();

	for (Index i = 0; i < D1*D2; ++i)
		mData->ptr[i] *= f;

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
constexpr Dimension Matrix<T, D1, D2>::columns() const
{
	return D1;
}

template<typename T, Dimension D1, Dimension D2>
constexpr Dimension Matrix<T, D1, D2>::rows() const
{
	return D2;
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D2, D1> Matrix<T, D1, D2>::transpose()
{
	Matrix<T, D2, D1> tmp;
	for (Index i = 0; i < D2; ++i)
	{
		for (Index j = 0; j < D1; ++j)
		{
			tmp(i, j) = (*this)(j, i);
		}
	}
	return tmp;
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> Matrix<T, D1, D2>::invert()
{
	//TODO
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator +(const Matrix<T, D1, D2>& v1, const Matrix<T, D1, D2>& v2)
{
	Matrix<T, D1, D2>  tmp = v1;
	return (tmp += v2);
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator -(const Matrix<T, D1, D2>& v1, const Matrix<T, D1, D2>& v2)
{
	Matrix<T, D1, D2>  tmp = v1;
	return (tmp -= v2);
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator -(const Matrix<T, D1, D2>& v)
{
	Matrix<T, D1, D2>  tmp = v;
	return (tmp *= -1);
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator *(const Matrix<T, D1, D2 >& v1, const Matrix<T, D1, D2>& v2)
{
	Matrix<T, D1, D2> tmp = v1;
	return (tmp *= v2);
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator *(const Matrix<T, D1, D2>& v1, T f)
{
	Matrix<T, D1, D2> tmp = v1;
	return (tmp *= f);
}

template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator *(T f, const Matrix<T, D1, D2>& v1)
{
	return v1 * f;
}

template<typename T, Dimension D1, Dimension D2>
bool operator ==(const Matrix<T, D1, D2>& v1, const Matrix<T, D1, D2>& v2)
{
	for (Index i = 0; i < D1*D2; ++i)
	{
		if (v1.linear_at(i) != v2.linear_at(i))
			return false;
	}

	return true;
}

template<typename T, Dimension D1, Dimension D2>
bool operator !=(const Matrix<T, D1, D2>& v1, const Matrix<T, D1, D2>& v2)
{
	return !(v1 == v2);
}

NS_END_NAMESPACE