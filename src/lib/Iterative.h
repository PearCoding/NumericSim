#pragma once

#include "Types.h"
#include "Utils.h"
#include "Exceptions.h"

NS_BEGIN_NAMESPACE

namespace Iterative
{
	namespace serial
	{
		template<template<typename, Dimension, Dimension> class M, typename T, Dimension D1, Dimension D2>
		typename std::enable_if<is_matrix<M, T, D1, D2>::value, Vector<T, D1> >::type
			jacobi(const M<T, D1, D2>& a, const Vector<T, D1>& b, const Vector<T, D1>& x0,
				uint32 maxIter = 1024, double eps = 10e-6, uint32* it_stat = nullptr);


		template<template<typename, Dimension, Dimension> class M, typename T, Dimension D1, Dimension D2>
		typename std::enable_if<is_matrix<M, T, D1, D2>::value, Vector<T, D1> >::type
			sor(const M<T, D1, D2>& a, const Vector<T, D1>& b, const Vector<T, D1>& x0,
				const T& weight = (T)1, uint32 maxIter = 1024, double eps = 10e-6, uint32* it_stat = nullptr);
	}
}

NS_END_NAMESPACE

#define _NS_ITERATIVE_INL
# include "Iterative.inl"
#undef _NS_ITERATIVE_INL