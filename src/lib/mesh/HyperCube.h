#pragma once

#include "Mesh.h"

NS_BEGIN_NAMESPACE

template<typename T, Dimension K>
class HyperCube
{
public:
	static Mesh<T,K> generate(const FixedVector<Dimension,K>& elements,
		const FixedVector<T,K>& size,
		const FixedVector<T,K>& offset);
};

/**
* Grid 2D
*/
template<typename T>
class HyperCube<T,2>
{
public:
	static Mesh<T,2> generate(const FixedVector<Dimension,2>& elements,
		const FixedVector<T,2>& size,
		const FixedVector<T,2>& offset);
};

NS_END_NAMESPACE


#define _NS_HYPERCUBE_INL
# include "HyperCube.inl"
#undef _NS_HYPERCUBE_INL