#pragma once

#include "matrix/Matrix.h"
#include "matrix/SparseMatrix.h"
#include "Types.h"
#include "Utils.h"

NS_BEGIN_NAMESPACE

namespace Construct
{
	template<template<typename,Dimension,Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T,D1,D2> >::type eye();

	// Real hilbert is only defined for square matrices, but we allow general aswell.
	template<template<typename,Dimension,Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T,D1,D2> >::type hilbert();

	template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
	typename std::enable_if<is_matrix<M, T, D1, D2>::value, M<T, D1, D2> >::type inv_hilbert();
}

NS_END_NAMESPACE

#define _NS_MATRIXCONSTRUCTOR_INL
# include "MatrixConstructor.inl"
#undef _NS_MATRIXCONSTRUCTOR_INL