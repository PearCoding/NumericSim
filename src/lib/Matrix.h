#pragma once

#include "Config.h"
#include "Vector.h"

NS_BEGIN_NAMESPACE

/** High dimensional matrix class.
 * Math order: First row, Second column!
 */
template<typename T, Dimension D1, Dimension D2>
class Matrix : public CountableSet<T, D1*D2>
{
public:
	Matrix();
	explicit Matrix(const T& f);
	Matrix(std::initializer_list<std::initializer_list<T> > l);

	virtual ~Matrix();

	// Index
	const T& at(Index i1, Index i2) const;
	void set(Index i1, Index i2, const T& v);

	// Element wise operations
	Matrix& operator +=(const Matrix& v2);
	Matrix& operator -=(const Matrix& v2);
	Matrix& operator *=(const Matrix& v2);
	Matrix& operator *=(const T& f);

	// Other
	constexpr Dimension columns() const;
	constexpr Dimension rows() const;

	Matrix<T, D2, D1> transpose();
	Matrix invert();

	template<Dimension D3>
	Matrix<T, D1, D3> mul(const Matrix<T, D2, D3>& m) const;
};

// Element wise operations
template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator +(const Matrix<T, D1, D2>& v1, const Matrix<T, D1, D2>& v2);
template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator -(const Matrix<T, D1, D2>& v1, const Matrix<T, D1, D2>& v2);
template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator -(const Matrix<T, D1, D2>& v);
template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator *(const Matrix<T, D1, D2 >& v1, const Matrix<T, D1, D2>& v2);
template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator *(const Matrix<T, D1, D2>& v1, T f);
template<typename T, Dimension D1, Dimension D2>
Matrix<T, D1, D2> operator *(T f, const Matrix<T, D1, D2>& v1);

// Comparison
template<typename T, Dimension D1, Dimension D2>
bool operator ==(const Matrix<T, D1, D2>& v1, const Matrix<T, D1, D2>& v2);
template<typename T, Dimension D1, Dimension D2>
bool operator !=(const Matrix<T, D1, D2>& v1, const Matrix<T, D1, D2>& v2);

NS_END_NAMESPACE

#define _NS_MATRIX_INL
# include "Matrix.inl"
#undef _NS_MATRIX_INL