#pragma once

#include "SparseCountableSet.h"

NS_BEGIN_NAMESPACE

// Does not derive from CountableSet -> Is sparse
template<typename T, Dimension D1, Dimension D2>
class SparseMatrix : public SparseCountableSet<T, D1*D2>
{
public:
	SparseMatrix();
	SparseMatrix(std::initializer_list<std::initializer_list<T> > l);

	virtual ~SparseMatrix();

	// Index
	T at(Index i1, Index i2) const;
	void set(Index i1, Index i2, const T& v);

	// Element wise operations
	SparseMatrix& operator +=(const SparseMatrix& v2);
	SparseMatrix& operator -=(const SparseMatrix& v2);
	SparseMatrix& operator *=(const SparseMatrix& v2);
	SparseMatrix& operator *=(const T& f);

	// Other
	constexpr Dimension columns() const;
	constexpr Dimension rows() const;

	SparseMatrix<T, D2, D1> transpose();
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