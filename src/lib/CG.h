#pragma once

#include "Types.h"
#include "Exceptions.h"

#include "matrix/MatrixCheck.h"

NS_BEGIN_NAMESPACE

namespace CG
{
	namespace serial
	{
		template<class M, class V1, class V2>
		V1 cg(const M& a, const V2& b, const V1& x0,
				size_t maxIter = 1024, double eps = 10e-6, size_t* it_stat = nullptr);

		template<class M, class V1, class V2>
		V1 pcg(const M& a, const V2& b, const M& c, const V1& x0,
				size_t maxIter = 1024, double eps = 10e-6, size_t* it_stat = nullptr);
	}
}

NS_END_NAMESPACE


#define _NS_CG_INL
# include "CG.inl"
#undef _NS_CG_INL