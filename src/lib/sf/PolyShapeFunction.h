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
	static constexpr Dimension DOF = (K+1)*Order;// Wrong, but works for Order < 3

	// For the standard k-simplex
	T value(Index localComponent, const FixedVector<T,K>& local) const;
	T value(Index localComponent, const FixedVector<T,K>& local, const FixedVector<T,DOF>& nodeValues) const;

	// For the standard k-simplex
	FixedVector<T,K> gradient(Index localComponent, const FixedVector<T,K>& local) const;
	FixedVector<T,K> gradient(Index localComponent, const FixedVector<T,K>& local, const FixedVector<T,DOF>& nodeValues) const;

	// For the standard k-simplex
	FixedVector<T,K> gradient2(Index localComponent, const FixedVector<T,K>& local) const;
	FixedVector<T,K> gradient2(Index localComponent, const FixedVector<T,K>& local, const FixedVector<T,DOF>& nodeValues) const;

	static void prepareMesh(Mesh<T,K>& m);
};

template<typename T, Dimension K, Dimension Order>
using PolyShapeFunction = ShapeFunction<PolyShapePolicy<T,K,Order> >;

NS_END_NAMESPACE


#define _NS_POLYSHAPEFUNCTION_INL
# include "PolyShapeFunction.inl"
#undef _NS_POLYSHAPEFUNCTION_INL