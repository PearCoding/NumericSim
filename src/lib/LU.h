#pragma once

#include "Types.h"
#include "Exceptions.h"

#include "matrix/MatrixCheck.h"

NS_BEGIN_NAMESPACE

namespace LU
{
	namespace serial
	{
		// Cholesky
		template<typename T>
		Matrix<T> cholesky(const Matrix<T>& m);
		template<typename T>//Specialization for ComplexNumber
		Matrix<ComplexNumber<T> > cholesky(const Matrix<ComplexNumber<T> >& m);
		template<typename T>
		SparseMatrix<T> cholesky(const SparseMatrix<T>& m);
		template<typename T>//Specialization for ComplexNumber
		SparseMatrix<ComplexNumber<T> > cholesky(const SparseMatrix<ComplexNumber<T> >& m);

		// Doolittle
		//template<typename T, Dimension D1, Dimension D2>
		//void doolittle(const Matrix<T, D1, D2>& m, Matrix<T, D1, D2>& L, Matrix<T, D1, D2>& U);
	}
}

NS_END_NAMESPACE


#define _NS_LU_INL
# include "LU.inl"
#undef _NS_LU_INL