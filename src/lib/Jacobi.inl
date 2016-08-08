#ifndef _NS_JACOBI_INL
# error Jacobi.inl should only be included by Jacobi.h
#endif

NS_BEGIN_NAMESPACE

namespace Iterative
{
	namespace serial
	{
		template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
		typename std::enable_if<is_matrix<M, T, D1, D2>::value, Vector<T, D1> >::type
			jacobi(const M<T, D1, D2>& a, const Vector<T, D1>& b, const Vector<T, D1>& x0,
				uint32 maxIter, double eps, uint32* it_stat)
		{
			static_assert(D1 == D2, "To use the Jacobi method a square matrix is needed.");

			Vector<T, D1> x = x0;
			Vector<T, D1> xm;

			for (uint32 it = 0; it < maxIter; ++it)
			{
				for (Index i = 0; i < D1; ++i)
				{
					T t = (T)0;
					for (Index j = 0; j < D1; ++j)
					{
						if (j != i)
							t += a.at(i, j)*x.at(j);
					}

					xm.set(i, (b.at(i) - t) / a.at(i, i));
				}

				auto conv = std::abs((xm - x).magSqr());
				if (/*!std::isfinite(conv) ||*/
					conv <= eps)// Convergence
				{
					if (it_stat)
						*it_stat = it + 1;

					return xm;
				}
				else
				{
					x = xm;
				}
			}

			if (it_stat)
				*it_stat = maxIter;

			return x;
		}
	}
}

NS_END_NAMESPACE