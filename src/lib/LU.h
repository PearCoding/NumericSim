#pragma once

#include "Types.h"
#include "Exceptions.h"

NS_BEGIN_NAMESPACE

namespace LU
{
	namespace serial
	{
		// Cholesky
		template<typename T, Dimension D1, Dimension D2>
		Matrix<T, D1, D2> cholesky(const Matrix<T, D1, D2>& m);
		template<typename T, Dimension D1, Dimension D2>//Specialization for ComplexNumber
		Matrix<ComplexNumber<T>, D1, D2> cholesky(const Matrix<ComplexNumber<T>, D1, D2>& m);
		template<typename T, Dimension D1, Dimension D2>
		SparseMatrix<T, D1, D2> cholesky(const SparseMatrix<T, D1, D2>& m);
		template<typename T, Dimension D1, Dimension D2>//Specialization for ComplexNumber
		SparseMatrix<ComplexNumber<T>, D1, D2> cholesky(const SparseMatrix<ComplexNumber<T>, D1, D2>& m);

		// Doolittle
		//template<typename T, Dimension D1, Dimension D2>
		//void doolittle(const Matrix<T, D1, D2>& m, Matrix<T, D1, D2>& L, Matrix<T, D1, D2>& U);
	}
}

NS_END_NAMESPACE


#define _NS_LU_INL
# include "LU.inl"
#undef _NS_LU_INL