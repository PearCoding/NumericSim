#ifndef _NS_DENSEMATRIX_INL
# error DenseMatrix.inl should only be included by DenseMatrix.h
#endif

NS_BEGIN_NAMESPACE

template<typename T>
DenseMatrix<T>::DenseMatrix() :
	BaseMatrix<T,dynamic_container2d_t<T> >()
{
}

template<typename T>
DenseMatrix<T>::DenseMatrix(Dimension d1, Dimension d2) :
	DenseMatrix()
{
	NS_ASSERT(d1 > 0);
	NS_ASSERT(d2 > 0);

	this->mValues.Container.resize(d1*d2);
	this->mValues.RowCount = d1;
	this->mValues.ColumnCount = d2;
}

template<typename T>
DenseMatrix<T>::DenseMatrix(std::initializer_list<std::initializer_list<T> > l) :
	DenseMatrix(l.size(),
		std::max_element(l.begin(), l.end(), [](const std::initializer_list<T>& a, const std::initializer_list<T>& b) 
		{ return a.size() < b.size(); })->size())
{
	Index i = 0;
	for (const auto& v : l)
	{
		Index j = 0;
		for (const auto& w : v)
		{
			this->mValues.Container[i*this->mValues.ColumnCount + j] = w;
			++j;
		}
		++i;
	}
}

template<typename T>
DenseMatrix<T>::~DenseMatrix()
{
}

template<typename T>
void DenseMatrix<T>::resize(Dimension d1, Dimension d2)
{
	NS_ASSERT(d1 > 0);
	NS_ASSERT(d2 > 0);
	//TODO
}

// Operators
template<typename T>
template<class DC2>
DenseMatrix<T>& DenseMatrix<T>::operator +=(const BaseMatrix<T,DC2>& m)
{
	if (this->size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mValues.Container[i] += m.linear_at(i);

	return *this;
}

template<typename T>
template<class DC2>
DenseMatrix<T>& DenseMatrix<T>::operator -=(const BaseMatrix<T,DC2>& m)
{
	if (this->size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mValues.Container[i] -= m.linear_at(i);

	return *this;
}

template<typename T>
template<class DC2>
DenseMatrix<T>& DenseMatrix<T>::operator *=(const BaseMatrix<T,DC2>& m)
{
	if (this->size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < this->size(); ++i)
		this->mValues.Container[i] *= m.linear_at(i);

	return *this;
}

template<typename T>
DenseMatrix<T>& DenseMatrix<T>::operator *=(const T& f)
{
	for (Index i = 0; i < this->size(); ++i)
		this->mValues.Container[i] *= f;

	return *this;
}

template<typename T>
void DenseMatrix<T>::swap(DenseMatrix<T>& v)
{
	std::swap(this->mValues.Container, v.mValues.Container);
	std::swap(this->mValues.RowCount, v.mValues.RowCount);
	std::swap(this->mValues.ColumnCount, v.mValues.ColumnCount);
}

template<typename T>
DenseMatrix<T> DenseMatrix<T>::transpose() const
{
	DenseMatrix<T> tmp(this->columns(), this->rows());
	for (Index i = 0; i < this->columns(); ++i)
	{
		for (Index j = 0; j < this->rows(); ++j)
		{
			tmp.set(i, j, this->at(j, i));
		}
	}
	return tmp;
}

template<typename T>
DenseMatrix<T> DenseMatrix<T>::conjugate() const
{
	DenseMatrix<T> tmp(this->rows(), this->columns());

	for (Index i = 0; i < this->rows(); ++i)
	{
		for (Index j = 0; j < this->columns(); ++j)
		{
			tmp.set(i, j, conjugate_vt(this->at(i, j)));
		}
	}

	return tmp;
}

template<typename T>
DenseMatrix<T> DenseMatrix<T>::adjugate() const
{
	DenseMatrix<T> tmp(this->columns(), this->rows());
	for (Index i = 0; i < this->columns(); ++i)
	{
		for (Index j = 0; j < this->rows(); ++j)
		{
			tmp.set(i, j, conjugate_vt(this->at(j, i)));
		}
	}

	return tmp;
}

template<typename T>
DenseMatrix<T> DenseMatrix<T>::mul(const DenseMatrix<T>& m) const
{
	if (this->columns() != m.rows())
		throw MatrixMulMismatchException();

	DenseMatrix<T> tmp(this->rows(), m.columns());
	for (Index i = 0; i < this->rows(); ++i)
	{
		for (Index k = 0; k < m.columns(); ++k)
		{
			T v = 0;
			for (Index j = 0; j < this->columns(); ++j)
			{
				v += this->at(i, j) * m.at(j, k);
			}
			tmp.set(i, k, v);
		}
	}
	return tmp;
}

template<typename T>
template<typename DC>
DynamicVector<T> DenseMatrix<T>::mul(const Vector<T,DC>& v) const
{
	if(this->columns() != v.size())
		throw MatrixMulMismatchException();

	DynamicVector<T> r;
	r.resize(this->rows());

	for (Index i = 0; i < this->rows(); ++i)
	{
		T t = (T)0;
		for (Index j = 0; j < this->columns(); ++j)
			t += this->at(i, j)*v.linear_at(j);
		r.linear_set(i, t);
	}
	return r;
}

template<typename T>
template<typename DC>
DynamicVector<T> DenseMatrix<T>::mul_left(const Vector<T,DC>& v) const
{
	if (this->rows() != v.size())
		throw MatrixMulMismatchException();

	DynamicVector<T> r;
	r.resize(this->columns());
	
	for (Index j = 0; j < this->columns(); ++j)
	{
		T t = (T)0;
		for (Index i = 0; i < this->rows(); ++i)
			t += this->at(i, j)*v.linear_at(i);
		r.linear_set(j, t);
	}
	return r;
}

template<typename T>
DenseMatrix<T> operator +(const DenseMatrix<T>& v1, const DenseMatrix<T>& v2)
{
	DenseMatrix<T>  tmp = v1;
	return (tmp += v2);
}

template<typename T>
DenseMatrix<T> operator -(const DenseMatrix<T>& v1, const DenseMatrix<T>& v2)
{
	DenseMatrix<T>  tmp = v1;
	return (tmp -= v2);
}

template<typename T>
DenseMatrix<T> operator -(const DenseMatrix<T>& v)
{
	DenseMatrix<T>  tmp = v;
	return (tmp *= (T)-1);
}

template<typename T>
DenseMatrix<T> operator *(const DenseMatrix<T>& v1, const DenseMatrix<T>& v2)
{
	DenseMatrix<T> tmp = v1;
	return (tmp *= v2);
}

template<typename T>
DenseMatrix<T> operator *(const DenseMatrix<T>& v1, T f)
{
	DenseMatrix<T> tmp = v1;
	return (tmp *= f);
}

template<typename T>
DenseMatrix<T> operator *(T f, const DenseMatrix<T>& v1)
{
	return v1 * f;
}

NS_END_NAMESPACE