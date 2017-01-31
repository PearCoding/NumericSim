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
		template<class M>
		void cholesky(const M& m, M& L);

		// Doolittle
		template<typename T, class DC>
		void doolittle(const BaseMatrix<T,DC>& m, BaseMatrix<T,DC>& L, BaseMatrix<T,DC>& U, BaseMatrix<T,DC>& P, size_t* pivotCount=nullptr);
		template<typename T>
		void doolittle(const SparseMatrix<T>& m, SparseMatrix<T>& L, SparseMatrix<T>& U, SparseMatrix<T>& P, size_t* pivotCount=nullptr);

		template<typename T>
		void ilu0(const SparseMatrix<T>& A, SparseMatrix<T>& L, SparseMatrix<T>& U);

		template<class M, class V>
		V solve_lu(const M& L, const M& U, const V& b);
	}
}

NS_END_NAMESPACE


#define _NS_LU_INL
# include "LU.inl"
#undef _NS_LU_INL