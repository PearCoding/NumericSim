#ifndef _NS_CG_INL
# error CG.inl should only be included by CG.h
#endif

NS_BEGIN_NAMESPACE
namespace CG {
	namespace serial {
		template<class M, class V1, class V2>
		V1 cg(const M& a, const V2& b, const V1& x0,
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

			V1 x = x0;
			V2 r = b - a.mul(x0);
			V2 p = r;

			typename V2::value_type rs = r.magSqr();

			V2 l;
			size_t k = 0;
			for (; k < maxIter; ++k)
			{
				const typename M::value_type ak = rs / (a.mul(p).dot(p));
				x += ak*p;
				l = r - ak*a.mul(p);

				if (l.magSqr() < eps2 || k == maxIter-1)
					break;

				const typename V2::value_type ls = l.magSqr();
				const typename V2::value_type bk = ls / rs;

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