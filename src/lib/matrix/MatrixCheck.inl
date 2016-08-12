#ifndef _NS_MATRIXCHECK_INL
# error MatrixCheck.inl should only be included by MatrixCheck.h
#endif

NS_BEGIN_NAMESPACE

namespace Check
{
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, bool >::type matrixIsOrthogonal(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return (m.mul(m.transpose())) == Construct::eye<M, T>(m.rows(), m.columns());
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, bool >::type matrixIsUnitary(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return (m.mul(m.adjugate())) == Construct::eye<M, T>(m.rows(), m.columns());
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, bool >::type matrixIsProjection(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return m.mul(m) == m;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, bool >::type matrixIsSymmetric(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return m.transpose() == m;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, bool >::type matrixIsHermitian(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return m.adjugate() == m;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, bool >::type matrixIsSkewSymmetric(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return -(m.transpose()) == m;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, bool >::type matrixIsSkewHermitian(const M<T>& m)
	{
		if (m.rows() != m.columns())// Not square
			return false;

		return -(m.adjugate()) == m;
	}
}

NS_END_NAMESPACE