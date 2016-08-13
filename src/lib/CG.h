#pragma once

#include "Types.h"
#include "Exceptions.h"

#include "matrix/MatrixCheck.h"

NS_BEGIN_NAMESPACE

namespace CG
{
	namespace serial
	{
		template<template<typename> class M, typename T>
		typename std::enable_if<is_matrix<M, T>::value, Vector<T> >::type
			cg(const M<T>& a, const Vector<T>& b, const Vector<T>& x0,
				uint32 maxIter = 1024, double eps = 10e-6, uint32* it_stat = nullptr);
	}
}

NS_END_NAMESPACE


#define _NS_CG_INL
# include "CG.inl"
#undef _NS_CG_INL