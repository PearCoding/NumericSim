#pragma once

#include "ShapeFunction.h"
#include "mesh/Mesh.h"
#include "matrix/DenseMatrix.h"
#include "Vector.h"

NS_BEGIN_NAMESPACE

template<typename T, Dimension K, Dimension Order>
class PolyShapePolicy
{
public:
	// For the standard k-simplex
	template<class V>
	V value(const V& local) const;

	// For the standard k-simplex
	template<class V>
	V gradient(Index localComponent, const V& local) const;

private:

};

template<typename T, Dimension K, Dimension Order>
using PolyShapeFunction = ShapeFunction<PolyShapePolicy<T,K,Order> >;

NS_END_NAMESPACE


#define _NS_POLYSHAPEFUNCTION_INL
# include "PolyShapeFunction.inl"
#undef _NS_POLYSHAPEFUNCTION_INL