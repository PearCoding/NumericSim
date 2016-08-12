#ifndef _NS_ITERATIVE_INL
# error Iterative.inl should only be included by Iterative.h
#endif

NS_BEGIN_NAMESPACE

namespace Iterative
{
	namespace serial
	{
		template<template<typename> class M, typename T>
		typename std::enable_if<is_matrix<M, T>::value, Vector<T> >::type
			jacobi(const M<T>& a, const Vector<T>& b, const Vector<T>& x0,
				uint32 maxIter, double eps, uint32* it_stat)
		{
			if (a.rows() != a.columns())
				throw NotSquareException();
			if (a.rows() != b.size() || a.rows() != x0.size())
				throw IterativeMatrixVectorMismatchException();

			const double eps2 = eps*eps;

			Vector<T> x = x0;
			Vector<T> xm(a.rows());

			for (uint32 it = 0; it < maxIter; ++it)
			{
				for (Index i = 0; i < a.rows(); ++i)// Could be parallel
				{
					T mid = (T)0;
					T t = (T)0;
					for (auto rit = a.row_begin(i); rit != a.row_end(i); ++rit)
					{
						if (rit.column() != i)
							t += (*rit)*x.at(rit.column());
						else
							mid = *rit;
					}

					xm.set(i, (b.at(i) - t) / mid);
				}

				auto conv = magSqr_vt((xm - x).magSqr());
				if (std::isnan(conv))
					throw MatrixHasZeroInDiagException();

				if (!std::isfinite(conv) ||
					conv <= eps2)// Convergence
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

		template<template<typename> class M, typename T>
		typename std::enable_if<is_matrix<M, T>::value, Vector<T> >::type
			sor(const M<T>& a, const Vector<T>& b, const Vector<T>& x0,
				const T& weight, uint32 maxIter, double eps, uint32* it_stat)
		{
			if (a.rows() != a.columns())
				throw NotSquareException();
			if (a.rows() != b.size() || a.rows() != x0.size())
				throw IterativeMatrixVectorMismatchException();

			const double eps2 = eps*eps;
			const T rweight = (T)1 - weight;

			Vector<T> x = x0;
			Vector<T> xm(a.rows());

			for (uint32 it = 0; it < maxIter; ++it)
			{
				for (Index i = 0; i < a.rows(); ++i)// Could be parallel
				{
					T mid = (T)0;
					T t = (T)0;
					for (auto rit = a.row_begin(i); rit != a.row_end(i); ++rit)
					{
						if (rit.column() < i)
							t += (*rit)*xm.at(rit.column());
						else if (rit.column() > i)
							t += (*rit)*x.at(rit.column());
						else
							mid = *rit;
					}

					t = (b.at(i) - t) / mid;
					xm.set(i, rweight*xm.at(i) + weight*t);
				}

				auto conv = magSqr_vt((xm - x).magSqr());
				if (std::isnan(conv))
					throw MatrixHasZeroInDiagException();

				if (!std::isfinite(conv) ||
					conv <= eps2)// Convergence
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