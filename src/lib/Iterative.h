#pragma once

#include "Types.h"
#include "Utils.h"
#include "Exceptions.h"

NS_BEGIN_NAMESPACE

namespace Iterative
{
	namespace serial
	{
		template<class M, class V>
		V jacobi(const M& a, const V& b, const V& x0,
				size_t maxIter = 1024, double eps = 10e-6, size_t* it_stat = nullptr);


		template<class M, class V>
		V sor(const M& a, const V& b, const V& x0,
				double weight = 1, size_t maxIter = 1024, double eps = 10e-6, size_t* it_stat = nullptr);
	}
}

NS_END_NAMESPACE

#define _NS_ITERATIVE_INL
# include "Iterative.inl"
#undef _NS_ITERATIVE_INL