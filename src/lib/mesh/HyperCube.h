#pragma once

#include "Mesh.h"

NS_BEGIN_NAMESPACE

namespace Mesh
{
	template<typename T, Dimension K, Dimension N>
	class HyperCube
	{
	public:
		static Mesh<T,K,N> generate(const FixedVector<Dimension,K>& elements,
			const FixedVector<T,K>& size,
			const FixedVector<T,N>& offset);
	};

	/**
	 * Grid 2D
	 */
	template<typename T, Dimension N>
	class HyperCube<T,2,N>
	{
	public:
		static Mesh<T,2,N> generate(const FixedVector<Dimension,2>& elements,
			const FixedVector<T,2>& size,
			const FixedVector<T,N>& offset);
	};
}

NS_END_NAMESPACE


#define _NS_HYPERCUBE_INL
# include "HyperCube.inl"
#undef _NS_HYPERCUBE_INL