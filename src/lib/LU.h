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
		template<typename T>
		void doolittle(const Matrix<T>& m, Matrix<T>& L, Matrix<T>& U, Matrix<T>& P, size_t* pivotCount=nullptr);
		template<typename T>
		void doolittle(const SparseMatrix<T>& m, SparseMatrix<T>& L, SparseMatrix<T>& U, SparseMatrix<T>& P, size_t* pivotCount=nullptr);
	}
}

NS_END_NAMESPACE


#define _NS_LU_INL
# include "LU.inl"
#undef _NS_LU_INL