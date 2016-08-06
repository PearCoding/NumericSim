#pragma once

#include "Types.h"
#include "Exceptions.h"

NS_BEGIN_NAMESPACE

namespace LU
{
	namespace serial
	{
		template<typename T, Dimension D1, Dimension D2>
		Matrix<T, D1, D2> cholesky(const Matrix<T, D1, D2>& m);
		template<typename T, Dimension D1, Dimension D2>//Specialization for ComplexNumber
		Matrix<ComplexNumber<T>, D1, D2> cholesky(const Matrix<ComplexNumber<T>, D1, D2>& m);

		//TODO: Sparse Implementation
	}
}

NS_END_NAMESPACE


#define _NS_LU_INL
# include "LU.inl"
#undef _NS_LU_INL