#pragma once

#include "matrix/Matrix.h"
#include "matrix/SparseMatrix.h"

NS_BEGIN_NAMESPACE

namespace Convert
{
	/**
	* @brief Converts a dense matrix to a sparse matrix.
	* @param m Matrix to convert.
	* @return The new sparse matrix.
	*/
	template<typename T>
	SparseMatrix<T> toSparseMatrix(const Matrix<T>& m);

	/**
	* @brief Converts a sparse matrix to a dense matrix.
	* @param m Sparse matrix to convert.
	* @return The new dense matrix.
	*/
	template<typename T>
	Matrix<T> toDenseMatrix(const SparseMatrix<T>& m);
}

NS_END_NAMESPACE

#define _NS_MATRIXCONVERTER_INL
# include "MatrixConverter.inl"
#undef _NS_MATRIXCONVERTER_INL