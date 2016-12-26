#pragma once

#include "BaseMatrix.h"

NS_BEGIN_NAMESPACE

template<typename T>
class DenseMatrix : public BaseMatrix<T, dynamic_container2d_t<T> >
{
public:
	/**
	* @brief Constructs an empty (dense) matrix
	*/
	DenseMatrix();

	/**
	* @brief Constructs an empty (dense) matrix of size(d1,d2)
	* @param d1 Row dimension
	* @param d2 Column dimension
	*/
	DenseMatrix(Dimension d1, Dimension d2);

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
	DenseMatrix(std::initializer_list<std::initializer_list<T> > list);

	virtual ~DenseMatrix();

	/**
	 * @brief Resize dimension of the matrix.
	 * @par Complexity
	 * Unknown
	 * @param d1 New row dimension.
	 * @param d2 New column dimension.
	 * @todo Implement this!
	 */
	void resize(Dimension d1, Dimension d2);
	

	/**
	* @brief Adds entries element wise.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @param m The other matrix.
	* @return A reference to this matrix.
	*/
	template<class DC2>
	DenseMatrix& operator +=(const BaseMatrix<T,DC2>& m);

	/**
	* @brief Subtracts entries element wise.
	* @par Complexity
	* Worst case: \f$ O((D1*D2)^2) \f$
	* @param m The other matrix.
	* @return A reference to this matrix.
	*/
	template<class DC2>
	DenseMatrix& operator -=(const BaseMatrix<T,DC2>& m);

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
	template<class DC2>
	DenseMatrix& operator *=(const BaseMatrix<T,DC2>& m);

	/**
	* @brief Multiplies entries with a scalar.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @param f Scalar.
	* @return A reference to this matrix.
	*/
	DenseMatrix& operator *=(const T& f);

	/**
	* @brief Swaps the entries of the matrices.
	* @par Complexity
	* Always: \f$ O(1) \f$
	* @param m The other matrix.
	*/
	void swap(DenseMatrix& m);

	/**
	* @brief Transpose matrix.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @note When using complex matrices most of the time you need the adjugate() operation,\n
	* as it is the most reasonable one in most of the cases. (Consider looking at your script)
	* @return Transpose of the matrix \f$ A^T \f$
	* @sa adjugate()
	*/
	DenseMatrix transpose() const;

	/**
	* @brief The conjugate of all entries if complex.
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return Transpose of the matrix \f$ A^T \f$
	*/
	DenseMatrix conjugate() const;

	/**
	* @brief The adjugate / conjugate transpose of the matrix.
	* @note If the matrix has no complex entries, it is the same as transpose().
	* @par Complexity
	* Worst case: \f$ O(D1*D2) \f$
	* @return Conjugate transpose of the matrix \f$ A^* \f$
	* @sa transpose()
	*/
	DenseMatrix adjugate() const;

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
	DenseMatrix mul(const DenseMatrix& right) const;

	/**
	* @brief Right side matrix vector multiplication.
	* @par Complexity
	* Worst case: UNKNOWN
	* @param right A vector with the same size as the column count of the matrix.
	* @return A vector with the same size as the row count.
	* @todo Complexity?
	*/
	template<class DC2>
	DynamicVector<T> mul(const Vector<T,DC2>& right) const;

	/**
	* @brief Left side matrix vector multiplication.
	* @par Complexity
	* Worst case: UNKNOWN
	* @param left A vector with the same size as the row count of the matrix.
	* @return A vector with the same size as the column count.
	* @todo Complexity?
	*/
	template<class DC2>
	DynamicVector<T> mul_left(const Vector<T,DC2>& left) const;
};

// Element wise operations
template<typename T>
DenseMatrix<T> operator +(const DenseMatrix<T>& v1, const DenseMatrix<T>& v2);
template<typename T>
DenseMatrix<T> operator -(const DenseMatrix<T>& v1, const DenseMatrix<T>& v2);
template<typename T>
DenseMatrix<T> operator -(const DenseMatrix<T>& v);
template<typename T>
DenseMatrix<T> operator *(const DenseMatrix<T>& v1, const DenseMatrix<T>& v2);
template<typename T>
DenseMatrix<T> operator *(const DenseMatrix<T>& v1, T f);
template<typename T>
DenseMatrix<T> operator *(T f, const DenseMatrix<T>& v1);

NS_END_NAMESPACE

#define _NS_DENSEMATRIX_INL
# include "DenseMatrix.inl"
#undef _NS_DENSEMATRIX_INL