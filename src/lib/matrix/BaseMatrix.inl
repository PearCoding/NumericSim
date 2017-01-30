#ifndef _NS_BASEMATRIX_INL
# error BaseMatrix.inl should only be included by BaseMatrix.h
#endif

NS_BEGIN_NAMESPACE

// Iterator
template<typename T, class DC>
MatrixIterator<T,DC>::MatrixIterator(const BaseMatrix<T,DC>& m, Index i1, Index i2) :
	mMatrix(&m), mIndex1(i1), mIndex2(i2)
{
}

template<typename T, class DC>
MatrixIterator<T,DC>& MatrixIterator<T,DC>::operator++ ()
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

template<typename T, class DC>
MatrixIterator<T,DC> MatrixIterator<T,DC>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

template<typename T, class DC>
T MatrixIterator<T,DC>::operator *() const
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

template<typename T, class DC>
T& MatrixIterator<T,DC>::operator *()// This is really dirty
{
	if (isValid())
	{
		return const_cast<BaseMatrix<T,DC>*>(mMatrix)->mValues.Container[mIndex1*mMatrix->columns() + mIndex2];
	}
	else
	{
		NS_ASSERT(mMatrix->mEmpty == (T)0);
		return const_cast<BaseMatrix<T,DC>*>(mMatrix)->mEmpty;
	}
}

// Row
template<typename T, class DC>
MatrixRowIterator<T,DC>::MatrixRowIterator(const BaseMatrix<T,DC>& m, Index i1, Index i2) :
	MatrixIterator<T,DC>(m, i1, i2)
{
}

template<typename T, class DC>
MatrixRowIterator<T,DC>& MatrixRowIterator<T,DC>::operator++ ()
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

template<typename T, class DC>
MatrixRowIterator<T,DC> MatrixRowIterator<T,DC>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

// Column
template<typename T, class DC>
MatrixColumnIterator<T,DC>::MatrixColumnIterator(const BaseMatrix<T,DC>& m, Index i1, Index i2) :
	MatrixIterator<T,DC>(m, i1, i2)
{
}

template<typename T, class DC>
MatrixColumnIterator<T,DC>& MatrixColumnIterator<T,DC>::operator++ ()
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

template<typename T, class DC>
MatrixColumnIterator<T,DC> MatrixColumnIterator<T,DC>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

// Main

template<typename T, class DC>
BaseMatrix<T,DC>::BaseMatrix() :
	mValues(), mEmpty((T)0)
{
	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are std::complex and the types allowed by std::is_floating_point.");
}

template<typename T, class DC>
BaseMatrix<T,DC>::~BaseMatrix()
{
}

template<typename T, class DC>
T BaseMatrix<T,DC>::at(Index i1, Index i2) const
{
	NS_ASSERT(i1 < rows());
	NS_ASSERT(i2 < columns());
	return linear_at(i1*columns() + i2);
}

template<typename T, class DC>
void BaseMatrix<T,DC>::set(Index i1, Index i2, const T& v)
{
	NS_ASSERT(i1 < rows());
	NS_ASSERT(i2 < columns());
	linear_set(i1*columns() + i2, v);
}

template<typename T, class DC>
T BaseMatrix<T,DC>::linear_at(Index i) const
{
	NS_ASSERT(i < size());
	return mValues.Container[i];
}

template<typename T, class DC>
void BaseMatrix<T,DC>::linear_set(Index i, const T& v)
{
	NS_ASSERT(i < size());
	mValues.Container[i] = v;
}

template<typename T, class DC>
bool BaseMatrix<T,DC>::has(Index i1, Index i2) const
{
	return at(i1, i2) != (T)0;
}

template<typename T, class DC>
bool BaseMatrix<T,DC>::has(Index i1, Index i2, T& val) const
{
	val = at(i1, i2);
	return val != (T)0;
}

// Iterators
template<typename T, class DC>
typename BaseMatrix<T,DC>::const_iterator BaseMatrix<T,DC>::begin() const
{
	return iterator(*this, 0, 0);
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_iterator BaseMatrix<T,DC>::end() const
{
	return iterator(*this, rows(), columns());
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::iterator BaseMatrix<T,DC>::begin()
{
	return iterator(*this, 0, 0);
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::iterator BaseMatrix<T,DC>::end()
{
	return iterator(*this, rows(), columns());
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_iterator BaseMatrix<T,DC>::cbegin() const
{
	return begin();
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_iterator BaseMatrix<T,DC>::cend() const
{
	return end();
}

// Row
template<typename T, class DC>
typename BaseMatrix<T,DC>::const_row_iterator BaseMatrix<T,DC>::row_begin(Index i) const
{
	NS_ASSERT(i < rows());
	return row_iterator(*this, i, 0);
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_row_iterator BaseMatrix<T,DC>::row_end(Index) const
{
	return row_iterator(*this, rows(), columns());
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::row_iterator BaseMatrix<T,DC>::row_begin(Index i)
{
	NS_ASSERT(i < rows());
	return row_iterator(*this, i, 0);
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::row_iterator BaseMatrix<T,DC>::row_end(Index)
{
	return row_iterator(*this, rows(), columns());
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_row_iterator BaseMatrix<T,DC>::row_cbegin(Index i) const
{
	return row_begin(i);
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_row_iterator BaseMatrix<T,DC>::row_cend(Index i) const
{
	return row_end(i);
}

//Column
template<typename T, class DC>
typename BaseMatrix<T,DC>::const_column_iterator BaseMatrix<T,DC>::column_begin(Index i) const
{
	NS_ASSERT(i < columns());
	return column_iterator(*this, 0, i);
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_column_iterator BaseMatrix<T,DC>::column_end(Index) const
{
	return column_iterator(*this, rows(), columns());
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::column_iterator BaseMatrix<T,DC>::column_begin(Index i)
{
	NS_ASSERT(i < columns());
	return column_iterator(*this, 0, i);
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::column_iterator BaseMatrix<T,DC>::column_end(Index)
{
	return column_iterator(*this, rows(), columns());
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_column_iterator BaseMatrix<T,DC>::column_cbegin(Index i) const
{
	return column_begin(i);
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::const_column_iterator BaseMatrix<T,DC>::column_cend(Index i) const
{
	return column_end(i);
}

// Set/Erase
template<typename T, class DC>
typename BaseMatrix<T,DC>::iterator BaseMatrix<T,DC>::set(const iterator& it, const T& val)
{
	NS_ASSERT(this == it.mMatrix);

	if (it.row() < rows() && it.column() < columns())
	{
		set(it.row(), it.column(), val);
	}
	
	return it;
}

template<typename T, class DC>
typename BaseMatrix<T,DC>::iterator BaseMatrix<T,DC>::erase(const iterator& it)
{
	NS_ASSERT(this == it.mMatrix);

	if (it.row() < rows() && it.column() < columns())
	{
		set(it.row(), it.column(), (T)0);
	}
	
	return it;
}

template<typename T, class DC>
Dimension BaseMatrix<T,DC>::columns() const
{
	return mValues.ColumnCount;
}

template<typename T, class DC>
Dimension BaseMatrix<T,DC>::rows() const
{
	return mValues.RowCount;
}

template<typename T, class DC>
Dimension BaseMatrix<T,DC>::size() const
{
	return rows() * columns();
}

template<typename T, class DC>
bool BaseMatrix<T,DC>::empty() const
{
	for (auto l : mValues.Container)
	{
		if (l != (T)0)
			return false;
	}
	
	return true;
}

template<typename T, class DC>
T BaseMatrix<T,DC>::sum() const
{
	T s = 0;
	for (auto l : mValues.Container)
		s += l;

	return s;
}

template<typename T, class DC>
T BaseMatrix<T,DC>::max() const
{
	T s = std::numeric_limits<typename get_complex_internal<T>::type>::min();
	for (auto l : mValues.Container)
	{
		if (l > s)
			s = l;
	}

	return s;
}

template<typename T, class DC>
T BaseMatrix<T,DC>::min() const
{
	T s = std::numeric_limits<typename get_complex_internal<T>::type>::max();
	for (auto l : mValues.Container)
	{
		if (l < s)
			s = l;
	}

	return s;
}

template<typename T, class DC>
T BaseMatrix<T,DC>::avg() const
{
	return sum() / size();
}

template<typename T, class DC>
bool BaseMatrix<T,DC>::has_nan() const
{
	for (auto l : mValues.Container)
	{
		if (std::isnan(l))
			return true;
	}

	return false;
}

template<typename T, class DC>
bool BaseMatrix<T,DC>::has_inf() const
{
	for (auto l : mValues.Container)
	{
		if (std::isinf(l))
			return true;
	}

	return false;
}

template<typename T, class DC>
bool BaseMatrix<T,DC>::has_zero() const
{
	for (auto l : mValues.Container)
	{
		if (l == (T)0)
			return true;
	}

	return false;
}

template<typename T, class DC>
T BaseMatrix<T,DC>::trace() const
{
	T v = (T)0;
	for (Index i = 0; i < std::min(rows(), columns()); ++i)
		v += at(i, i);

	return v;
}

template<typename T, class DC>
bool operator ==(const BaseMatrix<T,DC>& v1, const BaseMatrix<T,DC>& v2)
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

template<typename T, class DC>
bool operator !=(const BaseMatrix<T,DC>& v1, const BaseMatrix<T,DC>& v2)
{
	return !(v1 == v2);
}

NS_END_NAMESPACE