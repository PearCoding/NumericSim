#pragma once

#include "Types.h"
#include "Utils.h"

NS_BEGIN_NAMESPACE

namespace Construct
{
	template<template<typename,Dimension,Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T,D1,D2> >::type
		eye();

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D>
	typename std::enable_if<is_matrix<M, T, D, D>::value, M<T, D, D> >::type
		diag(const Vector<T, D>& v);

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	Vector<T, t_min(D1,D2)> diag(const M<T, D1, D2>& v);

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T, D1, D2> >::type
		tril(const M<T, D1, D2>& m, int32 k = 0);

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T, D1, D2> >::type
		triu(const M<T, D1, D2>& m, int32 k = 0);

	// Real hilbert is only defined for square matrices, but we allow general aswell.
	template<template<typename,Dimension,Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T,D1,D2> >::type
		hilbert();

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T, D1, D2> >::type
		inv_hilbert();
}

NS_END_NAMESPACE

#define _NS_MATRIXCONSTRUCTOR_INL
# include "MatrixConstructor.inl"
#undef _NS_MATRIXCONSTRUCTOR_INL