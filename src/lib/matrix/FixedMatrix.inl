#ifndef _NS_FIXEDMATRIX_INL
# error FixedMatrix.inl should only be included by FixedMatrix.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2>::FixedMatrix() :
	BaseMatrix<T,fixed_container2d_t<T,K1,K2> >()
{
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2>::FixedMatrix(std::initializer_list<std::initializer_list<T> > l) :
	FixedMatrix()
{
	NS_ASSERT(K1 >= l.size());
	NS_ASSERT(K2 >= std::max_element(l.begin(), l.end(), [](const std::initializer_list<T>& a, const std::initializer_list<T>& b) 
		{ return a.size() < b.size(); })->size());

	Index i = 0;
	for (const auto& v : l)
	{
		Index j = 0;
		for (const auto& w : v)
		{
			this->mValues.Container[i*K2 + j] = w;
			++j;
		}
		++i;
	}
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2>::~FixedMatrix()
{
}

// Operators
template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2>& FixedMatrix<T,K1,K2>::operator +=(const FixedMatrix<T,K1,K2>& m)
{
	if (this->size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mValues.Container[i] += m.mValues.Container[i];

	return *this;
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2>& FixedMatrix<T,K1,K2>::operator -=(const FixedMatrix<T,K1,K2>& m)
{
	if (this->size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mValues.Container[i] -= m.mValues.Container[i];

	return *this;
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2>& FixedMatrix<T,K1,K2>::operator *=(const FixedMatrix<T,K1,K2>& m)
{
	if (this->size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mValues.Container[i] *= m.mValues.Container[i];

	return *this;
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2>& FixedMatrix<T,K1,K2>::operator *=(const T& f)
{
	for (Index i = 0; i < this->size(); ++i)
		this->mValues.Container[i] *= f;

	return *this;
}

template<typename T, Dimension K1, Dimension K2>
void FixedMatrix<T,K1,K2>::swap(FixedMatrix<T,K1,K2>& v)
{
	std::swap(this->mValues.Container, v.mValues.Container);
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K2,K1> FixedMatrix<T,K1,K2>::transpose() const
{
	FixedMatrix<T,K2,K1> tmp;
	for (Index i = 0; i < this->columns(); ++i)
	{
		for (Index j = 0; j < this->rows(); ++j)
		{
			tmp.set(i, j, this->at(j, i));
		}
	}
	return tmp;
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> FixedMatrix<T,K1,K2>::conjugate() const
{
	FixedMatrix<T,K1,K2> tmp;

	for (Index i = 0; i < this->rows(); ++i)
	{
		for (Index j = 0; j < this->columns(); ++j)
		{
			tmp.set(i, j, conjugate_vt(this->at(i, j)));
		}
	}

	return tmp;
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K2,K1> FixedMatrix<T,K1,K2>::adjugate() const
{
	FixedMatrix<T,K2,K1> tmp;
	for (Index i = 0; i < this->columns(); ++i)
	{
		for (Index j = 0; j < this->rows(); ++j)
		{
			tmp.set(i, j, conjugate_vt(this->at(j, i)));
		}
	}

	return tmp;
}

template<typename T, Dimension K1, Dimension K2>
template<Dimension K3>
FixedMatrix<T,K1,K3> FixedMatrix<T,K1,K2>::mul(const FixedMatrix<T,K2,K3>& m) const
{
	if (this->columns() != m.rows())
		throw MatrixMulMismatchException();

	FixedMatrix<T,K1,K3> tmp;
	for (Index i = 0; i < this->rows(); ++i)
	{
		for (Index k = 0; k < m.columns(); ++k)
		{
			T v = 0;
			for (Index j = 0; j < this->columns(); ++j)
				v += this->at(i, j) * m.at(j, k);
			tmp.set(i, k, v);
		}
	}
	return tmp;
}

template<typename T, Dimension K1, Dimension K2>
FixedVector<T, K1> FixedMatrix<T,K1,K2>::mul(const FixedVector<T,K2>& v) const
{
	if(this->columns() != v.size())
		throw MatrixMulMismatchException();

	FixedVector<T, K1> r;
	for (Index i = 0; i < this->rows(); ++i)
	{
		T t = (T)0;
		for (Index j = 0; j < this->columns(); ++j)
			t += this->at(i, j)*v.linear_at(j);
		r.linear_set(i, t);
	}
	return r;
}

template<typename T, Dimension K1, Dimension K2>
FixedVector<T, K2> FixedMatrix<T,K1,K2>::mul_left(const FixedVector<T, K1>& v) const
{
	if (this->rows() != v.size())
		throw MatrixMulMismatchException();

	FixedVector<T, K2> r;
	for (Index j = 0; j < this->columns(); ++j)
	{
		T t = (T)0;
		for (Index i = 0; i < this->rows(); ++i)
			t += this->at(i, j)*v.linear_at(i);
		r.linear_set(j, t);
	}
	return r;
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator +(const FixedMatrix<T,K1,K2>& v1, const FixedMatrix<T,K1,K2>& v2)
{
	FixedMatrix<T,K1,K2> tmp = v1;
	return (tmp += v2);
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator -(const FixedMatrix<T,K1,K2>& v1, const FixedMatrix<T,K1,K2>& v2)
{
	FixedMatrix<T,K1,K2> tmp = v1;
	return (tmp -= v2);
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator -(const FixedMatrix<T,K1,K2>& v)
{
	FixedMatrix<T,K1,K2> tmp = v;
	return (tmp *= (T)-1);
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator *(const FixedMatrix<T,K1,K2>& v1, const FixedMatrix<T,K1,K2>& v2)
{
	FixedMatrix<T,K1,K2> tmp = v1;
	return (tmp *= v2);
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator *(const FixedMatrix<T,K1,K2>& v1, T f)
{
	FixedMatrix<T,K1,K2> tmp = v1;
	return (tmp *= f);
}

template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator *(T f, const FixedMatrix<T,K1,K2>& v1)
{
	return v1 * f;
}

NS_END_NAMESPACE