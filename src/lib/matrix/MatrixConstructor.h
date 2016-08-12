#pragma once

#include "Types.h"
#include "Utils.h"

NS_BEGIN_NAMESPACE

namespace Construct
{
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		eye(Dimension d1, Dimension d2);

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		diag(const Vector<T>& v);

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, Vector<T> >::type
		diag(const M<T>& v);

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		tril(const M<T>& m, int32 k = 0);

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		triu(const M<T>& m, int32 k = 0);

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		hilbert(Dimension d);

	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		inv_hilbert(Dimension d);
}

NS_END_NAMESPACE

#define _NS_MATRIXCONSTRUCTOR_INL
# include "MatrixConstructor.inl"
#undef _NS_MATRIXCONSTRUCTOR_INL