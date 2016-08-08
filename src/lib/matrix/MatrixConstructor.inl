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
		for (Index i = 0; i < std::min(D1, D2); ++i)
		{
			m.set(i, i, (T)1);
		}

		return m;
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
				T h = Math::sign_pow(i + j)*static_cast<T>((i + j + 1)
					*Math::binom(D1 + i, D1 - j - 1)*Math::binom(D1 + j, D1 - i - 1)*v*v);
				m.set(i, j, h);
			}
		}
		return m;
	}
}

NS_END_NAMESPACE