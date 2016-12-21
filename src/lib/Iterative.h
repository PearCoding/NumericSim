#pragma once

#include "Types.h"
#include "Utils.h"
#include "Exceptions.h"

NS_BEGIN_NAMESPACE

namespace Iterative
{
	namespace serial
	{
		template<template<typename> class M, typename T>
		typename std::enable_if<is_matrix<M, T>::value, DynamicVector<T> >::type
			jacobi(const M<T>& a, const DynamicVector<T>& b, const DynamicVector<T>& x0,
				uint32 maxIter = 1024, double eps = 10e-6, uint32* it_stat = nullptr);


		template<template<typename> class M, typename T>
		typename std::enable_if<is_matrix<M, T>::value, DynamicVector<T> >::type
			sor(const M<T>& a, const DynamicVector<T>& b, const DynamicVector<T>& x0,
				const T& weight = (T)1, uint32 maxIter = 1024, double eps = 10e-6, uint32* it_stat = nullptr);
	}
}

NS_END_NAMESPACE

#define _NS_ITERATIVE_INL
# include "Iterative.inl"
#undef _NS_ITERATIVE_INL