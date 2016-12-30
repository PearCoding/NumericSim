#ifndef _NS_LU_INL
# error LU.inl should only be included by LU.h
#endif

NS_BEGIN_NAMESPACE
namespace LU {
	namespace serial {
		template<class M>
		void cholesky(const M& m, M& L)
		{
			if (m.rows() != m.columns())
				throw NotSquareException();

			if (m.rows() != L.rows() || m.columns() != L.columns())
				throw InvalidOutputMatrixException();

#ifdef NS_ALLOW_CHECKS
			if (!Check::matrixIsHermitian(m))
				throw NotHermitianException();
#endif

			typedef typename M::value_type T;

			for (Index j = 0; j < L.rows(); ++j)
			{
				T s = (T)0;
				for (Index k = 0; k < j; ++k)
				{
					T v = L.at(j, k);
					s += v*v;
				}

				s = m.at(j, j) - s;
				if (!sqrt_allowed(s))
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
		}

		//---------------------------------------------------------------------
		template<typename T, class DC>
		void doolittle(const BaseMatrix<T,DC>& A, BaseMatrix<T,DC>& L, BaseMatrix<T,DC>& U, BaseMatrix<T,DC>& P, size_t* pivotCount)
		{
			if (A.rows() != A.columns())
				throw NotSquareException();

			if (A.rows() != L.rows() || A.columns() != L.columns() ||
				A.rows() != P.rows() || A.columns() != P.columns())
				throw InvalidOutputMatrixException();

			const size_t n = A.rows();

			// Setup matrices
			U = A;
			if(pivotCount)
				*pivotCount = 0;

			Index* rowTable = new Index[n];
			for (Index i = 0; i < n; ++i)
			{
				L.set(i, i, (T)1);
				rowTable[i] = i;
			}
			
			// Start
			for(Index k = 0; k < n-1; ++k)
			{
				// Find max i for pivoting
				Index i = k;
				auto m = std::abs(U.at(i,k));
				for(Index j = k+1; j < n; ++j)
				{
					const auto v = std::abs(U.at(j,k));
					if(v >= m)
					{
						m = v;
						i = j;
					}
				}
				
				// Reorder if necessary
				if(i != k)
				{
					if(pivotCount)
						(*pivotCount)++;
					
					for(Index j = k; j < n; ++j)// Swap upper part
					{
						const T tmp = U.at(k,j);
						U.set(k,j,U.at(i,j));
						U.set(i,j,tmp);
					}
					
					for(Index j = 0; j < k; ++j)// Swap lower part
					{
						const T tmp = L.at(k,j);
						L.set(k,j,L.at(i,j));
						L.set(i,j,tmp);
					}

					std::swap(rowTable[i], rowTable[k]);
				}

				// Actual calculating
				if(std::abs(U.at(k,k)) <=
					std::numeric_limits<typename get_complex_internal<T>::type>::epsilon())
					throw SingularException();

				const T invMid = (T)1/U.at(k,k);
				for(Index j = k+1; j < n; ++j)
				{
					L.set(j,k, U.at(j,k)*invMid);

					for(Index t = k; t < n; ++t)
						U.set(j,t, U.at(j,t) - L.at(j,k)*U.at(k,t));
				}
			}

			// Build pivot matrix
			for (Index i = 0; i < n; ++i)
				P.set(i,rowTable[i], (T)1);
			
			delete[] rowTable;
		}

		/* TODO: Optimize*/
		template<typename T>
		void doolittle(const SparseMatrix<T>& A, SparseMatrix<T>& L, SparseMatrix<T>& U, SparseMatrix<T>& P, size_t* pivotCount)
		{
			if (A.rows() != A.columns())
				throw NotSquareException();

			const size_t n = A.rows();

			// Setup matrices
			U = A;
			L = SparseMatrix<T>(n,n);
			if(pivotCount)
				*pivotCount = 0;

			Index* rowTable = new Index[n];
			for (Index i = 0; i < n; ++i)
			{
				L.set(i, i, (T)1);
				rowTable[i] = i;
			}
			
			// Start
			for(Index k = 0; k < n-1; ++k)
			{
				// Find max i for pivoting
				Index i = k;
				auto m = std::abs(U.at(i,k));
				for(Index j = k+1; j < n; ++j)
				{
					const auto v = std::abs(U.at(j,k));
					if(v >= m)
					{
						m = v;
						i = j;
					}
				}
				
				// Reorder if necessary
				if(i != k)
				{
					if(pivotCount)
						(*pivotCount)++;
					
					for(Index j = k; j < n; ++j)// Swap upper part
					{
						const T tmp = U.at(k,j);
						U.set(k,j,U.at(i,j));
						U.set(i,j,tmp);
					}
					
					for(Index j = 0; j < k; ++j)// Swap lower part
					{
						const T tmp = L.at(k,j);
						L.set(k,j,L.at(i,j));
						L.set(i,j,tmp);
					}

					std::swap(rowTable[i], rowTable[k]);
				}

				// Actual calculating
				if(std::abs(U.at(k,k)) <=
					std::numeric_limits<typename get_complex_internal<T>::type>::epsilon())
					throw SingularException();

				const T invMid = (T)1/U.at(k,k);
				for(Index j = k+1; j < n; ++j)
				{
					L.set(j,k, U.at(j,k)*invMid);

					for(Index t = k; t < n; ++t)
						U.set(j,t, U.at(j,t) - L.at(j,k)*U.at(k,t));
				}
			}

			// Build pivot matrix
			P = SparseMatrix<T>(n,n);
			for (Index i = 0; i < n; ++i)
				P.set(i,rowTable[i], (T)1);
			
			delete[] rowTable;
		}
	}
}
NS_END_NAMESPACE