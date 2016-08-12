#ifndef _NS_MATRIXCONSTRUCTOR_INL
# error MatrixConstructor.inl should only be included by MatrixConstructor.h
#endif

NS_BEGIN_NAMESPACE

namespace Construct
{
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type eye(Dimension d1, Dimension d2)
	{
		M<T> m(d1, d2);
		for (Index i = 0; i < std::min(d1, d2); ++i)
		{
			m.set(i, i, (T)1);
		}

		return m;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		diag(const Vector<T>& v)
	{
		M<T> m(v.size(), v.size());

		for (Index i = 0; i < v.size(); ++i)
		{
			m.set(i, i, v.at(i));
		}

		return m;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, Vector<T> >::type
		diag(const M<T>& m)
	{
		Vector<T> v(std::min(m.rows(), m.columns()));

		for (Index i = 0; i < std::min(m.rows(), m.columns()); ++i)
		{
			v.set(i, m.at(i, i));
		}

		return v;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		tril(const M<T>& m, int32 k)
	{
		M<T> r(m.rows(), m.columns());

		if (k >= 0)
		{
			for (Index i = 0; i < m.rows(); ++i)
				for (Index j = 0; j <= i + k && j < m.columns(); ++j)
					r.set(i, j, m.at(i, j));
		}
		else
		{
			for (Index j = 0; j < m.columns(); ++j)
				for (Index i = j - k; i < m.rows(); ++i)
					r.set(i, j, m.at(i, j));
		}

		return r;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		triu(const M<T>& m, int32 k)
	{
		M<T> r(m.rows(), m.columns());

		if (k >= 0)
		{
			for (Index i = 0; i < m.rows(); ++i)
				for (Index j = i + k; j < m.columns(); ++j)
					r.set(i, j, m.at(i, j));
		}
		else
		{
			for (Index j = 0; j < m.columns(); ++j)
				for (Index i = 0; i <= j - k && i < m.rows(); ++i)
					r.set(i, j, m.at(i, j));
		}

		return r;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type hilbert(Dimension d)
	{
		M<T> m(d, d);
		for (Index i = 0; i < d; ++i)
		{
			for (Index j = 0; j < d; ++j)
			{
				double t = 1 / (i + j + 1.0);
				m.set(i, j, (T)t);
			}
		}

		return m;
	}

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type inv_hilbert(Dimension d)
	{
		M<T> m(d, d);
		for (Index i = 0; i < d; ++i)
		{
			for (Index j = 0; j < d; ++j)
			{
				auto v = Math::binom(i + j, i);
				T h = (T)Math::sign_pow(i + j)*static_cast<T>((i + j + 1)
					*Math::binom(d + i, d - j - 1)*Math::binom(d + j, d - i - 1)*v*v);
				m.set(i, j, h);
			}
		}
		return m;
	}
}

NS_END_NAMESPACE