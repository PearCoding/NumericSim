#ifndef _NS_MATRIXCONSTRUCTOR_INL
# error MatrixConstructor.inl should only be included by MatrixConstructor.h
#endif

NS_BEGIN_NAMESPACE

namespace Construct
{
	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T,D1,D2> >::type eye()
	{
		M<T,D1,D2> m;
		for (Index i = 0; i < t_min(D1, D2); ++i)
		{
			m.set(i, i, (T)1);
		}

		return m;
	}

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D>
	typename std::enable_if<is_matrix<M, T, D, D>::value, M<T, D, D> >::type
		diag(const Vector<T, D>& v)
	{
		M<T, D, D> m;

		for (Index i = 0; i < D; ++i)
		{
			m.set(i, i, v.at(i));
		}

		return m;
	}

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	Vector<T, t_min(D1,D2)>	diag(const M<T, D1, D2>& m)
	{
		Vector<T, t_min(D1, D2)> v;

		for (Index i = 0; i < t_min(D1, D2); ++i)
		{
			v.set(i, m.at(i, i));
		}

		return v;
	}

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T, D1, D2> >::type
		tril(const M<T, D1, D2>& m, int32 k)
	{
		M<T, D1, D2> r;

		if (k >= 0)
		{
			for (Index i = 0; i < D1; ++i)
				for (Index j = 0; j <= i + k && j < D2; ++j)
					r.set(i, j, m.at(i, j));
		}
		else
		{
			for (Index j = 0; j < D2; ++j)
				for (Index i = j - k; i < D1; ++i)
					r.set(i, j, m.at(i, j));
		}

		return r;
	}

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T, D1, D2> >::type
		triu(const M<T, D1, D2>& m, int32 k)
	{
		M<T, D1, D2> r;

		if (k >= 0)
		{
			for (Index i = 0; i < D1; ++i)
				for (Index j = i + k; j < D2; ++j)
					r.set(i, j, m.at(i, j));
		}
		else
		{
			for (Index j = 0; j < D2; ++j)
				for (Index i = 0; i <= j - k && i < D1; ++i)
					r.set(i, j, m.at(i, j));
		}

		return r;
	}

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T,D1,D2> >::type hilbert()
	{
		M<T,D1,D2> m;
		for (Index i = 0; i < D1; ++i)
		{
			for (Index j = 0; j < D2; ++j)
			{
				double t = 1 / (i + j + 1.0);
				m.set(i, j, (T)t);
			}
		}

		return m;
	}

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T, D1, D2> >::type inv_hilbert()
	{
		static_assert(D1 == D2, "Inverse hilbert matrix must be square.");

		M<T, D1, D2> m;
		for (Index i = 0; i < D1; ++i)
		{
			for (Index j = 0; j < D2; ++j)
			{
				auto v = Math::binom(i + j, i);
				T h = (T)Math::sign_pow(i + j)*static_cast<T>((i + j + 1)
					*Math::binom(D1 + i, D1 - j - 1)*Math::binom(D1 + j, D1 - i - 1)*v*v);
				m.set(i, j, h);
			}
		}
		return m;
	}
}

NS_END_NAMESPACE