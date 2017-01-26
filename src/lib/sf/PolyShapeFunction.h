#pragma once

#include "ShapeFunction.h"
#include "mesh/Mesh.h"
#include "matrix/DenseMatrix.h"
#include "Vector.h"

NS_BEGIN_NAMESPACE

/**
 * Implementing continious Lagrange elements
 */
template<typename T, Dimension K, Dimension Order>
class PolyShapePolicy
{
public:
	static constexpr Dimension DOF = (K+1)*Order;// Wrong

	// For the standard k-simplex
	T value(Index localComponent, const FixedVector<T,K>& local) const;

	// For the standard k-simplex
	FixedVector<T,K> gradient(Index localComponent, const FixedVector<T,K>& local) const;
};

template<typename T, Dimension K, Dimension Order>
using PolyShapeFunction = ShapeFunction<PolyShapePolicy<T,K,Order> >;

NS_END_NAMESPACE


#define _NS_POLYSHAPEFUNCTION_INL
# include "PolyShapeFunction.inl"
#undef _NS_POLYSHAPEFUNCTION_INL