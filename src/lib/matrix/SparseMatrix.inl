#ifndef _NS_SPARSEMATRIX_INL
# error SparseMatrix.inl should only be included by SparseMatrix.h
#endif

NS_BEGIN_NAMESPACE

// Iterator
template<typename T, Dimension D1, Dimension D2>
SparseMatrixIterator<T, D1, D2>::SparseMatrixIterator(const SparseMatrix<T, D1, D2>& m, Index i1, Index i2) :
	mMatrix(&m), mIndex1(i1), mIndex2(i2), mColumnPtrIndex(0)
{
	if (mIndex1 < D1 && mIndex2 < D2)
	{
		bool found;
		mMatrix->internal_at(mIndex1, mIndex2, mColumnPtrIndex, found);
	}
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrixIterator<T,D1,D2>& SparseMatrixIterator<T,D1,D2>::operator++ ()
{
	if (mIndex1 < D1 && mIndex2 < D2)
	{
		mColumnPtrIndex++;

		if (mMatrix->mColumnPtr.size() <= mColumnPtrIndex)
		{
			mIndex1 = D1;
			mIndex2 = D2;
		}
		else
		{
			mIndex2 = mMatrix->mColumnPtr[mColumnPtrIndex];

			for (Index s1 = mIndex1; s1 < D1; ++s1)
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

template<typename T, Dimension D1, Dimension D2>
SparseMatrixIterator<T, D1, D2> SparseMatrixIterator<T, D1, D2>::operator++ (int)
{
	auto c = *this;
	SparseMatrixIterator<T, D1, D2>::operator++ ();
	return c;
}

template<typename T, Dimension D1, Dimension D2>
T SparseMatrixIterator<T, D1, D2>::operator *() const
{
	if (mIndex1 < D1 && mIndex2 < D2)
	{
		return mMatrix->mValues[mColumnPtrIndex];
	}
	else
	{
		return (T)0;
	}
}

template<typename T, Dimension D1, Dimension D2>
T& SparseMatrixIterator<T, D1, D2>::operator *()// This is really dirty
{
	if (mIndex1 < D1 && mIndex2 < D2)
	{
		return const_cast<SparseMatrix<T,D1,D2>*>(mMatrix)->mValues[mColumnPtrIndex];
	}
	else
	{
		return const_cast<SparseMatrix<T, D1, D2>*>(mMatrix)->mEmpty;
	}
}

// Main
template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>::SparseMatrix() :
	mValues(), mColumnPtr(), mRowPtr(D1), mEmpty((T)0)
{
	static_assert(D1 > 0, "Dimension (Row) has to be greater then 0.");
	static_assert(D2 > 0, "Dimension (Column) has to be greater then 0.");
	static_assert(is_number<T>::value, "Type T has to be a number.\nAllowed are ComplexNumber and the types allowed by std::is_floating_point.");
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>::SparseMatrix(std::initializer_list<std::initializer_list<T> > l) :
	SparseMatrix()
{
	if (l.size() > D1)
		throw MatrixInitializerListFailedException();

	Index i = 0;
	for (const auto& v : l)
	{
		Index j = 0;
		if (v.size() > D2)
			throw MatrixInitializerListFailedException();

		for (const auto& w : v)
		{
			if (w != (T)0)
				set(i, j, w);

			++j;
		}
		++i;
	}
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>::~SparseMatrix()
{
}

/*
 Only for internal use.
 columnPtrIndex has the found index for columnPtr or the one next to the new entry position (not created!).
 */
template<typename T, Dimension D1, Dimension D2>
const T& SparseMatrix<T, D1, D2>::internal_at(Index i1, Index i2, Index& columnPtrIndex, bool& found, bool needNear) const
{
	found = false;
	columnPtrIndex = 0;

	if (!isEmpty())
	{
		Index rowPtr = mRowPtr[i1];// O(1)
		size_t s = i1 != (D1 - 1) ? mRowPtr[i1 + 1] - rowPtr : mColumnPtr.size() - rowPtr;
		if (needNear && s == 0 && i1 != (D1 - 1))// Nothing found, but we need next close one.
		{
			// Recursion for very big data is not that good :/
			bool found2;
			internal_at(i1 + 1, 0, columnPtrIndex, found2, true);// O(D2)
		}
		else
		{
			for (columnPtrIndex = rowPtr; columnPtrIndex < rowPtr + s; ++columnPtrIndex)// O(D2)
			{
				auto ci = mColumnPtr[columnPtrIndex];// O(1)

				if (ci == i2)
				{
					found = true;
					return mValues[columnPtrIndex];// O(1)
				}
				else if (ci > i2)
				{
					break;
				}
			}
		}
	}

	NS_ASSERT(mEmpty == (T)0);
	return mEmpty;
}

template<typename T, Dimension D1, Dimension D2>
void SparseMatrix<T, D1, D2>::remove_at(Index i1, Index i2)
{
	Index columnPtrIndex;
	bool found;
	internal_at(i1, i2, columnPtrIndex, found);// O(D2)

	if (found)
	{
		mValues.erase(mValues.begin() + columnPtrIndex);// O(D1*D2), due to the moving in std::vector<T>::erase
		mColumnPtr.erase(mColumnPtr.begin() + columnPtrIndex);// O(D1*D2), see above

		for (Index k = i1 + 1; k < D1; ++k)// O(D1)
		{
			mRowPtr[k] -= 1;
		}
	}
}

template<typename T, Dimension D1, Dimension D2>
void SparseMatrix<T, D1, D2>::set_at(Index i1, Index i2, const T& v)
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

		for (Index k = i1 + 1; k < D1; ++k)// O(D1)
		{
			mRowPtr[k] += 1;
		}
	}
}

template<typename T, Dimension D1, Dimension D2>
T SparseMatrix<T, D1, D2>::at(Index i1, Index i2) const
{
	NS_ASSERT(i1 < D1);
	NS_ASSERT(i2 < D2);

	Index tmp;
	bool found;
	return internal_at(i1, i2, tmp, found);// O(D2)
}

template<typename T, Dimension D1, Dimension D2>
void SparseMatrix<T, D1, D2>::set(Index i1, Index i2, const T& t)
{
	NS_ASSERT(i1 < D1);
	NS_ASSERT(i2 < D2);

	if (t == (T)0)
		remove_at(i1, i2);// O(D1*D2)
	else
		set_at(i1, i2, t);// 
}

template<typename T, Dimension D1, Dimension D2>
T SparseMatrix<T, D1, D2>::linear_at(Index i) const
{
	NS_ASSERT(i < D1*D2);
	return at(i / D2, i % D2);
}

template<typename T, Dimension D1, Dimension D2>
void SparseMatrix<T, D1, D2>::linear_set(Index i, const T& v)
{
	NS_ASSERT(i < D1*D2);
	return set(i / D2, i % D2, v);
}

template<typename T, Dimension D1, Dimension D2>
bool SparseMatrix<T, D1, D2>::has(Index i1, Index i2) const
{
	NS_ASSERT(i1 < D1);
	NS_ASSERT(i2 < D2);

	Index tmp;
	bool found;
	internal_at(i1, i2, tmp, found);

	return found;
}

template<typename T, Dimension D1, Dimension D2>
bool SparseMatrix<T, D1, D2>::has(Index i1, Index i2, T& val) const
{
	NS_ASSERT(i1 < D1);
	NS_ASSERT(i2 < D2);

	Index tmp;
	bool found;
	val = internal_at(i1, i2, tmp, found);

	return found;
}

// Iterators
template<typename T, Dimension D1, Dimension D2>
const SparseMatrixIterator<T, D1, D2> SparseMatrix<T, D1, D2>::begin() const
{
	if (isEmpty())
		return end();
	else
		return iterator(*this, mRowPtr.front(), mColumnPtr.front());
}

template<typename T, Dimension D1, Dimension D2>
const SparseMatrixIterator<T, D1, D2> SparseMatrix<T, D1, D2>::end() const
{
	return iterator(*this, D1, D2);
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrixIterator<T, D1, D2> SparseMatrix<T, D1, D2>::begin()
{
	if (isEmpty())
		return end();
	else
		return iterator(*this, mRowPtr.front(), mColumnPtr.front());
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrixIterator<T, D1, D2> SparseMatrix<T, D1, D2>::end()
{
	return iterator(*this, D1, D2);
}

template<typename T, Dimension D1, Dimension D2>
const SparseMatrixIterator<T, D1, D2> SparseMatrix<T, D1, D2>::cbegin() const
{
	return begin();
}

template<typename T, Dimension D1, Dimension D2>
const SparseMatrixIterator<T, D1, D2> SparseMatrix<T, D1, D2>::cend() const
{
	return end();
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrixIterator<T, D1, D2> SparseMatrix<T, D1, D2>::set(const SparseMatrixIterator<T, D1, D2>& it, const T& val)
{
	NS_ASSERT(this == it.mMatrix);

	if (it.row() < D1 && it.column() < D2)
	{
		if (val == (T)0)
			return erase(it);
		else
		{
			SparseMatrixIterator<T, D1, D2> nit = it;
			mValues[nit.mColumnPtrIndex] = val;
			return nit;
		}
	}
	else
	{
		return it;
	}
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrixIterator<T, D1, D2> SparseMatrix<T, D1, D2>::erase(const SparseMatrixIterator<T, D1, D2>& it)
{
	NS_ASSERT(this == it.mMatrix);

	if (it.row() < D1 && it.column() < D2)
	{
		mValues.erase(mValues.begin() + it.mColumnPtrIndex);
		auto cIt = mColumnPtr.erase(mColumnPtr.begin() + it.mColumnPtrIndex);

		for (Index k = it.row() + 1; k < D1; ++k)
		{
			mRowPtr[k] -= 1;
		}

		SparseMatrixIterator<T, D1, D2> nIt = it;
		if (cIt == mColumnPtr.end())// End/Empty
		{
			nIt.mIndex1 = D1;
			nIt.mIndex2 = D2;
		}
		else
		{
			nIt.mIndex2 = *cIt;

			for (Index s1 = it.row(); s1 < D1; ++s1)
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

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>& SparseMatrix<T, D1, D2>::operator +=(const SparseMatrix<T, D1, D2>& v2)
{
	for (auto it = v2.cbegin(); it != v2.cend(); ++it)// O(D1*D2)
		set(it.row(), it.column(), at(it.row(), it.column()) + *it);// O(D1*D2)

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>& SparseMatrix<T, D1, D2>::operator -=(const SparseMatrix<T, D1, D2>& v2)
{
	for (auto it = v2.cbegin(); it != v2.cend(); ++it)
		set(it.row(), it.column(), at(it.row(), it.column()) - *it);

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>& SparseMatrix<T, D1, D2>::operator *=(const SparseMatrix<T, D1, D2>& v2)
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

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>& SparseMatrix<T, D1, D2>::operator *=(const T& f)
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

template<typename T, Dimension D1, Dimension D2>
constexpr Dimension SparseMatrix<T, D1, D2>::columns() const
{
	return D1;
}

template<typename T, Dimension D1, Dimension D2>
constexpr Dimension SparseMatrix<T, D1, D2>::rows() const
{
	return D2;
}

template<typename T, Dimension D1, Dimension D2>
constexpr Dimension SparseMatrix<T, D1, D2>::size() const
{
	return D1*D2;
}

template<typename T, Dimension D1, Dimension D2>
Dimension SparseMatrix<T, D1, D2>::filledCount() const
{
	return mValues.size();
}

template<typename T, Dimension D1, Dimension D2>
bool SparseMatrix<T, D1, D2>::isEmpty() const
{
	return mValues.empty();
}

template<typename T, Dimension D1, Dimension D2>
T SparseMatrix<T, D1, D2>::sum() const
{
	T s = 0;
	for (auto l : mValues)
		s += l;

	return s;
}

template<typename T, Dimension D1, Dimension D2>
T SparseMatrix<T, D1, D2>::max() const
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

template<typename T, Dimension D1, Dimension D2>
T SparseMatrix<T, D1, D2>::min() const
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

template<typename T, Dimension D1, Dimension D2>
T SparseMatrix<T, D1, D2>::avg() const
{
	return sum() / size();// Overflow?
}

template<typename T, Dimension D1, Dimension D2>
bool SparseMatrix<T, D1, D2>::hasNaN() const
{
	for (auto l : mValues)
	{
		if (std::isnan(l))
			return true;
	}

	return false;
}

template<typename T, Dimension D1, Dimension D2>
bool SparseMatrix<T, D1, D2>::hasInf() const
{
	for (auto l : mValues)
	{
		if (std::isinf(l))
			return true;
	}

	return false;
}

template<typename T, Dimension D1, Dimension D2>
bool SparseMatrix<T, D1, D2>::hasZero() const
{
	return mValues.size() != size();
}

template<typename T, Dimension D1, Dimension D2>
void SparseMatrix<T, D1, D2>::swap(SparseMatrix<T, D1, D2>& v)
{
	std::swap(mValues, v.mValues);
	std::swap(mColumnPtr, v.mColumnPtr);
	std::swap(mRowPtr, v.mRowPtr);
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D2, D1> SparseMatrix<T, D1, D2>::transpose()
{
	SparseMatrix<T, D2, D1> tmp;

	for (auto it = begin(); it != end(); ++it)// O(D1*D2)
		tmp.set(it.column(), it.row(), *it);// O(D1*D2)

	return tmp;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> SparseMatrix<T, D1, D2>::conjugate()
{
	SparseMatrix<T, D1, D2> tmp;

	for (auto it = begin(); it != end(); ++it)
		tmp.set(it.row(), it.column(), conjugate_vt(*it));

	return tmp;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D2, D1> SparseMatrix<T, D1, D2>::adjugate()
{
	return transpose().conjugate();
}

template<typename T, Dimension D1, Dimension D2>
T SparseMatrix<T, D1, D2>::trace() const
{
	T v = (T)0;
	for (Index i = 0; i < t_min(D1, D2); ++i)
	{
		v += at(i, i);// O(D2)
	}

	return v;
}

//TODO: Improve
template<typename T, Dimension D1, Dimension D2>
template<Dimension D3>
SparseMatrix<T, D1, D3> SparseMatrix<T, D1, D2>::mul(const SparseMatrix<T, D2, D3>& m) const
{
	SparseMatrix<T, D1, D3> tmp;
	for (Index i = 0; i < D1; ++i)// O(D1)
	{
		for (Index k = 0; k < D3; ++k)// O(D3)
		{
			T v = 0;
			for (Index j = 0; j < D2; ++j)// O(D2)
			{
				v += at(i, j) * m.at(j, k);// O(D2) + O(D3)
			}
			tmp.set(i, k, v);// O(D1*D3)
		}
	}

	return tmp;
}

template<typename T, Dimension D1, Dimension D2>
Vector<T, D1> SparseMatrix<T, D1, D2>::mul(const Vector<T, D2>& m) const
{
	Vector<T, D1> r;
	for (auto it = begin(); it != end(); ++it)// O(D1*D2)
		r.set(it.row(), r.at(it.row()) + *it * m.at(it.column()));// 2*O(D1)+O(D2)
	return r;
}

template<typename T, Dimension D1, Dimension D2>
Vector<T, D2> SparseMatrix<T, D1, D2>::mul_left(const Vector<T, D1>& m) const
{
	Vector<T, D2> r;
	for (auto it = begin(); it != end(); ++it)// O(D1*D2)
		r.set(it.column(), r.at(it.column()) + *it * m.at(it.row()));// 2*O(D1)+O(D2)
	return r;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator +(const SparseMatrix<T, D1, D2>& v1, const SparseMatrix<T, D1, D2>& v2)
{
	SparseMatrix<T, D1, D2>  tmp = v1;
	return (tmp += v2);
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator -(const SparseMatrix<T, D1, D2>& v1, const SparseMatrix<T, D1, D2>& v2)
{
	SparseMatrix<T, D1, D2> tmp = v1;
	return (tmp -= v2);
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator -(const SparseMatrix<T, D1, D2>& v)
{
	SparseMatrix<T, D1, D2>  tmp = v;
	return (tmp *= -1);
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator *(const SparseMatrix<T, D1, D2 >& v1, const SparseMatrix<T, D1, D2>& v2)
{
	SparseMatrix<T, D1, D2> tmp = v1;
	return (tmp *= v2);
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator *(const SparseMatrix<T, D1, D2>& v1, T f)
{
	SparseMatrix<T, D1, D2> tmp = v1;
	return (tmp *= f);
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator *(T f, const SparseMatrix<T, D1, D2>& v1)
{
	return v1 * f;
}

template<typename T, Dimension D1, Dimension D2>
bool operator ==(const SparseMatrix<T, D1, D2>& v1, const SparseMatrix<T, D1, D2>& v2)
{
	if (v1.filledCount() != v2.filledCount())
		return false;

	for (auto it = v1.begin(); it != v1.end(); ++it)
	{
		T val;
		if (!v2.has(it.row(), it.column(), val) || *it != val)
			return false;
	}

	return true;
}

template<typename T, Dimension D1, Dimension D2>
bool operator !=(const SparseMatrix<T, D1, D2>& v1, const SparseMatrix<T, D1, D2>& v2)
{
	return !(v1 == v2);
}

NS_END_NAMESPACE