#ifndef _NS_LU_INL
# error LU.inl should only be included by LU.h
#endif

NS_BEGIN_NAMESPACE
namespace LU {
	namespace serial {
		template<typename T>
		Matrix<T> cholesky(const Matrix<T>& m)
		{
			if (m.rows() != m.columns())
				throw NotSquareException();
			// We assume symmetry!

			Matrix<T> L(m.rows(), m.columns());
			for (Index j = 0; j < L.rows(); ++j)
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
				for (Index i = j; i < L.rows(); ++i)
				{
					s = (T)0;
					for (Index k = 0; k < i; ++k)
						s += L.at(i, k)*L.at(j, k);
					L.set(i, j, (m.at(i, j) - s) / L.at(j, j));
				}
			}

			return L;
		}

		template<typename T>
		Matrix<ComplexNumber<T> > cholesky(const Matrix<ComplexNumber<T> >& m)
		{
			// Question: How cholesky works with not positive definite complex matrices? -> Not unique anymore :)

			if (m.rows() != m.columns())
				throw NotSquareException();

			// We assume symmetry!

			Matrix<ComplexNumber<T> > L;
			for (Index j = 0; j < L.rows(); ++j)
			{
				ComplexNumber<T> s;
				for (Index k = 0; k < j; ++k)
				{
					ComplexNumber<T> v = L.at(j, k);
					s += v * v.conjugate();
				}
				
				L.set(j, j, std::sqrt(m.at(j, j) - s));
				for (Index i = j; i < L.rows(); ++i)
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
		template<typename T>
		SparseMatrix<T> cholesky(const SparseMatrix<T>& m)
		{
			if (m.rows() != m.columns())
				throw NotSquareException();
			// We assume symmetry!

			SparseMatrix<T> L(m.rows(), m.columns());
			for (Index j = 0; j < L.rows(); ++j)// Rows
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
				for (Index i = j; i < L.rows(); ++i)
				{
					s = (T)0;
					for (Index k = 0; k < i; ++k)
						s += L.at(i, k)*L.at(j, k);
					L.set(i, j, (m.at(i, j) - s) / L.at(j, j));
				}
			}

			return L;
		}


		template<typename T>
		SparseMatrix<ComplexNumber<T> > cholesky(const SparseMatrix<ComplexNumber<T> >& m)
		{
			// Question: How cholesky works with not positive definite complex matrices? -> Not unique anymore :)
			if (m.rows() != m.columns())
				throw NotSquareException();

			// We assume symmetry!

			SparseMatrix<ComplexNumber<T> > L(m.rows(), m.columns());
			for (Index j = 0; j < L.rows(); ++j)
			{
				ComplexNumber<T> s;
				for (Index k = 0; k < j; ++k)
				{
					ComplexNumber<T> v = L.at(j, k);
					s += v*v.conjugate();
				}

				L.set(j, j, std::sqrt(m.at(j, j) - s));
				for (Index i = j; i < L.rows(); ++i)
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