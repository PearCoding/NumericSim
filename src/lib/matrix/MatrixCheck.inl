#ifndef _NS_MATRIXCHECK_INL
# error MatrixCheck.inl should only be included by MatrixCheck.h
#endif

NS_BEGIN_NAMESPACE

namespace Check
{
	template<template<typename> class M, typename T>
	typename std::enable_if<is_dynamic_matrix<M, T>::value, bool >::type
	matrixIsOrthogonal(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return (m.mul(m.transpose())) == Construct::eye<M, T>(m.rows(), m.columns());
	}

	template<typename T, Dimension K>
	bool matrixIsOrthogonal(const FixedMatrix<T,K,K>& m)
	{
		return (m.mul(m.transpose())) == Construct::eye<T,K,K>();
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_dynamic_matrix<M, T>::value, bool >::type
	matrixIsUnitary(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return (m.mul(m.adjugate())) == Construct::eye<M, T>(m.rows(), m.columns());
	}

	template<typename T, Dimension K>
	bool matrixIsUnitary(const FixedMatrix<T,K,K>& m)
	{
		return (m.mul(m.adjugate())) == Construct::eye<T,K,K>();
	}

	template<class M>
	bool matrixIsProjection(const M& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return m.mul(m) == m;
	}

	template<class M>
	bool matrixIsSymmetric(const M& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return m.transpose() == m;
	}

	template<class M>
	bool matrixIsHermitian(const M& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return m.adjugate() == m;
	}

	template<class M>
	bool matrixIsSkewSymmetric(const M& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return -(m.transpose()) == m;
	}

	template<class M>
	bool matrixIsSkewHermitian(const M& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return -(m.adjugate()) == m;
	}
}

NS_END_NAMESPACE