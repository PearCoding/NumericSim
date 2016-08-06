#ifndef _NS_SPARSEMATRIX_INL
# error SparseMatrix.inl should only be included by SparseMatrix.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>::SparseMatrix() :
	SparseCountableSet()
{
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>::SparseMatrix(std::initializer_list<std::initializer_list<T> > l) :
	SparseMatrix()
{
	NS_ASSERT(l.size() <= D2);// Would be great if we could use size() with constexpr [C++14]
	Index i = 0;
	for (const auto& v : l)
	{
		Index j = 0;
		NS_ASSERT(v.size() <= D1);

		for (const auto& w : v)
		{
			if(w != 0)
				mData[i*D1 + j] = w;

			++j;
		}
		++i;
	}
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>::~SparseMatrix()
{
}

template<typename T, Dimension D1, Dimension D2>
T SparseMatrix<T, D1, D2>::at(Index i1, Index i2) const
{
	NS_ASSERT(i1 < D1);
	NS_ASSERT(i2 < D2);
	return linear_at(i2*D1 + i1);
}

template<typename T, Dimension D1, Dimension D2>
void SparseMatrix<T, D1, D2>::set(Index i1, Index i2, const T& t)
{
	NS_ASSERT(i1 < D1);
	NS_ASSERT(i2 < D2);
	linear_set(i2*D1 + i1, t);
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>& SparseMatrix<T, D1, D2>::operator +=(const SparseMatrix<T, D1, D2>& v2)
{
	for (const auto& p : v2.mData)
		mData[p.first] = (mData.count(p.first) ? mData[p.first] : 0) + p.second;

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>& SparseMatrix<T, D1, D2>::operator -=(const SparseMatrix<T, D1, D2>& v2)
{
	for (const auto& p : v2.mData)
		mData[p.first] = (mData.count(p.first) ? mData[p.first] : 0) - p.second;

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>& SparseMatrix<T, D1, D2>::operator *=(const SparseMatrix<T, D1, D2>& v2)
{
	for (const auto& p : v2.mData)
	{
		if (mData.count(p.first))
			mData[p.first] *= p.second;
		else
			mData.erase(p.first);
	}

	for (auto it mData.begin(); it != mData.end(); ++it)
	{
		if (!v.mData.count(it->first))
			it = mData.erase(it);
	}

	return *this;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2>& SparseMatrix<T, D1, D2>::operator *=(const T& f)
{
	if (f == 0)
		mData.clear();
	else
	{
		for (auto p : mData)// Better way?
			mData[p.first] *= f;
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
SparseMatrix<T, D2, D1> SparseMatrix<T, D1, D2>::transpose()
{
	SparseMatrix<T, D2, D1> tmp;
	for (Index i = 0; i < D2; ++i)
	{
		for (Index j = 0; j < D1; ++j)
		{
			tmp.set(i, j, at(j, i));
		}
	}
	return tmp;
}

template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> SparseMatrix<T, D1, D2>::invert()
{
	//TODO
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
	for (Index i = 0; i < D1*D2; ++i)
	{
		if (v1.linear_at(i) != v2.linear_at(i))
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