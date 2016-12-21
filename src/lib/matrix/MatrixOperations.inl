#ifndef _NS_MATRIXOPERATIONS_INL
# error MatrixOperations.inl should only be included by MatrixOperations.h
#endif

NS_BEGIN_NAMESPACE

namespace Operations
{
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, T >::type
		determinant(const M<T>& m)
	{
		try
		{
			M<T> L, U, P;
			size_t pivotCount;
			LU::serial::doolittle(m, L, U, P, &pivotCount);
			T det = Math::sign_pow(pivotCount);

			for(Index i = 0; i < U.rows(); ++i)
				det *= U.at(i,i);
			
			return det;
		}
		catch(const MathException&)
		{
			return (T)0;
		}
	}
}

NS_END_NAMESPACE