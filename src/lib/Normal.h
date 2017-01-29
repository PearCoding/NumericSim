#pragma once

#include "Vector.h"
#include "matrix/DenseMatrix.h"
#include "matrix/MatrixOperations.h"

#include <array>

NS_BEGIN_NAMESPACE

namespace Math
{
	/*
		Calculates the (K-1) normal of a K-1 set of K vectors
	*/
	template<typename T, Dimension K>
	FixedVector<T,K> normal(const std::array<FixedVector<T,K>, K-1>& vectors)
	{
		static_assert(K > 1, "Normal for dimension 0 and 1 not well defined.");
		FixedVector<T,K> res;

		for(Index i = 0; i < K; ++i)
		{
			FixedMatrix<T,K-1,K-1> subMat;

			// Setup sub matrix
			for(Index j = 0; j < K-1; ++j)
			{
				Index p = 0;
				for(Index k = 0; k < K; ++k)
				{
					if(k == i)
						continue;
					
					subMat.set(j,p, vectors[j][k]);
					++p;
				}
			}

			if(i % 2)
				res[i] = -Operations::determinant(subMat);
			else
				res[i] = Operations::determinant(subMat);
		}

		return res.normalized();
	}
}

NS_END_NAMESPACE