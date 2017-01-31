#ifndef _NS_MATRIXOPERATIONS_INL
# error MatrixOperations.inl should only be included by MatrixOperations.h
#endif

NS_BEGIN_NAMESPACE

namespace Operations
{
	template<template<typename> class M, typename T>
	T determinant(const M<T>& m)
	{
		try
		{
			M<T> L(m.rows(), m.columns()), U(m.rows(), m.columns()), P(m.rows(), m.columns());
			size_t pivotCount;
			LU::serial::doolittle(m, L, U, P, &pivotCount);
			T det = (T)Math::sign_pow(pivotCount);

			for(Index i = 0; i < U.rows(); ++i)
				det *= U.at(i,i);
			
			return det;
		}
		catch(const MathException&)
		{
			return (T)0;
		}
	}

	template<typename T, Dimension K1, Dimension K2>
	T determinant(const FixedMatrix<T, K1, K2>& m)
	{
		try
		{
			FixedMatrix<T, K1, K2> L, U, P;
			size_t pivotCount;
			LU::serial::doolittle(m, L, U, P, &pivotCount);
			T det = (T)Math::sign_pow(pivotCount);

			for(Index i = 0; i < U.rows(); ++i)
				det *= U.at(i,i);
			
			return det;
		}
		catch(const MathException&)
		{
			return (T)0;
		}
	}

	template<class M>
	M inverse(const M& m)
	{
		M L = m;
		M U = m;
		M P = m;
		size_t pivotCount;
		LU::serial::doolittle(m, L, U, P, &pivotCount);
		
		return inverse(L, U, P);
	}

	template<class M>
	M inverse(const M& L, const M& U)
	{
		typedef typename M::value_type T;

		if (L.rows() != L.columns())
			throw NotSquareException();

		if (U.rows() != U.columns())
			throw NotSquareException();
			
		if (L.rows() != U.rows())
			throw MatrixSizeMismatchException();

		DynamicVector<T> b(L.rows());
		DynamicVector<T> x(L.rows());

		M Inv = L;
		for(Index i = 0; i < L.columns(); ++i)
		{
			b[i] = 1;
			x = LU::serial::solve_lu(L,U,b);

			for(Index j = 0; j < L.rows(); ++j)
				Inv.set(j,i,x[j]);

			b[i] = 0;
		}
		
		return Inv;
	}

	template<class M>
	M inverse(const M& L, const M& U, const M& P)
	{
		typedef typename M::value_type T;

		if (L.rows() != L.columns())
			throw NotSquareException();

		if (U.rows() != U.columns())
			throw NotSquareException();

		if (P.rows() != P.columns())
			throw NotSquareException();
			
		if (L.rows() != U.rows())
			throw MatrixSizeMismatchException();
			
		if (P.rows() != U.rows())
			throw MatrixSizeMismatchException();

		DynamicVector<T> b(L.rows());
		DynamicVector<T> x(L.rows());

		M Inv = L;
		for(Index i = 0; i < L.columns(); ++i)
		{
			b[i] = 1;
			x = LU::serial::solve_lu(L,U,P.mul(b));

			for(Index j = 0; j < L.rows(); ++j)
				Inv.set(j,i,x[j]);

			b[i] = 0;
		}
		
		return Inv;
	}

	template<class M>
	typename M::value_type max_norm2(const M& m)
	{
		typedef typename M::value_type T;

		T v = (T)0;
		for(Index i = 0; i < m.rows(); ++i)
		{
			T s = (T)0;
			for(auto rIt = m.row_begin(i); rIt != m.row_end(i); ++rIt)
			{
				if(std::abs(*rIt)>s)
					s = std::abs(*rIt);
			}

			v += s*s;
		}

		return std::sqrt(v);
	}

	template<class M>
	typename M::value_type min_norm2(const M& m)
	{
		typedef typename M::value_type T;

		T v = (T)0;
		for(Index i = 0; i < m.rows(); ++i)
		{
			T s = (T)std::numeric_limits<T>::max();
			for(auto rIt = m.row_begin(i); rIt != m.row_end(i); ++rIt)
			{
				if(std::abs(*rIt)<s)
					s = std::abs(*rIt);
			}

			v += s*s;
		}

		return std::sqrt(v);
	}

	template<class M>
	typename M::value_type cond(const M& m)
	{
		return max_norm2(m)/min_norm2(m);
	}
}

NS_END_NAMESPACE