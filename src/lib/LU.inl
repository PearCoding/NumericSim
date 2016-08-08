#ifndef _NS_LU_INL
# error LU.inl should only be included by LU.h
#endif

NS_BEGIN_NAMESPACE
namespace LU {
	namespace serial {
		template<typename T, Dimension D1, Dimension D2>
		Matrix<T, D1, D2> cholesky(const Matrix<T, D1, D2>& m)
		{
			static_assert(D1 == D2, "Matrix must be square.");
			// We assume symmetry!

			Matrix<T, D1, D2> L;
			for (Index j = 0; j < D1; ++j)
			{
				T s = (T)0;
				for (Index k = 0; k < j; ++k)
				{
					T v = L.at(j, k);
					s += v*v;
				}

				s = m.at(j, j) - s;
				if (s <= (T)0)
					throw NotPositiveDefiniteException();

				L.set(j, j, std::sqrt(s));
				for (Index i = j; i < D1; ++i)
				{
					s = (T)0;
					for (Index k = 0; k < i; ++k)
						s += L.at(i, k)*L.at(j, k);
					L.set(i, j, (m.at(i, j) - s) / L.at(j, j));
				}
			}

			return L;
		}

		template<typename T, Dimension D1, Dimension D2>
		Matrix<ComplexNumber<T>, D1, D2> cholesky(const Matrix<ComplexNumber<T>, D1, D2>& m)
		{
			// Question: How cholesky works with not positive definite complex matrices? -> Not unique anymore :)

			static_assert(D1 == D2, "Matrix must be square.");
			// We assume symmetry!

			Matrix<ComplexNumber<T>, D1, D2> L;
			for (Index j = 0; j < D1; ++j)
			{
				ComplexNumber<T> s;
				for (Index k = 0; k < j; ++k)
				{
					ComplexNumber<T> v = L.at(j, k);
					s += v * v.conjugate();
				}
				
				L.set(j, j, std::sqrt(m.at(j, j) - s));
				for (Index i = j; i < D1; ++i)
				{
					s = (ComplexNumber<T>)0;
					for (Index k = 0; k < i; ++k)
						s += L.at(i, k)*L.at(j, k).conjugate();
					L.set(i, j, (m.at(i, j) - s) / L.at(j, j));
				}
			}

			return L;
		}

		// TODO: Improve for sparse matrices!
		template<typename T, Dimension D1, Dimension D2>
		SparseMatrix<T, D1, D2> cholesky(const SparseMatrix<T, D1, D2>& m)
		{
			static_assert(D1 == D2, "Matrix must be square.");
			// We assume symmetry!

			SparseMatrix<T, D1, D2> L;
			for (Index j = 0; j < D1; ++j)// Rows
			{
				T s = (T)0;
				for (Index k = 0; k < j; ++k)
				{
					T v = L.at(j, k);
					s += v*v;
				}

				s = m.at(j, j) - s;
				if (s <= (T)0)// FIXME: Complex?
					throw NotPositiveDefiniteException();

				L.set(j, j, std::sqrt(s));
				for (Index i = j; i < D1; ++i)
				{
					s = (T)0;
					for (Index k = 0; k < i; ++k)
						s += L.at(i, k)*L.at(j, k);
					L.set(i, j, (m.at(i, j) - s) / L.at(j, j));
				}
			}

			return L;
		}


		template<typename T, Dimension D1, Dimension D2>
		SparseMatrix<ComplexNumber<T>, D1, D2> cholesky(const SparseMatrix<ComplexNumber<T>, D1, D2>& m)
		{
			// Question: How cholesky works with not positive definite complex matrices? -> Not unique anymore :)

			static_assert(D1 == D2, "Matrix must be square.");
			// We assume symmetry!

			SparseMatrix<ComplexNumber<T>, D1, D2> L;
			for (Index j = 0; j < D1; ++j)
			{
				ComplexNumber<T> s;
				for (Index k = 0; k < j; ++k)
				{
					ComplexNumber<T> v = L.at(j, k);
					s += v*v.conjugate();
				}

				L.set(j, j, std::sqrt(m.at(j, j) - s));
				for (Index i = j; i < D1; ++i)
				{
					s = (ComplexNumber<T>)0;
					for (Index k = 0; k < i; ++k)
						s += L.at(i, k)*L.at(j, k).conjugate();
					L.set(i, j, (m.at(i, j) - s) / L.at(j, j));
				}
			}

			return L;
		}
	}
}
NS_END_NAMESPACE