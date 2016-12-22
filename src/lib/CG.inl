#ifndef _NS_CG_INL
# error CG.inl should only be included by CG.h
#endif

NS_BEGIN_NAMESPACE
namespace CG {
	namespace serial {
		template<template<typename> class M, typename T>
		typename std::enable_if<is_matrix<M, T>::value, DynamicVector<T> >::type
			cg(const M<T>& a, const DynamicVector<T>& b, const DynamicVector<T>& x0,
				size_t maxIter, double eps, size_t* it_stat)
		{
			if (a.rows() != a.columns())
				throw NotSquareException();

#ifdef NS_ALLOW_CHECKS
			if (!Check::matrixIsHermitian(a))
				throw NotHermitianException();
#endif

			//TODO: Positive definite?

			const double eps2 = eps*eps;

			DynamicVector<T> x = x0;
			DynamicVector<T> r = b - a.mul(x0);
			DynamicVector<T> p = r;

			T rs = r.magSqr();

			DynamicVector<T> l;
			size_t k = 0;
			for (; k < maxIter; ++k)
			{

				const T ak = rs / (a.mul(p).dot(p));
				x += ak*p;
				l = r - ak*a.mul(p);

				if (l.magSqr() < eps2 || k == maxIter-1)
					break;

				const T ls = l.magSqr();
				const T bk = ls / rs;

				p = l + bk*r;
				r = l;
				rs = ls;
			}

			if (it_stat)
				*it_stat = k + 1;

			return x;
		}
	}
}
NS_END_NAMESPACE