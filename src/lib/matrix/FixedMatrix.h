#pragma once

#include "BaseMatrix.h"

NS_BEGIN_NAMESPACE

template<typename T, Dimension K1, Dimension K2>
class FixedMatrix : public BaseMatrix<T, fixed_container2d_t<T,K1,K2> >
{
public:
	/**
	* @brief Constructs an empty (dense) matrix
	*/
	FixedMatrix();

	/**
	* @brief Constructs a sparse matrix from the two dimensional initializer list.
	* @details The initializer list size dynamically sets the dimension of the matrix.
	*
	* @par Example
	* `Matrix<float> A = { {1,0,1},{0,1,0},{2,0,2} }` will generate the (dense) matrix
	* `[1,0,1; 0,1,0; 2,0,2]`\n
	* Here every semicolon starts a new row.
	*
	* @note Every missing entry will be initialized with 0.
	*
	* @param list A two dimensional initializer list.
	* @throw MatrixInitializerListFailedException
	*/
	FixedMatrix(std::initializer_list<std::initializer_list<T> > list);

	virtual ~FixedMatrix();	

	/**
	* @brief Adds entries element wise.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @param m The other matrix.
	* @return A reference to this matrix.
	*/
	FixedMatrix& operator +=(const FixedMatrix& m);

	/**
	* @brief Subtracts entries element wise.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @param m The other matrix.
	* @return A reference to this matrix.
	*/
	FixedMatrix& operator -=(const FixedMatrix& m);

	/**
	* @brief Multiplies entries element wise.
	* @note Don't mistake this with real matrix matrix multiplication.\n
	* Use mul() instead for matrix matrix multiplication.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @param m The other matrix.
	* @return A reference to this matrix.
	* @todo Is there a better approach?
	*/
	FixedMatrix& operator *=(const FixedMatrix& m);

	/**
	* @brief Multiplies entries with a scalar.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @param f Scalar.
	* @return A reference to this matrix.
	*/
	FixedMatrix& operator *=(const T& f);

	/**
	* @brief Swaps the entries of the matrices.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param m The other matrix.
	*/
	void swap(FixedMatrix& m);

	/**
	* @brief Transpose matrix.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @note When using complex matrices most of the time you need the adjugate() operation,\n
	* as it is the most reasonable one in most of the cases. (Consider looking at your script)
	* @return Transpose of the matrix \f$ A^T \f$
	* @sa adjugate()
	*/
	FixedMatrix<T,K2,K1> transpose() const;

	/**
	* @brief The conjugate of all entries if complex.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return Transpose of the matrix \f$ A^T \f$
	*/
	FixedMatrix<T,K1,K2> conjugate() const;

	/**
	* @brief The adjugate / conjugate transpose of the matrix.
	* @note If the matrix has no complex entries, it is the same as transpose().
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return Conjugate transpose of the matrix \f$ A^* \f$
	* @sa transpose()
	*/
	FixedMatrix<T,K2,K1> adjugate() const;

	/**
	* @brief Right side matrix matrix multiplication.
	* @details The matrix matrix multiplication is defined as:\n
	* \f[
	* A.mul(B) := A \cdot B \textrm{ with } A \in T^{D1 \times D2} \times B \in T^{D2 \times D3} \to C \in T^{D1 \times D3}
	* \f]
	* @par Complexity
	* Worst case: UNKNOWN
	* @param right The other matrix, which row count must match the column count of this matrix.
	* @return The result of the matrix multiplication.
	* @todo Complexity?
	*/
	template<Dimension K3>
	FixedMatrix<T,K1,K3> mul(const FixedMatrix<T,K2,K3>& right) const;

	/**
	* @brief Right side matrix vector multiplication.
	* @par Complexity
	* Worst case: UNKNOWN
	* @param right A vector with the same size as the column count of the matrix.
	* @return A vector with the same size as the row count.
	* @todo Complexity?
	*/
	FixedVector<T,K1> mul(const FixedVector<T,K2>& right) const;

	/**
	* @brief Left side matrix vector multiplication.
	* @par Complexity
	* Worst case: UNKNOWN
	* @param left A vector with the same size as the row count of the matrix.
	* @return A vector with the same size as the column count.
	* @todo Complexity?
	*/
	FixedVector<T,K2> mul_left(const FixedVector<T,K1>& left) const;
};

// Element wise operations
template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator +(const FixedMatrix<T,K1,K2>& v1, const FixedMatrix<T,K1,K2>& v2);
template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator -(const FixedMatrix<T,K1,K2>& v1, const FixedMatrix<T,K1,K2>& v2);
template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator -(const FixedMatrix<T,K1,K2>& v);
template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator *(const FixedMatrix<T,K1,K2>& v1, const FixedMatrix<T,K1,K2>& v2);
template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator *(const FixedMatrix<T,K1,K2>& v1, T f);
template<typename T, Dimension K1, Dimension K2>
FixedMatrix<T,K1,K2> operator *(T f, const FixedMatrix<T,K1,K2>& v1);

NS_END_NAMESPACE

#define _NS_FIXEDMATRIX_INL
# include "FixedMatrix.inl"
#undef _NS_FIXEDMATRIX_INL