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
				const auto t = a.mul(p);
				const typename M::value_type ak = rs / (t.dot(p));
				x += ak*p;
				l = r - ak*t;

				const auto ls = l.magSqr();
				if (ls < eps2 || k == maxIter-1)
					break;

				const typename V2::value_type bk = ls / rs;

				p = l + bk*r;
				r = l;
				rs = ls;
			}

			if (it_stat)
				*it_stat = k + 1;

			return x;
		}

		template<class M, class V1, class V2>
		V1 pcg(const M& a, const V2& b, const M& c, const V1& x0,
				size_t maxIter, double eps, size_t* it_stat)
		{
			if (a.rows() != a.columns())
				throw NotSquareException();

			if (c.rows() != c.columns())
				throw NotSquareException();

			if (a.rows() != c.rows())
				throw MatrixSizeMismatchException();

#ifdef NS_ALLOW_CHECKS
			if (!Check::matrixIsHermitian(a))
				throw NotHermitianException();
#endif

			const double eps2 = eps*eps;

			V1 x = x0;
			V2 r = b - a.mul(x0);
			V2 z = c.mul(r);
			V2 p = z;

			auto l1 = r.dot(z);
			auto l2 = l1;
			size_t k = 0;
			for (; k < maxIter; ++k)
			{
				const auto t = a.mul(p);
				const typename M::value_type ak = l1 / (t.dot(p));
				x += ak*p;
				r -= ak*t;

				if (r.magSqr() < eps2 || k == maxIter-1)
					break;

				z = c.mul(r);
				l2 = r.dot(z);
				p = z + (l2/l1)*p;
				l1 = l2;
			}

			if (it_stat)
				*it_stat = k + 1;

			return x;
		}

		template<class M>
		void jacobi(const M& A, M& C)
		{
			typedef typename M::value_type T;

			if (A.rows() != A.columns())
				throw NotSquareException();

			if (C.rows() != C.columns())
				throw NotSquareException();

			if (A.rows() != C.rows())
				throw MatrixSizeMismatchException();

			for(Index i = 0; i < A.rows(); ++i)
			{
				const auto mid = A.at(i,i);
				if(std::abs(mid) <= std::numeric_limits<typename get_complex_internal<T>::type>::epsilon())
					throw SingularException();
				C.set(i,i, 1/mid);
			}
		}
	}
}
NS_END_NAMESPACE