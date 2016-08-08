#pragma once

#include "matrix/Matrix.h"
#include "matrix/SparseMatrix.h"

NS_BEGIN_NAMESPACE

namespace Convert
{
	template<typename T, Dimension D1, Dimension D2>
	SparseMatrix<T, D1, D2> toSparseMatrix(const Matrix<T, D1, D2>& m);

	template<typename T, Dimension D1, Dimension D2>
	Matrix<T, D1, D2> toFullMatrix(const SparseMatrix<T, D1, D2>& m);
}

NS_END_NAMESPACE

#define _NS_MATRIXCONVERTER_INL
# include "MatrixConverter.inl"
#undef _NS_MATRIXCONVERTER_INL