#ifndef _NS_MATRIX_INL
# error Matrix.inl should only be included by Matrix.h
#endif

NS_BEGIN_NAMESPACE

// Iterator
template<typename T>
MatrixIterator<T>::MatrixIterator(const Matrix<T>& m, Index i1, Index i2) :
	mMatrix(&m), mIndex1(i1), mIndex2(i2)
{
}

template<typename T>
MatrixIterator<T>& MatrixIterator<T>::operator++ ()
{
	if (isValid())
	{
		mIndex2++;

		if(mIndex2 >= mMatrix->columns())
		{
			mIndex2 = 0;
			mIndex1++;
			if (mIndex1 >= mMatrix->rows())
			{
				mIndex1 = mMatrix->rows();
				mIndex2 = mMatrix->columns();
			}
		}
	}
	return *this;
}

template<typename T>
MatrixIterator<T> MatrixIterator<T>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

template<typename T>
T MatrixIterator<T>::operator *() const
{
	if (isValid())
	{
		return mMatrix->at(mIndex1, mIndex2);
	}
	else
	{
		return (T)0;
	}
}

template<typename T>
T& MatrixIterator<T>::operator *()// This is really dirty
{
	if (isValid())
	{
		return const_cast<Matrix<T>*>(mMatrix)->mValues[mIndex1*mMatrix->columns() + mIndex2];
	}
	else
	{
		NS_ASSERT(mMatrix->mEmpty == (T)0);
		return const_cast<Matrix<T>*>(mMatrix)->mEmpty;
	}
}

// Row
template<typename T>
MatrixRowIterator<T>::MatrixRowIterator(const Matrix<T>& m, Index i1, Index i2) :
	MatrixIterator<T>(m, i1, i2)
{
}

template<typename T>
MatrixRowIterator<T>& MatrixRowIterator<T>::operator++ ()
{
	if (this->isValid())
	{
		this->mIndex2++;

		if (this->mIndex2 >= this->mMatrix->columns())
		{
			this->mIndex1 = this->mMatrix->rows();
			this->mIndex2 = this->mMatrix->columns();
		}
	}
	return *this;
}

template<typename T>
MatrixRowIterator<T> MatrixRowIterator<T>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

// Column
template<typename T>
MatrixColumnIterator<T>::MatrixColumnIterator(const Matrix<T>& m, Index i1, Index i2) :
	MatrixIterator<T>(m, i1, i2)
{
}

template<typename T>
MatrixColumnIterator<T>& MatrixColumnIterator<T>::operator++ ()
{
	if (this->isValid())
	{
		this->mIndex1++;

		if (this->mIndex1 >= this->mMatrix->rows())
		{
			this->mIndex1 = this->mMatrix->rows();
			this->mIndex2 = this->mMatrix->columns();
		}
	}

	return *this;
}

template<typename T>
MatrixColumnIterator<T> MatrixColumnIterator<T>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

// Main
template<typename T>
Matrix<T>::Matrix(Dimension d1, Dimension d2) :
	mValues(d1*d2, (T)0), mEmpty((T)0), mRowCount(d1), mColumnCount(d2)
{
	NS_ASSERT(d1 > 0);
	NS_ASSERT(d2 > 0);

	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are ComplexNumber and the types allowed by std::is_floating_point.");
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T> > l) :
	Matrix(l.size(),
		std::max_element(l.begin(), l.end(), [](const std::initializer_list<T>& a, const std::initializer_list<T>& b) 
		{ return a.size() < b.size(); })->size())
{
	Index i = 0;
	for (const auto& v : l)
	{
		Index j = 0;
		for (const auto& w : v)
		{
			mValues[i*mColumnCount + j] = w;
			++j;
		}
		++i;
	}
}

template<typename T>
Matrix<T>::~Matrix()
{
}

template<typename T>
void Matrix<T>::resize(Dimension d1, Dimension d2)
{
	NS_ASSERT(d1 > 0);
	NS_ASSERT(d2 > 0);
	//TODO
}

template<typename T>
T Matrix<T>::at(Index i1, Index i2) const
{
	NS_ASSERT(i1 < rows());
	NS_ASSERT(i2 < columns());
	return linear_at(i1*columns() + i2);
}

template<typename T>
void Matrix<T>::set(Index i1, Index i2, const T& v)
{
	NS_ASSERT(i1 < rows());
	NS_ASSERT(i2 < columns());
	linear_set(i1*columns() + i2, v);
}

template<typename T>
T Matrix<T>::linear_at(Index i) const
{
	NS_ASSERT(i < size());
	return mValues[i];
}

template<typename T>
void Matrix<T>::linear_set(Index i, const T& v)
{
	NS_ASSERT(i < size());
	mValues[i] = v;
}

template<typename T>
bool Matrix<T>::has(Index i1, Index i2) const
{
	return at(i1, i2) != (T)0;
}

template<typename T>
bool Matrix<T>::has(Index i1, Index i2, T& val) const
{
	val = at(i1, i2);
	return val != (T)0;
}

// Iterators
template<typename T>
const MatrixIterator<T> Matrix<T>::begin() const
{
	return iterator(*this, 0, 0);
}

template<typename T>
const MatrixIterator<T> Matrix<T>::end() const
{
	return iterator(*this, rows(), columns());
}

template<typename T>
MatrixIterator<T> Matrix<T>::begin()
{
	return iterator(*this, 0, 0);
}

template<typename T>
MatrixIterator<T> Matrix<T>::end()
{
	return iterator(*this, rows(), columns());
}

template<typename T>
const MatrixIterator<T> Matrix<T>::cbegin() const
{
	return begin();
}

template<typename T>
const MatrixIterator<T> Matrix<T>::cend() const
{
	return end();
}

// Row
template<typename T>
const MatrixRowIterator<T> Matrix<T>::row_begin(Index i) const
{
	NS_ASSERT(i < rows());
	return row_iterator(*this, i, 0);
}

template<typename T>
const MatrixRowIterator<T> Matrix<T>::row_end(Index) const
{
	return row_iterator(*this, rows(), columns());
}

template<typename T>
MatrixRowIterator<T> Matrix<T>::row_begin(Index i)
{
	NS_ASSERT(i < rows());
	return row_iterator(*this, i, 0);
}

template<typename T>
MatrixRowIterator<T> Matrix<T>::row_end(Index)
{
	return row_iterator(*this, rows(), columns());
}

template<typename T>
const MatrixRowIterator<T> Matrix<T>::row_cbegin(Index i) const
{
	return row_begin(i);
}

template<typename T>
const MatrixRowIterator<T> Matrix<T>::row_cend(Index i) const
{
	return row_end(i);
}

//Column
template<typename T>
const MatrixColumnIterator<T> Matrix<T>::column_begin(Index i) const
{
	NS_ASSERT(i < columns());
	return column_iterator(*this, 0, i);
}

template<typename T>
const MatrixColumnIterator<T> Matrix<T>::column_end(Index) const
{
	return column_iterator(*this, rows(), columns());
}

template<typename T>
MatrixColumnIterator<T> Matrix<T>::column_begin(Index i)
{
	NS_ASSERT(i < columns());
	return column_iterator(*this, 0, i);
}

template<typename T>
MatrixColumnIterator<T> Matrix<T>::column_end(Index)
{
	return column_iterator(*this, rows(), columns());
}

template<typename T>
const MatrixColumnIterator<T> Matrix<T>::column_cbegin(Index i) const
{
	return column_begin(i);
}

template<typename T>
const MatrixColumnIterator<T> Matrix<T>::column_cend(Index i) const
{
	return column_end(i);
}

// Set/Erase
template<typename T>
MatrixIterator<T> Matrix<T>::set(const MatrixIterator<T>& it, const T& val)
{
	NS_ASSERT(this == it.mMatrix);

	if (it.row() < rows() && it.column() < columns())
	{
		set(it.row(), it.column(), val);
	}
	
	return it;
}

template<typename T>
MatrixIterator<T> Matrix<T>::erase(const MatrixIterator<T>& it)
{
	NS_ASSERT(this == it.mMatrix);

	if (it.row() < rows() && it.column() < columns())
	{
		set(it.row(), it.column(), (T)0);
	}
	
	return it;
}

// Operators
template<typename T>
Matrix<T>& Matrix<T>::operator +=(const Matrix<T>& m)
{
	if (size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < size(); ++i)
		mValues[i] += m.mValues[i];

	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator -=(const Matrix<T>& m)
{
	if (size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < size(); ++i)
		mValues[i] -= m.mValues[i];

	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator *=(const Matrix<T>& m)
{
	if (size() != m.size())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < size(); ++i)
		mValues[i] *= m.mValues[i];

	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator *=(const T& f)
{
	for (Index i = 0; i < size(); ++i)
		mValues[i] *= f;

	return *this;
}

template<typename T>
Dimension Matrix<T>::columns() const
{
	return mColumnCount;
}

template<typename T>
Dimension Matrix<T>::rows() const
{
	return mRowCount;
}

template<typename T>
Dimension Matrix<T>::size() const
{
	return rows() * columns();
}

template<typename T>
bool Matrix<T>::isEmpty() const
{
	for (auto l : mValues)
	{
		if (l != (T)0)
			return false;
	}
	
	return true;
}

template<typename T>
T Matrix<T>::sum() const
{
	T s = 0;
	for (auto l : mValues)
		s += l;

	return s;
}

template<typename T>
T Matrix<T>::max() const
{
	T s = std::numeric_limits<typename get_complex_internal<T>::type>::min();
	for (auto l : mValues)
	{
		if (l > s)
			s = l;
	}

	return s;
}

template<typename T>
T Matrix<T>::min() const
{
	T s = std::numeric_limits<typename get_complex_internal<T>::type>::max();
	for (auto l : mValues)
	{
		if (l < s)
			s = l;
	}

	return s;
}

template<typename T>
T Matrix<T>::avg() const
{
	return sum() / size();
}

template<typename T>
bool Matrix<T>::hasNaN() const
{
	for (auto l : mValues)
	{
		if (std::isnan(l))
			return true;
	}

	return false;
}

template<typename T>
bool Matrix<T>::hasInf() const
{
	for (auto l : mValues)
	{
		if (std::isinf(l))
			return true;
	}

	return false;
}

template<typename T>
bool Matrix<T>::hasZero() const
{
	for (auto l : mValues)
	{
		if (l == (T)0)
			return true;
	}

	return false;
}

template<typename T>
void Matrix<T>::swap(Matrix<T>& v)
{
	std::swap(mValues, v.mValues);
}

template<typename T>
Matrix<T> Matrix<T>::transpose() const
{
	Matrix<T> tmp(columns(), rows());
	for (Index i = 0; i < columns(); ++i)
	{
		for (Index j = 0; j < rows(); ++j)
		{
			tmp.set(i, j, at(j, i));
		}
	}
	return tmp;
}

template<typename T>
Matrix<T> Matrix<T>::conjugate() const
{
	Matrix<T> tmp(rows(), columns());

	for (Index i = 0; i < rows(); ++i)
	{
		for (Index j = 0; j < columns(); ++j)
		{
			tmp.set(i, j, conjugate_vt(at(i, j)));
		}
	}

	return tmp;
}

template<typename T>
Matrix<T> Matrix<T>::adjugate() const
{
	Matrix<T> tmp(columns(), rows());
	for (Index i = 0; i < columns(); ++i)
	{
		for (Index j = 0; j < rows(); ++j)
		{
			tmp.set(i, j, conjugate_vt(at(j, i)));
		}
	}

	return tmp;
}

template<typename T>
T Matrix<T>::trace() const
{
	T v = (T)0;
	for (Index i = 0; i < std::min(rows(), columns()); ++i)
		v += at(i, i);

	return v;
}

template<typename T>
Matrix<T> Matrix<T>::mul(const Matrix<T>& m) const
{
	if (columns() != m.rows())
		throw MatrixMulMismatchException();

	Matrix<T> tmp(rows(), m.columns());
	for (Index i = 0; i < rows(); ++i)
	{
		for (Index k = 0; k < m.columns(); ++k)
		{
			T v = 0;
			for (Index j = 0; j < columns(); ++j)
			{
				v += at(i, j) * m.at(j, k);
			}
			tmp.set(i, k, v);
		}
	}
	return tmp;
}

template<typename T>
Vector<T> Matrix<T>::mul(const Vector<T>& v) const
{
	if(columns() != v.size())
		throw MatrixMulMismatchException();

	Vector<T> r(rows());
	for (Index i = 0; i < rows(); ++i)
	{
		T t = (T)0;
		for (Index j = 0; j < columns(); ++j)
		{
			t += at(i, j)*v.linear_at(j);
		}
		r.linear_set(i, t);
	}
	return r;
}

template<typename T>
Vector<T> Matrix<T>::mul_left(const Vector<T>& v) const
{
	if (rows() != v.size())
		throw MatrixMulMismatchException();

	Vector<T> r(columns());
	for (Index j = 0; j < columns(); ++j)
	{
		T t = (T)0;
		for (Index i = 0; i < rows(); ++i)
		{
			t += at(i, j)*v.linear_at(i);
		}
		r.linear_set(j, t);
	}
	return r;
}

template<typename T>
Matrix<T> operator +(const Matrix<T>& v1, const Matrix<T>& v2)
{
	Matrix<T>  tmp = v1;
	return (tmp += v2);
}

template<typename T>
Matrix<T> operator -(const Matrix<T>& v1, const Matrix<T>& v2)
{
	Matrix<T>  tmp = v1;
	return (tmp -= v2);
}

template<typename T>
Matrix<T> operator -(const Matrix<T>& v)
{
	Matrix<T>  tmp = v;
	return (tmp *= (T)-1);
}

template<typename T>
Matrix<T> operator *(const Matrix<T>& v1, const Matrix<T>& v2)
{
	Matrix<T> tmp = v1;
	return (tmp *= v2);
}

template<typename T>
Matrix<T> operator *(const Matrix<T>& v1, T f)
{
	Matrix<T> tmp = v1;
	return (tmp *= f);
}

template<typename T>
Matrix<T> operator *(T f, const Matrix<T>& v1)
{
	return v1 * f;
}

template<typename T>
bool operator ==(const Matrix<T>& v1, const Matrix<T>& v2)
{
	if (v1.rows() != v2.rows() || v1.columns() != v2.columns())
		throw MatrixSizeMismatchException();

	for (Index i = 0; i < v1.size(); ++i)
	{
		if (v1.linear_at(i) != v2.linear_at(i))
			return false;
	}

	return true;
}

template<typename T>
bool operator !=(const Matrix<T>& v1, const Matrix<T>& v2)
{
	return !(v1 == v2);
}

NS_END_NAMESPACE