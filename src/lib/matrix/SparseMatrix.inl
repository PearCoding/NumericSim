#ifndef _NS_SPARSEMATRIX_INL
# error SparseMatrix.inl should only be included by SparseMatrix.h
#endif

NS_BEGIN_NAMESPACE

// Iterator
template<typename T>
SparseMatrixIterator<T>::SparseMatrixIterator(const SparseMatrix<T>& m, Index i1, Index i2) :
	mMatrix(&m), mIndex1(i1), mIndex2(i2), mColumnPtrIndex(0)
{
	if (isValid())
	{
		bool found;
		mMatrix->internal_at(mIndex1, mIndex2, mColumnPtrIndex, found);
		NS_ASSERT(found);
	}
}

template<typename T>
SparseMatrixIterator<T>& SparseMatrixIterator<T>::operator++ ()
{
	if (isValid())
	{
		mColumnPtrIndex++;

		if (mMatrix->mColumnPtr.size() <= mColumnPtrIndex)
		{
			mIndex1 = mMatrix->rows();
			mIndex2 = mMatrix->columns();
		}
		else
		{
			mIndex2 = mMatrix->mColumnPtr[mColumnPtrIndex];

			for (Index s1 = mIndex1; s1 < mMatrix->rows(); ++s1)
			{
				if (mMatrix->mRowPtr[s1] == mColumnPtrIndex)
				{
					mIndex1 = s1;
					break;
				}
				else if (mMatrix->mRowPtr[s1] > mColumnPtrIndex)
				{
					mIndex1 = s1 - 1;
					break;
				}
			}
		}
	}
	return *this;
}

template<typename T>
SparseMatrixIterator<T> SparseMatrixIterator<T>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

template<typename T>
T SparseMatrixIterator<T>::operator *() const
{
	if (isValid())
	{
		return mMatrix->mValues[mColumnPtrIndex];
	}
	else
	{
		return (T)0;
	}
}

template<typename T>
T& SparseMatrixIterator<T>::operator *()// This is really dirty
{
	if (isValid())
	{
		return const_cast<SparseMatrix<T>*>(mMatrix)->mValues[mColumnPtrIndex];
	}
	else
	{
		NS_ASSERT(mMatrix->mEmpty == (T)0);
		return const_cast<SparseMatrix<T>*>(mMatrix)->mEmpty;
	}
}

// Row
template<typename T>
SparseMatrixRowIterator<T>::SparseMatrixRowIterator(const SparseMatrix<T>& m, Index i1, Index i2) :
	SparseMatrixIterator<T>(m,i1,i2)
{
}

template<typename T>
SparseMatrixRowIterator<T>& SparseMatrixRowIterator<T>::operator++ ()
{
	if (this->isValid())
	{
		this->mColumnPtrIndex++;

		if (this->mMatrix->mColumnPtr.size() <= this->mColumnPtrIndex)
		{
			this->mIndex1 = this->mMatrix->rows();
			this->mIndex2 = this->mMatrix->columns();
		}
		else
		{
			this->mIndex2 = this->mMatrix->mColumnPtr[this->mColumnPtrIndex];

			if (this->mIndex1 != this->mMatrix->rows() - 1 &&
				this->mMatrix->mRowPtr[this->mIndex1 + 1] <= this->mColumnPtrIndex)
			{
				this->mIndex1 = this->mMatrix->rows();
				this->mIndex2 = this->mMatrix->columns();
			}
		}
	}
	return *this;
}

template<typename T>
SparseMatrixRowIterator<T> SparseMatrixRowIterator<T>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

// Column
template<typename T>
SparseMatrixColumnIterator<T>::SparseMatrixColumnIterator(const SparseMatrix<T>& m, Index i1, Index i2) :
	SparseMatrixIterator<T>(m,i1,i2)
{
}

template<typename T>
SparseMatrixColumnIterator<T>& SparseMatrixColumnIterator<T>::operator++ ()
{
	if (this->isValid())
	{
		bool found = false;
		do
		{
			this->mIndex1++;

			if (this->mIndex1 >= this->mMatrix->rows())
			{
				this->mIndex1 = this->mMatrix->rows();
				this->mIndex2 = this->mMatrix->columns();
				break;
			}

			Index rowPtr;
			size_t s = this->mMatrix->row_entry_count(this->mIndex1, rowPtr);
			
			for (this->mColumnPtrIndex = rowPtr; this->mColumnPtrIndex < rowPtr + s; ++this->mColumnPtrIndex)// O(D2)
			{
				auto ci = this->mMatrix->mColumnPtr[this->mColumnPtrIndex];// O(1)
				
				if (ci == this->mIndex2)
				{
					found = true;
					break;
				}
				else if (ci > this->mIndex2)
				{
					break;
				}
			}
		} while (!found);//O(D1)
	}

	return *this;
}

template<typename T>
SparseMatrixColumnIterator<T> SparseMatrixColumnIterator<T>::operator++ (int)
{
	auto c = *this;
	this->operator++ ();
	return c;
}

// Main
template<typename T>
SparseMatrix<T>::SparseMatrix(Dimension d1, Dimension d2) :
	mValues(), mColumnPtr(), mRowPtr(d1), mColumnCount(d2), mEmpty((T)0)
{
	NS_ASSERT(d1 > 0);
	NS_ASSERT(d2 > 0);
	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are ComplexNumber and the types allowed by std::is_floating_point.");
}

template<typename T>
SparseMatrix<T>::SparseMatrix(std::initializer_list<std::initializer_list<T> > l) :
	SparseMatrix(l.size(), 
		std::max_element(l.begin(), l.end(), [](const std::initializer_list<T>& a, const std::initializer_list<T>& b)
		{ return a.size() < b.size(); })->size())
{
	Index i = 0;
	for (const auto& v : l)
	{
		Index j = 0;
		for (const auto& w : v)
		{
			if (w != (T)0)
				set(i, j, w);

			++j;
		}
		++i;
	}
}

template<typename T>
SparseMatrix<T>::~SparseMatrix()
{
}

template<typename T>
void SparseMatrix<T>::resize(Dimension d1, Dimension d2)
{
	NS_ASSERT(d1 > 0);
	NS_ASSERT(d2 > 0);
	//TODO
}

/*
 Only for internal use.
 */
template<typename T>
const T& SparseMatrix<T>::internal_at(Index i1, Index i2, Index& columnPtrIndex, bool& found, bool needNear) const
{
	Index tmp_i1 = i1;

	found = false;
	columnPtrIndex = 0;

	if (!isEmpty())
	{
		Index rowPtr;
		size_t s = row_entry_count(tmp_i1, rowPtr);// O(1)
		while (needNear && s == 0 && tmp_i1 < (rows() - 1))// Nothing found, but we need the next close one.
		{
			tmp_i1 += 1;
			s = row_entry_count(tmp_i1, rowPtr);
		}

		for (columnPtrIndex = rowPtr; columnPtrIndex < rowPtr + s; ++columnPtrIndex)// O(D2)
		{
			auto ci = mColumnPtr[columnPtrIndex];// O(1)
			
			if (ci == i2)
			{
				found = (tmp_i1 == i1) ? true : false;
				return mValues[columnPtrIndex];// O(1)
			}
			else if (ci > i2)
			{
				break;
			}
		}
	}

	NS_ASSERT(mEmpty == (T)0);
	return mEmpty;
}

template<typename T>
void SparseMatrix<T>::remove_at(Index i1, Index i2)
{
	Index columnPtrIndex;
	bool found;
	internal_at(i1, i2, columnPtrIndex, found);// O(D2)

	if (found)
	{
		mValues.erase(mValues.begin() + columnPtrIndex);// O(D1*D2), due to the moving in std::vector<T>::erase
		mColumnPtr.erase(mColumnPtr.begin() + columnPtrIndex);// O(D1*D2), see above

		for (Index k = i1 + 1; k < rows(); ++k)// O(D1)
			mRowPtr[k] -= 1;
	}
}

template<typename T>
void SparseMatrix<T>::set_at(Index i1, Index i2, const T& v)
{
	Index columnPtrIndex;
	bool found;
	internal_at(i1, i2, columnPtrIndex, found, true);// O(D1*D2), due to the needNear == true

	if (found)// Replace
	{
		mValues[columnPtrIndex] = v; // O(1)
	}
	else// New
	{
		mValues.insert(mValues.begin() + columnPtrIndex, v);// O(D1*D2)
		mColumnPtr.insert(mColumnPtr.begin() + columnPtrIndex, i2);// O(D1*D2)

		for (Index k = i1 + 1; k < rows(); ++k)// O(D1)
			mRowPtr[k] += 1;
	}
}

template<typename T>
size_t SparseMatrix<T>::row_entry_count(Index i, Index& rowPtr) const
{
	if (i < rows())
	{
		rowPtr = mRowPtr[i];// O(1)
		return i != (rows() - 1) ? mRowPtr[i + 1] - rowPtr : mColumnPtr.size() - rowPtr;
	}
	else
	{
		rowPtr = 0;
		return 0;
	}
}

template<typename T>
T SparseMatrix<T>::at(Index i1, Index i2) const
{
	NS_ASSERT(i1 < rows());
	NS_ASSERT(i2 < columns());

	Index tmp;
	bool found;
	return internal_at(i1, i2, tmp, found);// O(D2)
}

template<typename T>
void SparseMatrix<T>::set(Index i1, Index i2, const T& t)
{
	NS_ASSERT(i1 < rows());
	NS_ASSERT(i2 < columns());

	if (t == (T)0)
		remove_at(i1, i2);// O(D1*D2)
	else
		set_at(i1, i2, t);// 
}

template<typename T>
T SparseMatrix<T>::linear_at(Index i) const
{
	NS_ASSERT(i < size());
	return at(i / columns(), i % columns());
}

template<typename T>
void SparseMatrix<T>::linear_set(Index i, const T& v)
{
	NS_ASSERT(i < size());
	set(i / columns(), i % columns(), v);
}

template<typename T>
bool SparseMatrix<T>::has(Index i1, Index i2) const
{
	NS_ASSERT(i1 < rows());
	NS_ASSERT(i2 < columns());

	Index tmp;
	bool found;
	internal_at(i1, i2, tmp, found);

	return found;
}

template<typename T>
bool SparseMatrix<T>::has(Index i1, Index i2, T& val) const
{
	NS_ASSERT(i1 < rows());
	NS_ASSERT(i2 < columns());

	Index tmp;
	bool found;
	val = internal_at(i1, i2, tmp, found);

	return found;
}

// Iterators
template<typename T>
const SparseMatrixIterator<T> SparseMatrix<T>::begin() const
{
	if (isEmpty())
		return end();
	else
	{
		Index row = 0;
		Index rowPtr;
		while (row_entry_count(row, rowPtr) == 0 && row != rows())
			++row;

		if (row == rows())
			return end();
		else
			return iterator(*this, row, mColumnPtr.front());
	}
}

template<typename T>
const SparseMatrixIterator<T> SparseMatrix<T>::end() const
{
	return iterator(*this, rows(), columns());
}

template<typename T>
SparseMatrixIterator<T> SparseMatrix<T>::begin()
{
	if (isEmpty())
		return end();
	else
	{
		Index row = 0;
		Index rowPtr;
		while (row_entry_count(row, rowPtr) == 0 && row != rows())
			++row;

		if (row == rows())
			return end();
		else
			return iterator(*this, row, mColumnPtr[rowPtr]);
	}
}

template<typename T>
SparseMatrixIterator<T> SparseMatrix<T>::end()
{
	return iterator(*this, rows(), columns());
}

template<typename T>
const SparseMatrixIterator<T> SparseMatrix<T>::cbegin() const
{
	return begin();
}

template<typename T>
const SparseMatrixIterator<T> SparseMatrix<T>::cend() const
{
	return end();
}

template<typename T>
const SparseMatrixRowIterator<T> SparseMatrix<T>::row_begin(Index i) const
{
	Index tmp;
	if (isEmpty() || row_entry_count(i, tmp) == 0)
		return row_end(i);
	else
		return row_iterator(*this, i, mColumnPtr[tmp]);
}

template<typename T>
const SparseMatrixRowIterator<T> SparseMatrix<T>::row_end(Index) const
{
	return row_iterator(*this, rows(), columns());
}

template<typename T>
SparseMatrixRowIterator<T> SparseMatrix<T>::row_begin(Index i)
{
	Index tmp;
	if (isEmpty() || row_entry_count(i, tmp) == 0)
		return row_end(i);
	else
		return row_iterator(*this, i, mColumnPtr[tmp]);
}

template<typename T>
SparseMatrixRowIterator<T> SparseMatrix<T>::row_end(Index)
{
	return row_iterator(*this, rows(), columns());
}

template<typename T>
const SparseMatrixRowIterator<T> SparseMatrix<T>::row_cbegin(Index i) const
{
	return row_begin(i);
}

template<typename T>
const SparseMatrixRowIterator<T> SparseMatrix<T>::row_cend(Index i) const
{
	return row_end(i);
}

//Column
template<typename T>
const SparseMatrixColumnIterator<T> SparseMatrix<T>::column_begin(Index i) const
{
	if (isEmpty())
		return column_end(i);
	else
	{
		Index row = 0;
		while (!has(row, i) && row != rows())
			++row;

		if (row == rows())
			return column_end(i);
		else
			return column_iterator(*this, row, i);
	}
}

template<typename T>
const SparseMatrixColumnIterator<T> SparseMatrix<T>::column_end(Index) const
{
	return column_iterator(*this, rows(), columns());
}

template<typename T>
SparseMatrixColumnIterator<T> SparseMatrix<T>::column_begin(Index i)
{
	if (isEmpty())
		return column_end(i);
	else
	{
		Index row = 0;
		while (!has(row, i) && row != rows())
			++row;

		if (row == rows())
			return column_end(i);
		else
			return column_iterator(*this, row, i);
	}
}

template<typename T>
SparseMatrixColumnIterator<T> SparseMatrix<T>::column_end(Index)
{
	return column_iterator(*this, rows(), columns());
}

template<typename T>
const SparseMatrixColumnIterator<T> SparseMatrix<T>::column_cbegin(Index i) const
{
	return column_begin(i);
}

template<typename T>
const SparseMatrixColumnIterator<T> SparseMatrix<T>::column_cend(Index i) const
{
	return column_end(i);
}

// Set/Erase
template<typename T>
SparseMatrixIterator<T> SparseMatrix<T>::set(const SparseMatrixIterator<T>& it, const T& val)
{
	NS_ASSERT(this == it.mMatrix);

	if (it.isValid())
	{
		if (val == (T)0)
			return erase(it);
		else
		{
			SparseMatrixIterator<T> nit = it;
			mValues[nit.mColumnPtrIndex] = val;
			return nit;
		}
	}
	else
	{
		return it;
	}
}

template<typename T>
SparseMatrixIterator<T> SparseMatrix<T>::erase(const SparseMatrixIterator<T>& it)
{
	NS_ASSERT(this == it.mMatrix);

	if (it.isValid())
	{
		mValues.erase(mValues.begin() + it.mColumnPtrIndex);
		auto cIt = mColumnPtr.erase(mColumnPtr.begin() + it.mColumnPtrIndex);

		for (Index k = it.row() + 1; k < rows(); ++k)
		{
			mRowPtr[k] -= 1;
		}

		SparseMatrixIterator<T> nIt = it;
		if (cIt == mColumnPtr.end())// End/Empty
		{
			nIt.mIndex1 = rows();
			nIt.mIndex2 = columns();
		}
		else
		{
			nIt.mIndex2 = *cIt;

			for (Index s1 = it.row(); s1 < rows(); ++s1)
			{
				if (mRowPtr[s1] == it.mColumnPtrIndex)
				{
					nIt.mIndex1 = s1;// New row
					break;
				}
				else if (mRowPtr[s1] > it.mColumnPtrIndex)
				{
					break;
				}
			}
		}

		return nIt;
	}
	else
	{
		return it;
	}
}

// Operators
template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator +=(const SparseMatrix<T>& v2)
{
	for (auto it = v2.cbegin(); it != v2.cend(); ++it)// O(D1*D2)
		set(it.row(), it.column(), at(it.row(), it.column()) + *it);// O(D1*D2)

	return *this;
}

template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator -=(const SparseMatrix<T>& v2)
{
	for (auto it = v2.cbegin(); it != v2.cend(); ++it)
		set(it.row(), it.column(), at(it.row(), it.column()) - *it);

	return *this;
}

template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator *=(const SparseMatrix<T>& v2)
{
	for (auto it = v2.cbegin(); it != v2.cend(); ++it)// O(D1*D2)
	{
		bool found;
		Index tmp;
		T& val = const_cast<T&>(internal_at(it.row(), it.column(), tmp, found));// O(D2)

		val *= *it;
	}

	for (auto it = begin(); it != end();)// O(D1*D2)
	{
		if (!v2.has(it.row(), it.column()))// O(D2)
			it = erase(it);// O(D1*D2)
		else
			++it;
	}

	return *this;
}

template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator *=(const T& f)
{
	if (f == (T)0)
	{
		mValues.clear();
		mColumnPtr.clear();
		std::transform(mRowPtr.begin(), mRowPtr.end(), mRowPtr.begin(), [](Index i) -> Index { return 0; });// Better way? O(D1)
	}
	else
	{
		for (auto it = begin(); it != end(); ++it)// O(D1*D2)
			*it *= f; // O(1)
	}

	return *this;
}

template<typename T>
constexpr Dimension SparseMatrix<T>::columns() const
{
	return mColumnCount;
}

template<typename T>
constexpr Dimension SparseMatrix<T>::rows() const
{
	return mRowPtr.size();
}

template<typename T>
constexpr Dimension SparseMatrix<T>::size() const
{
	return rows()*columns();
}

template<typename T>
Dimension SparseMatrix<T>::filledCount() const
{
	return mValues.size();
}

template<typename T>
bool SparseMatrix<T>::isEmpty() const
{
	return mValues.empty();
}

template<typename T>
T SparseMatrix<T>::sum() const
{
	T s = 0;
	for (auto l : mValues)
		s += l;

	return s;
}

template<typename T>
T SparseMatrix<T>::max() const
{
	T s = std::numeric_limits<typename get_complex_internal<T>::type>::min();
	for (auto l : mValues)
	{
		if (l > s)
			s = l;
	}

	if (mValues.size() != size() && 0 > s)
		s = 0;

	return s;
}

template<typename T>
T SparseMatrix<T>::min() const
{
	T s = std::numeric_limits<typename get_complex_internal<T>::type>::max();
	for (auto l : mValues)
	{
		if (l < s)
			s = l;
	}

	if (mValues.size() != size() && 0 < s)
		s = 0;

	return s;
}

template<typename T>
T SparseMatrix<T>::avg() const
{
	return sum() / size();// Overflow?
}

template<typename T>
bool SparseMatrix<T>::hasNaN() const
{
	for (auto l : mValues)
	{
		if (std::isnan(l))
			return true;
	}

	return false;
}

template<typename T>
bool SparseMatrix<T>::hasInf() const
{
	for (auto l : mValues)
	{
		if (std::isinf(l))
			return true;
	}

	return false;
}

template<typename T>
bool SparseMatrix<T>::hasZero() const
{
	return mValues.size() != size();
}

template<typename T>
void SparseMatrix<T>::swap(SparseMatrix<T>& v)
{
	std::swap(mValues, v.mValues);
	std::swap(mColumnPtr, v.mColumnPtr);
	std::swap(mRowPtr, v.mRowPtr);
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::transpose()
{
	SparseMatrix<T> tmp(columns(), rows());

	for (auto it = begin(); it != end(); ++it)// O(D1*D2)
		tmp.set(it.column(), it.row(), *it);// O(D1*D2)

	return tmp;
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::conjugate()
{
	SparseMatrix<T> tmp(rows(), columns());

	for (auto it = begin(); it != end(); ++it)
		tmp.set(it.row(), it.column(), conjugate_vt(*it));

	return tmp;
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::adjugate()
{
	SparseMatrix<T> tmp(columns(), rows());

	for (auto it = begin(); it != end(); ++it)// O(D1*D2)
		tmp.set(it.column(), it.row(), conjugate_vt(*it));// O(D1*D2)

	return tmp;
}

template<typename T>
T SparseMatrix<T>::trace() const
{
	T v = (T)0;
	for (Index i = 0; i < t_min(rows(), columns()); ++i)
		v += at(i, i);// O(D2)

	return v;
}

//TODO: Improve
template<typename T>
SparseMatrix<T> SparseMatrix<T>::mul(const SparseMatrix<T>& m) const
{
	if (columns() != m.rows())
		throw MatrixMulMismatchException();

	SparseMatrix<T> tmp(rows(), m.columns());
	for (Index i = 0; i < rows(); ++i)// O(D1)
	{
		for (Index k = 0; k < m.columns(); ++k)// O(D3)
		{
			T v = 0;
			for (Index j = 0; j < columns(); ++j)// O(D2)
			{
				v += at(i, j) * m.at(j, k);// O(D2) + O(D3)
			}
			tmp.set(i, k, v);// O(D1*D3)
		}
	}

	return tmp;
}

template<typename T>
Vector<T> SparseMatrix<T>::mul(const Vector<T>& v) const
{
	if (columns() != v.size())
		throw MatrixMulMismatchException();

	Vector<T> r(rows());
	for (auto it = begin(); it != end(); ++it)// O(D1*D2)
		r.set(it.row(), r.at(it.row()) + *it * v.at(it.column()));// 2*O(D1)+O(D2)

	return r;
}

template<typename T>
Vector<T> SparseMatrix<T>::mul_left(const Vector<T>& v) const
{
	if (rows() != v.size())
		throw MatrixMulMismatchException();

	Vector<T> r(columns());
	for (auto it = begin(); it != end(); ++it)// O(D1*D2)
		r.set(it.column(), r.at(it.column()) + *it * v.at(it.row()));// 2*O(D1)+O(D2)

	return r;
}

template<typename T>
SparseMatrix<T> operator +(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2)
{
	SparseMatrix<T>  tmp = v1;
	return (tmp += v2);
}

template<typename T>
SparseMatrix<T> operator -(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2)
{
	SparseMatrix<T> tmp = v1;
	return (tmp -= v2);
}

template<typename T>
SparseMatrix<T> operator -(const SparseMatrix<T>& v)
{
	SparseMatrix<T> tmp = v;
	return (tmp *= -1);
}

template<typename T>
SparseMatrix<T> operator *(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2)
{
	SparseMatrix<T> tmp = v1;
	return (tmp *= v2);
}

template<typename T>
SparseMatrix<T> operator *(const SparseMatrix<T>& v1, T f)
{
	SparseMatrix<T> tmp = v1;
	return (tmp *= f);
}

template<typename T>
SparseMatrix<T> operator *(T f, const SparseMatrix<T>& v1)
{
	return v1 * f;
}

template<typename T>
bool operator ==(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2)
{
	if(v1.rows() != v2.rows() || v1.columns() != v2.columns() || v1.filledCount() != v2.filledCount())
		return false;

	for (auto it = v1.begin(); it != v1.end(); ++it)
	{
		T val;
		if (!v2.has(it.row(), it.column(), val) || *it != val)
			return false;
	}

	return true;
}

template<typename T>
bool operator !=(const SparseMatrix<T>& v1, const SparseMatrix<T>& v2)
{
	return !(v1 == v2);
}

NS_END_NAMESPACE