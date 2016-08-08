#ifndef _NS_MATRIXCONVERTER_INL
# error MatrixConverter.inl should only be included by MatrixConverter.h
#endif

NS_BEGIN_NAMESPACE

namespace Convert
{
	template<typename T, Dimension D1, Dimension D2>
	SparseMatrix<T, D1, D2> toSparseMatrix(const Matrix<T, D1, D2>& m)
	{
		SparseMatrix<T, D1, D2> res;
		for (Index i = 0; i < D1; ++i)
		{
			for (Index j = 0; j < D2; ++j)
			{
				auto v = m.at(i, j);
				if (v != (T)0)
					res.set(i, j, v);
			}
		}

		return res;
	}

	template<typename T, Dimension D1, Dimension D2>
	Matrix<T, D1, D2> toFullMatrix(const SparseMatrix<T, D1, D2>& m)
	{
		Matrix<T, D1, D2> res;
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			res.set(it.row(), it.column(), *it);
		}

		return res;
	}
}

NS_END_NAMESPACE