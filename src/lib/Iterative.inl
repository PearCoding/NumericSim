#ifndef _NS_ITERATIVE_INL
# error Iterative.inl should only be included by Iterative.h
#endif

NS_BEGIN_NAMESPACE

namespace Iterative
{
	namespace serial
	{
		template<class M, class V>
		V jacobi(const M& a, const V& b, const V& x0,
				size_t maxIter, double eps, size_t* it_stat)
		{
			typedef typename V::value_type T;

			if (a.rows() != a.columns())
				throw NotSquareException();
			if (a.rows() != b.size() || a.rows() != x0.size())
				throw IterativeMatrixVectorMismatchException();

			const double eps2 = eps*eps;

			V x = x0;
			V xm = x0;
			xm.fill((T)0);

			for (size_t it = 0; it < maxIter; ++it)
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

				auto conv = std::abs((xm - x).magSqr());
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

		template<class M, class V>
		V sor(const M& a, const V& b, const V& x0,
				double weight, size_t maxIter, double eps, size_t* it_stat)
		{
			typedef typename V::value_type T;

			if (a.rows() != a.columns())
				throw NotSquareException();
			if (a.rows() != b.size() || a.rows() != x0.size())
				throw IterativeMatrixVectorMismatchException();

			const double eps2 = eps*eps;
			const T rweight = (T)1 - weight;

			V x = x0;
			V xm = x0;
			xm.fill((T)0);

			for (size_t it = 0; it < maxIter; ++it)
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

				auto conv = std::abs((xm - x).magSqr());
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