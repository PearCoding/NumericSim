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

		template<typename T>
		void ilu0(const SparseMatrix<T>& A, SparseMatrix<T>& L, SparseMatrix<T>& U)
		{
			if (A.rows() != A.columns())
				throw NotSquareException();

			SparseMatrix<T> M = A;
			// Calculate
			for(Index i = 1; i < M.rows(); ++i)
			{
				for(auto it = M.row_begin(i); it != M.row_end(i) && it.column() < i; ++it)//a_ik
				{
					const auto mid = M.at(it.column(), it.column());
					if(std::abs(mid) <= std::numeric_limits<typename get_complex_internal<T>::type>::epsilon())
						throw SingularException();

					const auto d = *it/mid;
					*it = d;
					
					auto it2 = it;
					it2++;
					for(; it2 != M.row_end(i); ++it2)//a_ij				
						*it2 -= d * M.at(it.column(), it2.column());
				}
			}

			// Seperate M into L and U
			for(auto it = M.begin(); it != M.end(); ++it)
			{
				if(it.column() == it.row())
				{
					L.set(it.row(), it.column(), 1);
					U.set(it.row(), it.column(), *it);
				}
				else if(it.column() < it.row())
				{
					L.set(it.row(), it.column(), *it);
				}
				else
				{
					U.set(it.row(), it.column(), *it);
				}
			}
		}

		template<class M, class V>
		V solve_lu(const M& L, const M& U, const V& b)
		{
			if (L.rows() != L.columns())
				throw NotSquareException();

			if (U.rows() != U.columns())
				throw NotSquareException();
				
			if (L.rows() != U.rows())
				throw MatrixSizeMismatchException();
				
			if (L.rows() != b.size())
				throw MatrixVectorMismatchException();

			// Forward
			V y = b;
			for(Index i = 0; i < L.rows(); ++i)
			{
				for(auto it = L.row_begin(i); it != L.row_end(i) && it.column() < i; ++it)
					y[i] -= (*it) * y[it.column()];
				
				const auto mid = L.at(i,i);
				if(std::abs(mid) <= std::numeric_limits<typename get_complex_internal<typename V::value_type>::type>::epsilon())
					throw SingularException();
				y[i] /= mid;
			}

			// Backward
			V x = y;
			for(Index i = U.rows()-1; ; --i)
			{
				for(auto it = U.row_begin(i); it != U.row_end(i); ++it)
				{
					if(it.column() < i+1)
						continue;

					x[i] -= (*it) * x[it.column()];
				}
				
				const auto mid = U.at(i,i);
				if(std::abs(mid) <= std::numeric_limits<typename get_complex_internal<typename V::value_type>::type>::epsilon())
					throw SingularException();
				x[i] /= mid;

				if(i == 0)
					break;
			}

			return x;
		}

		template<class M>
		void ainv_lu(const M& L, const M& U, M& invL, M& invU)
		{
			typedef typename M::value_type T;

			if (L.rows() != L.columns())
				throw NotSquareException();

			if (U.rows() != U.columns())
				throw NotSquareException();

			if (invL.rows() != invL.columns())
				throw NotSquareException();

			if (invU.rows() != invU.columns())
				throw NotSquareException();
				
			if (L.rows() != U.rows())
				throw MatrixSizeMismatchException();
				
			if (L.rows() != invL.rows())
				throw MatrixSizeMismatchException();
				
			if (L.rows() != invU.rows())
				throw MatrixSizeMismatchException();

			const Dimension N = L.columns();

			for(Index k = 0; k < N; ++k)
			{
				// L
				invL.set(k,k, 1);// Assuming L has only ones as diagonal entries

				for(Index i = k+1; i < N; ++i)
				{
					T m = invL.at(i,k);

					auto it = L.row_begin(i);
					while(it != L.row_end(i) && it.column() < k)// Skipping. Is zero
						++it;

					for(; it != L.row_end(i) && it.column() < i; ++it)
						m -= (*it) * invL.at(it.column(),k);

					invL.set(i,k,m);
				}

				// U
				invU.set(k,k, 1/U.at(k,k));
				
				for(Index i = (N-1)-k; ; --i)
				{
					T m = invU.at(i,k);

					auto it = U.row_begin(i);
					while(it != U.row_end(i) && it.column() < i+1)// Skipping. Is zero
						++it;

					for(;it != U.row_end(i) && it.column() < k+1; ++it)
						m -= (*it) * invU.at(it.column(),k);

					const auto mid = U.at(i,i);
					if(std::abs(mid) <= std::numeric_limits<typename get_complex_internal<T>::type>::epsilon())
						throw SingularException();

					invU.set(i,k, m/mid);

					if(i == 0)
						break;
				}
			}
		}
	}
}
NS_END_NAMESPACE