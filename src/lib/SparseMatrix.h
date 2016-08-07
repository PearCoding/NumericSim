#pragma once

#include "Config.h"
#include "ComplexNumber.h"

#include <initializer_list>
#include <type_traits>
#include <vector>
#include <algorithm>

NS_BEGIN_NAMESPACE

template<typename T, Dimension D1, Dimension D2>
class SparseMatrix;

/*
 Iterator class
 */
template<typename T, Dimension D1, Dimension D2>
class SparseMatrixIterator
{
private:
	friend SparseMatrix<T, D1, D2>;
	SparseMatrixIterator(const SparseMatrix<T, D1, D2>& m, Index i1, Index i2);

public:

	Index row() const
	{
		return mIndex1;
	}

	Index column() const
	{
		return mIndex2;
	}

	bool operator ==(const SparseMatrixIterator& other) const
	{
		return mIndex1 == other.mIndex1 && mIndex2 == other.mIndex2;
	}

	bool operator !=(const SparseMatrixIterator& other) const
	{
		return !(*this == other);
	}

	T operator *() const;
	SparseMatrixIterator& operator++ ();
	SparseMatrixIterator operator++ (int);
private:
	const SparseMatrix<T, D1, D2>* mMatrix;
	Index mIndex1;
	Index mIndex2;
	Index mColumnPtrIndex;
};

/** High dimensional sparse matrix class.
* Math order: First row, Second column!
* Does not derive from CountableSet -> Is sparse
*/
template<typename T, Dimension D1, Dimension D2>
class SparseMatrix
{
	friend SparseMatrixIterator<T, D1, D2>;
private:
	std::vector<T> mValues;
	std::vector<Index> mColumnPtr;
	std::vector<Index> mRowPtr;

	T mEmpty;//Used for references; Could also be static, but that would break the header only approach

protected:
	const T& internal_at(Index i1, Index i2, Index& columnPtrIndex, bool& found) const;
	void remove_at(Index i1, Index i2);
	void set_at(Index i1, Index i2, const T& v);

public:
	typedef SparseMatrixIterator<T, D1, D2> iterator;
	typedef const SparseMatrixIterator<T, D1, D2> const_iterator;

	SparseMatrix();
	SparseMatrix(std::initializer_list<std::initializer_list<T> > l);

	virtual ~SparseMatrix();

	const SparseMatrixIterator<T, D1, D2> begin() const;
	const SparseMatrixIterator<T, D1, D2> end() const;
	SparseMatrixIterator<T, D1, D2> begin();
	SparseMatrixIterator<T, D1, D2> end();
	const SparseMatrixIterator<T, D1, D2> cbegin() const;
	const SparseMatrixIterator<T, D1, D2> cend() const;

	// Index
	T at(Index i1, Index i2) const;
	void set(Index i1, Index i2, const T& v);
	T linear_at(Index i) const;
	void linear_set(Index i, const T& v);

	bool has(Index i1, Index i2) const;
	bool has(Index i1, Index i2, T& val) const;

	SparseMatrixIterator<T, D1, D2> set(const SparseMatrixIterator<T, D1, D2>& it, const T& val);
	SparseMatrixIterator<T, D1, D2> erase(const SparseMatrixIterator<T, D1, D2>& it);

	// Element wise operations
	SparseMatrix& operator +=(const SparseMatrix& v2);
	SparseMatrix& operator -=(const SparseMatrix& v2);
	SparseMatrix& operator *=(const SparseMatrix& v2);
	SparseMatrix& operator *=(const T& f);

	// Other
	constexpr Dimension columns() const;
	constexpr Dimension rows() const;
	constexpr Dimension size() const;
	Dimension filledCount() const;

	T sum() const;
	T max() const;
	T min() const;
	T avg() const;

	void swap(SparseMatrix& v);

	bool hasNaN() const;
	bool hasInf() const;
	bool hasZero() const;

	bool isEmpty() const;// everything is zero!

	SparseMatrix<T, D2, D1> transpose();

	SparseMatrix<T, D1, D2> conjugate();
	SparseMatrix<T, D2, D1> conjugate_transpose();
	SparseMatrix invert();

	template<Dimension D3>
	SparseMatrix<T, D1, D3> mul(const SparseMatrix<T, D2, D3>& m) const;// Matrix multiplication
};

// Element wise operations
template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator +(const SparseMatrix<T, D1, D2>& v1, const SparseMatrix<T, D1, D2>& v2);
template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator -(const SparseMatrix<T, D1, D2>& v1, const SparseMatrix<T, D1, D2>& v2);
template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator -(const SparseMatrix<T, D1, D2>& v);
template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator *(const SparseMatrix<T, D1, D2 >& v1, const SparseMatrix<T, D1, D2>& v2);
template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator *(const SparseMatrix<T, D1, D2>& v1, T f);
template<typename T, Dimension D1, Dimension D2>
SparseMatrix<T, D1, D2> operator *(T f, const SparseMatrix<T, D1, D2>& v1);

// Comparison
template<typename T, Dimension D1, Dimension D2>
bool operator ==(const SparseMatrix<T, D1, D2>& v1, const SparseMatrix<T, D1, D2>& v2);
template<typename T, Dimension D1, Dimension D2>
bool operator !=(const SparseMatrix<T, D1, D2>& v1, const SparseMatrix<T, D1, D2>& v2);

NS_END_NAMESPACE

#define _NS_SPARSEMATRIX_INL
# include "SparseMatrix.inl"
#undef _NS_SPARSEMATRIX_INL