#ifndef _NS_MATRIXCONVERTER_INL
# error MatrixConverter.inl should only be included by MatrixConverter.h
#endif

NS_BEGIN_NAMESPACE

namespace Convert
{
	template<typename T>
	SparseMatrix<T> toSparseMatrix(const Matrix<T>& m)
	{
		SparseMatrix<T> res(m.rows(), m.columns());
		for (Index i = 0; i < m.rows(); ++i)
		{
			for (Index j = 0; j < m.columns(); ++j)
			{
				auto v = m.at(i, j);
				if (v != (T)0)
					res.set(i, j, v);
			}
		}

		return res;
	}

	template<typename T>
	Matrix<T> toDenseMatrix(const SparseMatrix<T>& m)
	{
		Matrix<T> res(m.rows(), m.columns());
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			res.set(it.row(), it.column(), *it);
		}

		return res;
	}
}

NS_END_NAMESPACE