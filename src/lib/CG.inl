#ifndef _NS_CG_INL
# error CG.inl should only be included by CG.h
#endif

NS_BEGIN_NAMESPACE
namespace CG {
	namespace serial {
		template<template<typename> class M, typename T>
		typename std::enable_if<is_matrix<M, T>::value, Vector<T> >::type
			cg(const M<T>& a, const Vector<T>& b, const Vector<T>& x0,
				uint32 maxIter, double eps, uint32* it_stat)
		{
			if (a.rows() != a.columns())
				throw NotSquareException();

#ifdef NS_ALLOW_CHECKS
			if (!Check::matrixIsHermitian(a))
				throw NotHermitianException();
#endif

			//TODO: Positive definite?

			const double eps2 = eps*eps;

			Vector<T> x = x0;
			Vector<T> r = b - a.mul(x0);
			Vector<T> p = r;

			T rs = r.magSqr();

			Vector<T> l;
			for (uint32 k = 0; k < maxIter; ++k)
			{
				if (it_stat)
					*it_stat = k;

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

			return x;
		}
	}
}
NS_END_NAMESPACE